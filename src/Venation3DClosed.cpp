#include "Venation3DClosed.h"

void Venation3DClosed::setup(int _leafRadius, int _nodeRadius, int _noOfAttractors)
{
    hasChildren.clear();
    
    attractors.clear();
    nodes.clear();
    
    attractorIndices.clear();
    nodeParents.clear();
    nodeThickness.clear();
    passedNodes.clear();
    nodeNeighbors.clear();
    attractorNeighbors.clear();
    passedContainers.clear();
    nodeContainer.clear();
    attractorContainer.clear();
    lines.clear();
    finalLines.clear();
    containers.clear();
    containersAttractors.clear();
    
    progressCounter = 0;
    leafRadius = _leafRadius;
    nodeRadius = _nodeRadius;
    
    initial = true;
    finalize = true;
    
    // setting starting Point
    nodes.push_back(ofVec3f(0, 0, 0));
    //    nodes.push_back(ofVec3f(0, 0, -(leafRadius - nodeRadius)));
    nodeParents.push_back(-1); // meaning it doesn't have a parent

    // setting up the container size
    containerNum = (int) sqrt(_noOfAttractors / 12);
    containerLength = (float) leafRadius * 2 / (float) containerNum;
    while (containerLength < nodeRadius * 2) // just in case the container sizes are larger than the nodes
    {
        containerLength * 2;
        containerNum / 2;
    }
    containerDist = containerLength * 2.0; // change if more dispersed

    // setting up attractor containers
    containersAttractors.resize(containerNum);
    for (int i = 0; i < containersAttractors.size(); i++)
    {
        containersAttractors[i].resize(containerNum);
        for (int j = 0; j < containersAttractors[i].size(); j++)
            containersAttractors[i][j].resize(containerNum);
    }

    // setting attraction points
    int counter = 0;
    while (attractors.size() < _noOfAttractors && counter < _noOfAttractors * 2)
    {
        // create new attractor
        float r = ofRandom(leafRadius * 0.1, leafRadius - nodeRadius);
        float phi = ofRandom(ofDegToRad(360));
        float theta = ofRandom(ofDegToRad(180));
        float x = r * sin(theta) * cos(phi);
        float y = r * sin(theta) * sin(phi);
        float z = r * cos(theta);
 
        // checking position of new attractor
        int nodeX = floor(ofMap(x, -leafRadius, leafRadius, 0, containerNum));
        int nodeY = floor(ofMap(y, -leafRadius, leafRadius, 0, containerNum));
        int nodeZ = floor(ofMap(z, -leafRadius, leafRadius, 0, containerNum));

        bool addToAttractors = true;
        for (int i = nodeX - 1; i <= nodeX + 1; i++)
            if (addToAttractors)
                for (int j = nodeY - 1; j <= nodeY + 1; j++)
                    if (addToAttractors)
                        for (int k = nodeZ - 1; k <= nodeZ + 1; k++)
                            if (addToAttractors)
                                if (i >= 0 && i < containersAttractors.size() && j >= 0 && j < containersAttractors[i].size() && k >= 0 && k < containersAttractors[i][j].size())
                                    for (int n = 0; n < containersAttractors[i][j][k].size(); n++)
                                        if (addToAttractors)
                                        {
                                            ofVec3f attractor = attractors[containersAttractors[i][j][k][n]];
                                            float dist = ofDist(x, y, z, attractor.x, attractor.y, attractor.z);
                                            if (dist < nodeRadius * 2) addToAttractors = false;
                                        }

        // adding new attractor
        if (addToAttractors)
        {
            containersAttractors[nodeX][nodeY][nodeZ].push_back(attractors.size());
            attractorIndices.push_back(attractors.size());
            attractors.push_back(ofVec3f(x, y, z));
            attractorContainer.push_back(vector <int> {nodeX, nodeY, nodeZ});
        }

        counter++;
    }
    
    // setting up containers
    containers.resize(containerNum);
    for (int i = 0; i < containers.size(); i++)
    {
        containers[i].resize(containerNum);
        for (int j = 0; j < containers[i].size(); j++)
            containers[i][j].resize(containerNum);
    }

    // putting initial nodes into containers
    for (int i = 0; i < nodes.size(); i++)
    {
        int nodeX = floor(ofMap(nodes[i].x, -leafRadius, leafRadius, 0, containerNum));
        int nodeY = floor(ofMap(nodes[i].y, -leafRadius, leafRadius, 0, containerNum));
        int nodeZ = floor(ofMap(nodes[i].z, -leafRadius, leafRadius, 0, containerNum));
        containers[nodeX][nodeY][nodeZ].push_back(i);
        nodeContainer.push_back(vector <int> {nodeX, nodeY, nodeZ});
    }
}

