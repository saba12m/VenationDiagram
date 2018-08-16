#include "Venation3DClosed.h"

void Venation3DClosed::setup(vector <ofVec3f> _attractors, vector <ofVec3f> _nodes, vector <vector <int>> _lines, vector <int> _nodeParents,
                             int _nodeRadius, float _minX, float _maxX, float _minY, float _maxY, float _minZ, float _maxZ)
{
    attractors.clear();
    nodes.clear();
    
    hasChildren.clear();
    
    nodeThickness.clear();
    
    attractorIndices.clear();
    nodeParents.clear();
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
    nodeRadius = _nodeRadius;
    
    initial = true;
    finalize = true;

    minX = _minX;
    minY = _minY;
    minZ = _minZ;
    maxX = _maxX;
    maxY = _maxY;
    maxZ = _maxZ;
    
    // setting up the container size
    float volume = (maxX - minX) * (maxY - minY) * (maxZ - minZ);
    containerNum = _attractors.size() / 6.0;
    containerLength = cbrt(volume / containerNum);
    
    containerNumX = ceil((maxX - minX) / containerLength + 0.001);
    containerNumY = ceil((maxY - minY) / containerLength + 0.001);
    containerNumZ = ceil((maxZ - minZ) / containerLength + 0.001);

    while (containerLength < nodeRadius * 2) // just in case the container sizes are larger than the nodes
    {
        containerLength *= 2;
        containerNumX /= 2;
        containerNumY /= 2;
        containerNumZ /= 2;
    }
    containerDist = containerLength * 2.0; // change if more dispersed

    // setting up attractor containers
    containersAttractors.resize(containerNumX);
    for (int i = 0; i < containersAttractors.size(); i++)
    {
        containersAttractors[i].resize(containerNumY);
        for (int j = 0; j < containersAttractors[i].size(); j++)
            containersAttractors[i][j].resize(containerNumZ);
    }

    // setting attraction points
    for (int i = 0; i < _attractors.size(); i++)
    {
        // checking position of new attractor
        int nodeX = floor(ofMap(_attractors[i].x, minX, maxX, 0, containerNumX - 0.001));
        int nodeY = floor(ofMap(_attractors[i].y, minY, maxY, 0, containerNumY - 0.001));
        int nodeZ = floor(ofMap(_attractors[i].z, minZ, maxZ, 0, containerNumZ - 0.001));
        
        containersAttractors[nodeX][nodeY][nodeZ].push_back(i);
        attractorIndices.push_back(i);
        attractors.push_back(_attractors[i]);
        attractorContainer.push_back(vector <int> {nodeX, nodeY, nodeZ});
    }

    // setting up containers
    containers.resize(containerNumX);
    for (int i = 0; i < containers.size(); i++)
    {
        containers[i].resize(containerNumY);
        for (int j = 0; j < containers[i].size(); j++)
            containers[i][j].resize(containerNumZ);
    }

    // putting initial nodes into containers
    for (int i = 0; i < _nodes.size(); i++)
    {
        nodes.push_back(_nodes[i]);
        nodeParents.push_back(_nodeParents[i]);
        int nodeX = floor(ofMap(nodes[i].x, minX, maxX, 0, containerNumX - 0.001));
        int nodeY = floor(ofMap(nodes[i].y, minY, maxY, 0, containerNumY - 0.001));
        int nodeZ = floor(ofMap(nodes[i].z, minZ, maxZ, 0, containerNumZ - 0.001));
        containers[nodeX][nodeY][nodeZ].push_back(i);
        nodeContainer.push_back(vector <int> {nodeX, nodeY, nodeZ});
    }
    
    for (int i = 0; i < _lines.size(); i++)
        lines.push_back(_lines[i]);
    
    initialNodesSize = nodes.size();
    initialLinesSize = lines.size();
}