void Venation3DClosed::update()
{
    if (!initial) attractorCheck();
    if (progressCounter % 5 == 0) nodePassedCheck();
    generateRng();
    generateNewNodes();
    if (attractors.empty() && finalize)
    {
        calculateThickness();
        finalRngStructure();
        finalize = false;
    }
    initial = false;
    progressCounter++;
}

void Venation3DClosed::draw()
{
    // draw containers
    ofSetColor(50);
    for (int i = 0; i <= containerNum; i++)
        for (int j = 0; j <= containerNum; j++)
            for (int k = 0; k <= containerNum; k++)

            {
                float x = ofMap(i, 0, containerNum, -leafRadius, leafRadius);
                float y = ofMap(j, 0, containerNum, -leafRadius, leafRadius);
                float z = ofMap(k, 0, containerNum, -leafRadius, leafRadius);
                ofDrawLine(x, y, -leafRadius, x, y, leafRadius);
                ofDrawLine(x, -leafRadius, z, x, leafRadius, z);
                ofDrawLine(-leafRadius, y, z, leafRadius, y, z);
            }
    
    // draw passed containers
//    ofFill();
//    ofSetColor(0, 255, 0, 30);
//    for (int i = 0; i < passedContainers.size(); i++)
//    {
//        ofDrawBox((containerLength * passedContainers[i][0]) - (containerLength * (containerNum - 1) / 2),
//                  (containerLength * passedContainers[i][1]) - (containerLength * (containerNum - 1) / 2),
//                  (containerLength * passedContainers[i][2]) - (containerLength * (containerNum - 1) / 2),
//                  containerLength, containerLength, containerLength);
//    }
    
    // draw leaf
    ofNoFill();
    ofSetColor(200);
    ofDrawSphere(0, 0, 0, leafRadius);
    
    // draw attractors
    ofFill();
    ofSetColor(255, 150, 150);
    for (int i = 0; i < attractors.size(); i++) ofDrawSphere(attractors[i].x, attractors[i].y, attractors[i].z, nodeRadius);
    
    // draw nodes (colors based on placement in containers (R for X, G for Y, B for Z))
//    ofFill();
//    for (int i = 0; i < containers.size(); i++)
//        for (int j = 0; j < containers[i].size(); j++)
//            for (int k = 0; k < containers[i][j].size(); k++)
//            if (!containers[i][j][k].empty())
//                for (int n = 0; n < containers[i][j][k].size(); n++)
//                {ofFill();
//                    int index = containers[i][j][k][n];
//                    float r = (nodeThickness.size() > 0) ? nodeThickness[index] / 2.0 : nodeRadius;
//                    ofSetColor((255 / containers.size()) * i, (255 / containers[i].size()) * j, (255 / containers[i][j].size()) * k);
//                    ofDrawSphere(nodes[index].x, nodes[index].y, nodes[index].z, r);
//                    ofNoFill();
//                    ofSetColor(255);
//                    if (hasChildren.size() > 0)
//                        if (!hasChildren[index])
//                            ofDrawSphere(nodes[index].x, nodes[index].y, nodes[index].z, r);
//                }

    // draw tree lines (with thickness at the end)
    ofPushStyle();
    ofSetColor(0, 255, 255, 150);
    ofSetLineWidth(4);
    if (finalLines.size() > 0)
        for (int i = 0; i < finalLines.size(); i++)
        {
            ofVec3f a = nodes[finalLines[i][0]];
            ofVec3f b = nodes[finalLines[i][1]];
            float w = (nodeThickness[finalLines[i][0]] + nodeThickness[finalLines[i][1]]) / 2.0;
            ofSetLineWidth(w);
            ofDrawLine(a.x, a.y, a.z, b.x, b.y, b.z);
        }
    else
        for (int i = 0; i < lines.size(); i++)
        {
            if (i < newLinesIndex) ofSetColor(0, 0, 255, 150);
            else ofSetColor(255, 0, 0, 100);
            ofVec3f a = nodes[lines[i][0]];
            ofVec3f b = nodes[lines[i][1]];
            ofDrawLine(a.x, a.y, a.z, b.x, b.y, b.z);
        }
    ofPopStyle();

    //draw RNG growth lines
    ofSetLineWidth(1);
    ofSetColor(0, 150, 0);
    if (nodeNeighbors.size() > 0)
        for (int i = 0; i < nodeNeighbors.size(); i++)
            if (nodeNeighbors[i].size() > 0)
                for (int j = 0; j < nodeNeighbors[i].size(); j++)
                    ofDrawLine(nodes[i].x, nodes[i].y, nodes[i].z, attractors[nodeNeighbors[i][j]].x, attractors[nodeNeighbors[i][j]].y, attractors[nodeNeighbors[i][j]].z);
}