void Venation3DClosed::setup(int _leafRadius, int _nodeRadius, int _noOfAttractors)
{
    attractors.clear();
    nodes.clear();
    
    hasChildren.clear();
    
    nodeThickness.clear();
    
    attractorIndices.clear();
    nodeParents.clear();
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
    //    nodes.push_back(ofVec3f(-(leafRadius - nodeRadius), 0, 0));
    //    nodes.push_back(ofVec3f(0, -(leafRadius - nodeRadius), 0));
    
    for (int i = 0; i < nodes.size(); i++)
        nodeParents.push_back(-1); // meaning it doesn't have a parent
    
    // setting up the container size
    float volume = pow(leafRadius, 3);
    containerNum = _noOfAttractors / 6.0;
    containerLength = cbrt(volume / containerNum);
    
    containerNumX = leafRadius / containerLength;
    containerNumY = leafRadius / containerLength;
    containerNumZ = leafRadius / containerLength;
    
    while (containerLength < nodeRadius * 2) // just in case the container sizes are larger than the nodes
    {
        containerLength *= 2;
        containerNumX /= 2;
        containerNumY /= 2;
        containerNumZ /= 2;
    }
    containerDist = containerLength * 2.0; // change if more dispersed
    
    // setting up attractor containers
    containersAttractors.resize(containerNumX);
    for (int i = 0; i < containersAttractors.size(); i++)
    {
        containersAttractors[i].resize(containerNumY);
        for (int j = 0; j < containersAttractors[i].size(); j++)
            containersAttractors[i][j].resize(containerNumZ);
    }
    
    // setting attraction points
    int counter = 0;
    while (attractors.size() < _noOfAttractors && counter < _noOfAttractors * 2)
    {
        // create new attractor
        float r = ofRandom(leafRadius * 0.2, leafRadius - nodeRadius);
        float phi = ofRandom(ofDegToRad(360));
        float theta = ofRandom(ofDegToRad(180));
        float x = r * sin(theta) * cos(phi);
        float y = r * sin(theta) * sin(phi);
        float z = r * cos(theta);
        
        // checking position of new attractor
        int nodeX = floor(ofMap(x, -leafRadius, leafRadius, 0, containerNumX));
        int nodeY = floor(ofMap(y, -leafRadius, leafRadius, 0, containerNumY));
        int nodeZ = floor(ofMap(z, -leafRadius, leafRadius, 0, containerNumZ));
        
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
    containers.resize(containerNumX);
    for (int i = 0; i < containers.size(); i++)
    {
        containers[i].resize(containerNumY);
        for (int j = 0; j < containers[i].size(); j++)
            containers[i][j].resize(containerNumZ);
    }
    
    // putting initial nodes into containers
    for (int i = 0; i < nodes.size(); i++)
    {
        int nodeX = floor(ofMap(nodes[i].x, -leafRadius, leafRadius, 0, containerNumX));
        int nodeY = floor(ofMap(nodes[i].y, -leafRadius, leafRadius, 0, containerNumY));
        int nodeZ = floor(ofMap(nodes[i].z, -leafRadius, leafRadius, 0, containerNumZ));
        containers[nodeX][nodeY][nodeZ].push_back(i);
        nodeContainer.push_back(vector <int> {nodeX, nodeY, nodeZ});
    }
    
    initialNodesSize = nodes.size();
    initialLinesSize = lines.size();
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
//        saveFile();
        finalize = false;
    }
    initial = false;
    progressCounter++;
}

void Venation3DClosed::draw()
{
    // draw containers
    ofSetColor(50);
    for (int i = 0; i <= containerNumX; i++)
        for (int j = 0; j <= containerNumY; j++)
            for (int k = 0; k <= containerNumZ; k++)

            {
//                float x = ofMap(i, 0, containerNumX, minX, maxX);
//                float y = ofMap(j, 0, containerNumY, minY, maxY);
//                float z = ofMap(k, 0, containerNumZ, minZ, maxZ);
                float x = i * containerLength;
                float y = j * containerLength;
                float z = k * containerLength;
                float X = containerNumX * containerLength;
                float Y = containerNumY * containerLength;
                float Z = containerNumZ * containerLength;
                ofDrawLine(x + minX, y + minY,     minZ, x + minX, y + minY, Z + minZ);
                ofDrawLine(x + minX,     minY, z + minZ, x + minX, Y + minY, z + minZ);
                ofDrawLine(    minX, y + minY, z + minZ, X + minX, y + minY, z + minZ);
            }
    
    // draw passed containers
//    ofFill();
//    ofSetColor(0, 255, 0, 30);
//    for (int i = 0; i < passedContainers.size(); i++)
//        ofDrawBox((containerLength * passedContainers[i][0]) + containerLength * 0.5 + minX,
//                  (containerLength * passedContainers[i][1]) + containerLength * 0.5 + minY,
//                  (containerLength * passedContainers[i][2]) + containerLength * 0.5 + minZ,
//                  containerLength, containerLength, containerLength);
    
    // draw leaf
    ofNoFill();
    ofSetColor(200);
    ofDrawSphere(0, 0, 0, leafRadius);
    
    // draw attractors
    ofFill();
    ofSetColor(255, 150, 150);
    for (int i = 0; i < attractors.size(); i++) ofDrawSphere(attractors[i].x, attractors[i].y, attractors[i].z, nodeRadius);
    
    // draw nodes (colors based on placement in containers (R for X, G for Y, B for Z))
    for (int i = 0; i < containers.size(); i++)
        for (int j = 0; j < containers[i].size(); j++)
            for (int k = 0; k < containers[i][j].size(); k++)
            if (!containers[i][j][k].empty())
                for (int n = 0; n < containers[i][j][k].size(); n++)
                {
                    int index = containers[i][j][k][n];
                    double r = (nodeThickness.size() > 0) ? nodeThickness[index]: nodeRadius;
                    ofFill();
                    ofSetColor((255 / containers.size()) * i, (255 / containers[i].size()) * j, (255 / containers[i][j].size()) * k);
                    ofDrawSphere(nodes[index].x, nodes[index].y, nodes[index].z, r);

                    // drawing the branch tips
//                    ofNoFill();
//                    ofSetColor(255);
//                    if (hasChildren.size() > 0)
//                        if (!hasChildren[index])
//                            ofDrawSphere(nodes[index].x, nodes[index].y, nodes[index].z, r);
                }

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
    ofSetColor(255, 255, 0);
    if (nodeNeighbors.size() > 0)
        for (int i = 0; i < nodeNeighbors.size(); i++)
            if (nodeNeighbors[i].size() > 0)
                for (int j = 0; j < nodeNeighbors[i].size(); j++)
                    ofDrawLine(nodes[i].x, nodes[i].y, nodes[i].z, attractors[nodeNeighbors[i][j]].x, attractors[nodeNeighbors[i][j]].y, attractors[nodeNeighbors[i][j]].z);
}

bool Venation3DClosed::done()
{
    if (nodeThickness.size() > 0) return true;
    else return false;
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
    for (int mei = 0; mei < containerNumX; mei++)
        for (int mej = 0; mej < containerNumY; mej++)
            for (int mek = 0; mek < containerNumZ; mek++)
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

            int nodeX = floor(ofMap(newNode.x, minX, maxX, 0, containerNumX - 0.1));
            int nodeY = floor(ofMap(newNode.y, minY, maxY, 0, containerNumY - 0.1));
            int nodeZ = floor(ofMap(newNode.z, minZ, maxZ, 0, containerNumZ - 0.1));
            
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
        if (!hasChildren[i])
        {
            int index = i;
            int counter = 1;
            int counterMax = 1;
            while (index > -1)
            {
                counterMax++;
                index = nodeParents[index];
            }
            double denominator = log(counterMax);
            
            index = i;
            while (index > -1)
            {
                counter++;
                nodeThickness[index] += 1 + log(counter) / denominator;
                nodeThickness[index] += 1;
                index = nodeParents[index];
            }
        }
    
    for (int i = 0; i < nodes.size(); i++)
        nodeThickness[i] = pow(nodeThickness[i], 1.0 / 2.4);
}

void Venation3DClosed::finalRngStructure()
{
    // getting initial structure
    for (int i = 0; i < initialLinesSize; i++)
        finalLines.push_back(lines[i]);

    // getting max distances for each pair
    for (int i = initialNodesSize; i < nodes.size(); i++)
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

void Venation3DClosed::saveFile()
{
    if (finalLines.size() > 0)
    {
        ofxCsv csvRecorder;
        csvRecorder.clear();
        for (int i = 0; i < finalLines.size(); i++)
        {
            ofxCsvRow row;
            
            // index
            row.setInt(0, i);
            
            // start point
            int startIndex = finalLines[i][0];
            float startX = nodes[startIndex].x;
            float startY = nodes[startIndex].y;
            float startZ = nodes[startIndex].z;
            
            row.setFloat(1, startX);
            row.setFloat(2, startY);
            row.setFloat(3, startZ);
            
            // end point
            int endIndex = finalLines[i][1];
            float endX = nodes[endIndex].x;
            float endY = nodes[endIndex].y;
            float endZ = nodes[endIndex].z;
            
            row.setFloat(4, endX);
            row.setFloat(5, endY);
            row.setFloat(6, endZ);
            
            // thicknesses
            double startThickness = nodeThickness[startIndex];
            double endThickness = nodeThickness[endIndex];
            
            row.setFloat(7, startThickness);
            row.setFloat(8, endThickness);
            
            csvRecorder.addRow(row);
        }
        string d = "VenationClosed_";
        d.append(ofToString(ofGetFrameNum()));
        d.append(".csv");
        csvRecorder.save(d);
    }
}

void Venation3DClosed::saveFile(int num)
{
    if (finalLines.size() > 0)
    {
        ofxCsv csvRecorder;
        csvRecorder.clear();
        for (int i = 0; i < finalLines.size(); i++)
        {
            ofxCsvRow row;
            
            // index
            row.setInt(0, i);
            
            // start point
            int startIndex = finalLines[i][0];
            float startX = nodes[startIndex].x;
            float startY = nodes[startIndex].y;
            float startZ = nodes[startIndex].z;
            
            row.setFloat(1, startX);
            row.setFloat(2, startY);
            row.setFloat(3, startZ);
            
            // end point
            int endIndex = finalLines[i][1];
            float endX = nodes[endIndex].x;
            float endY = nodes[endIndex].y;
            float endZ = nodes[endIndex].z;
            
            row.setFloat(4, endX);
            row.setFloat(5, endY);
            row.setFloat(6, endZ);
            
            // thicknesses
            double startThickness = nodeThickness[startIndex];
            double endThickness = nodeThickness[endIndex];
            
            row.setFloat(7, startThickness);
            row.setFloat(8, endThickness);
            
            csvRecorder.addRow(row);
        }
        string d = "VenationMerged_";
        d.append(ofToString(num));
        d.append(".csv");
        csvRecorder.save(d);
    }
}