void Venation3DClosed::saveFile()
{
    ofxCsv csvRecorder;
    csvRecorder.clear();
    ofxCsvRow row;
    row.setInt(0, 876);
    row.setInt(1, 543);
    csvRecorder.addRow(row);
    row.setString(0, "hello");
    row.setString(1, "hi");
    row.setString(2, "hey there");
    row.setInt(3, 456);
    row.setInt(0, 79);
    row.setString(6, "me");
    csvRecorder.addRow(row);
    string d = "Data";
    int w = 143;
    d.append(ofToString(ofGetFrameNum()));
    d.append(".csv");
    csvRecorder.save(d);
}

void Venation3DClosed::attractorCheck()
{
    float attractorDist = 0;
    for (int i = attractors.size() - 1; i >= 0 ; i--)
    {
        bool deleteAttractor = true;
        int attractorNeighborSize = attractorNeighbors[i].size();
        if (attractorNeighborSize > 0)
            for (int j = 0; j < attractorNeighborSize; j++)
            {
                int neighbor  = attractorNeighbors[i][j];
                attractorDist = ofDist(attractors[i].x, attractors[i].y, attractors[i].z, nodes[neighbor].x, nodes[neighbor].y, nodes[neighbor].z);
                if (attractorDist > nodeRadius * 4)
                {
                    deleteAttractor = false;
                    break;
                }
            }
        else deleteAttractor = false;
        if (deleteAttractor)
        {
            int attractorX = attractorContainer[attractorIndices[i]][0];
            int attractorY = attractorContainer[attractorIndices[i]][1];
            int attractorZ = attractorContainer[attractorIndices[i]][2];
            containersAttractors[attractorX][attractorY][attractorZ].erase(remove(containersAttractors[attractorX][attractorY][attractorZ].begin(), containersAttractors[attractorX][attractorY][attractorZ].end(), attractorIndices[i]), containersAttractors[attractorX][attractorY][attractorZ].end());
            attractorIndices.erase(attractorIndices.begin() + i);
            attractors.erase(attractors.begin() + i);
        }
    }
}

void Venation3DClosed::nodePassedCheck()
{
    for (int mei = 0; mei < containerNum; mei++)
        for (int mej = 0; mej < containerNum; mej++)
            for (int mek = 0; mek < containerNum; mek++)
                if (!(find(passedContainers.begin(), passedContainers.end(), vector <int> {mei, mej, mek}) != passedContainers.end()))
                {
                    bool passed = true;
                    for (int i = mei - 1; i <= mei + 1; i++)
                        if (passed)
                            for (int j = mej - 1; j <= mej + 1; j++)
                                if (passed)
                                    for (int k = mek - 1; k <= mek + 1; k++)
                                        if (passed)
                                            if (i >= 0 && i < containers.size() && j >= 0 && j < containers[i].size() && k >= 0 && k < containers[i][j].size())
                                                if (containersAttractors[i][j][k].size() > 0) passed = false;
                    if (passed)
                    {
                        passedContainers.push_back(vector <int> {mei, mej, mek});
                        for (int n = 0; n < containers[mei][mej][mek].size(); n++)
                            passedNodes.push_back(containers[mei][mej][mek][n]);
                    }
                }
}

void Venation3DClosed::generateRng()
{
    rngSize = nodes.size() + attractors.size();
    int nSize = nodes.size();
    int aSize = attractors.size();

    nodeNeighbors.clear();
    nodeNeighbors.resize(nodes.size());

    attractorNeighbors.clear();
    attractorNeighbors.resize(attractors.size());

    // getting max distances for each pair
    for (int i = 0; i < nSize; i++)
        if (!(find(passedNodes.begin(), passedNodes.end(), i) != passedNodes.end()))
            for (int j = 0; j < aSize; j++)
            {
                bool relativeNeighborhood = true;
                float ijDist = ofDist(nodes[i].x, nodes[i].y, nodes[i].z, attractors[j].x, attractors[j].y, attractors[j].z);

                int nodeX = nodeContainer[i][0];
                int nodeY = nodeContainer[i][1];
                int nodeZ = nodeContainer[i][2];
                int attractorX = attractorContainer[j][0];
                int attractorY = attractorContainer[j][1];
                int attractorZ = attractorContainer[j][2];

                if (abs(nodeX - attractorX) <  containerDist && abs(nodeY - attractorY) < containerDist && abs(nodeZ - attractorZ) < containerDist)
                {
                    for (int k = 0; k < nSize + aSize; k++)
                        if (i != k && j + nSize != k)
                        {
                            float ikDist;
                            float jkDist;
                            if (k < nSize)
                            {
                                ikDist = ofDist(nodes[i].x, nodes[i].y, nodes[i].z, nodes[k].x, nodes[k].y, nodes[k].z);
                                jkDist = ofDist(attractors[j].x, attractors[j].y, attractors[j].z, nodes[k].x, nodes[k].y, nodes[k].z);
                            }
                            else
                            {
                                ikDist = ofDist(nodes[i].x, nodes[i].y, nodes[i].z, attractors[k - nSize].x, attractors[k - nSize].y, attractors[k - nSize].z);
                                jkDist = ofDist(attractors[j].x, attractors[j].y, attractors[j].z, attractors[k - nSize].x, attractors[k - nSize].y, attractors[k - nSize].z);
                            }
                            float maxDist = max(ikDist, jkDist);

                            // checking if there is a distance smaller than the pair distance
                            if (maxDist < ijDist)
                            {
                                relativeNeighborhood = false;
                                break;
                            }
                        }
                }
                else relativeNeighborhood = false;

                if (relativeNeighborhood)
                {
                    nodeNeighbors[i].push_back(j);
                    attractorNeighbors[j].push_back(i);
                }
            }
}

void Venation3DClosed::generateNewNodes()
{
    newLinesIndex = lines.size();
    vector <ofVec3f> newNodes;
    ofVec3f newNode;
    ofVec3f attractionDist;
    vector <int> newLine;
    int n = nodes.size();
    for (int i = 0 ; i < n; i++)
    {
        newNode.set(0, 0, 0);
        int nodeAttractorSize = nodeNeighbors[i].size();
        if (nodeAttractorSize > 0)
        {
            newLine.push_back(i);
            for (int j = 0; j < nodeAttractorSize; j++)
            {
                attractionDist = attractors[nodeNeighbors[i][j]] - nodes[i];
                attractionDist.normalize();
                newNode += attractionDist;
            }
            if (newNode.length() < nodeRadius * 0.5) newNode = attractionDist;
            newNode.normalize();
            newNode *= nodeRadius * 2;
            newNode += nodes[i];

            int nodeX = floor(ofMap(newNode.x, -leafRadius, leafRadius, 0, containerNum));
            int nodeY = floor(ofMap(newNode.y, -leafRadius, leafRadius, 0, containerNum));
            int nodeZ = floor(ofMap(newNode.z, -leafRadius, leafRadius, 0, containerNum));
            
            // check proximity to existing nodes
            float proximityRatio = 0.8;
            bool acceptableNodeProximity = true;
            for (int i = nodeX - 1; i <= nodeX + 1; i++)
                if (acceptableNodeProximity)
                    for (int j = nodeY - 1; j <= nodeY + 1; j++)
                        if (acceptableNodeProximity)
                            for (int k = nodeZ - 1; k <= nodeZ + 1; k++)
                                if (acceptableNodeProximity)
                                    if (i >= 0 && i < containers.size() && j >= 0 && j < containers[i].size() && k >= 0 && k < containers[i][j].size())
                                        if (containers[i][j][k].size() > 0)
                                            for (int n = 0; n < containers[i][j][k].size(); n++)
                                                if (acceptableNodeProximity)
                                                {
                                                    ofVec3f otherNode = nodes[containers[i][j][k][n]];
                                                    float dist = ofDist(newNode.x, newNode.y, newNode.z, otherNode.x, otherNode.y, otherNode.z);
                                                    if (dist < nodeRadius * proximityRatio) acceptableNodeProximity = false;
                                                }
            if (acceptableNodeProximity)
            {
                // add the new data to the existing
                containers[nodeX][nodeY][nodeZ].push_back(nodes.size());
                nodeContainer.push_back(vector <int> {nodeX, nodeY, nodeZ});
                newLine.push_back(nodes.size());
                lines.push_back(newLine);
                nodes.push_back(newNode);
                nodeParents.push_back(i);
            }
            newLine.clear();
        }
    }
}

void Venation3DClosed::calculateThickness()
{
    // calculating end notes (without children)
    hasChildren.resize(nodes.size(), false);
    for (int i = 0; i < nodes.size(); i++)
        if (nodeParents[i] > -1)
            hasChildren[nodeParents[i]] = true;

    // calculating each node's thickness
    nodeThickness.resize(nodes.size(), 0);
    for (int i = 0; i < nodes.size(); i++)
    {
        if (!hasChildren[i])
        {
            int index = i;
            while (index > -1)
            {
                nodeThickness[index]++;
                index = nodeParents[index];
            }
        }
    }
}

void Venation3DClosed::finalRngStructure()
{
    // getting max distances for each pair
    for (int i = 0; i < nodes.size(); i++)
        for (int j = 0; j < i; j++)
        {
            bool relativeNeighborhood = true;
            float ijDist = ofDist(nodes[i].x, nodes[i].y, nodes[i].z, nodes[j].x, nodes[j].y, nodes[j].z);

            int iNodeX = nodeContainer[i][0];
            int iNodeY = nodeContainer[i][1];
            int iNodeZ = nodeContainer[i][2];
            int jNodeX = nodeContainer[j][0];
            int jNodeY = nodeContainer[j][1];
            int jNodeZ = nodeContainer[j][2];
            if (abs(iNodeX - jNodeX) <  containerDist && abs(iNodeY - jNodeY) < containerDist && abs(iNodeZ - jNodeZ) < containerDist)
            {
                for (int k = 0; k < nodes.size(); k++)
                    if (i != k && j != k)
                    {
                        float ikDist;
                        float jkDist;

                        ikDist = ofDist(nodes[i].x, nodes[i].y, nodes[i].z, nodes[k].x, nodes[k].y, nodes[k].z);
                        jkDist = ofDist(nodes[j].x, nodes[j].y, nodes[j].z, nodes[k].x, nodes[k].y, nodes[k].z);

                        float maxDist = max(ikDist, jkDist);

                        // checking if there is a distance smaller than the pair distance
                        if (maxDist < ijDist)
                        {
                            relativeNeighborhood = false;
                            break;
                        }
                    }
            }
            else relativeNeighborhood = false;

            if (relativeNeighborhood)
                finalLines.push_back(vector <int> {i, j});
        }
}
