#include "Venation2DClosed.h"

void Venation2DClosed::setup(int _leafRadius, int _nodeRadius, int _noOfAttractors)
{
    hasChildren.clear();
    
    attractors.clear();
    nodes.clear();
    
    attractorIndices.clear();
    nodeParents.clear();
    nodeThickness.clear();
    passedNodes.clear();
    lines.clear();
    finalLines.clear();
    nodeContainer.clear();
    attractorContainer.clear();
    lineContainer.clear();
    nodeNeighbors.clear();
    attractorNeighbors.clear();
    passedContainers.clear();
    containers.clear();
    containersAttractors.clear();
    
    progressCounter = 0;
    leafRadius = _leafRadius;
    nodeRadius = _nodeRadius;

    initial = true;
    finalize = true;
    
    // setting starting Point
    nodes.push_back(ofVec2f(0, 0));
//    nodes.push_back(ofVec2f(0, -(leafRadius - nodeRadius)));
    nodeParents.push_back(-1); // meaning it doesn't have a parent
    
    // setting up the container size
    containerNum = (int) leafRadius / (nodeRadius * 4.0);
    containerLength = (float) leafRadius * 2 / (float) containerNum;
    while (containerLength < nodeRadius * 2) // just in case the container sizes are larger than the nodes
    {
        containerLength * 2;
        containerNum / 2;
    }
    
    // setting up attractor containers
    containersAttractors.resize(containerNum);
    for (int i = 0; i < containersAttractors.size(); i++) containersAttractors[i].resize(containerNum);

    // setting attraction points
    int counter = 0;
    while (attractors.size() < _noOfAttractors && counter < _noOfAttractors * 2)
    {
        // create new attractor
        float distance = ofRandom(leafRadius * 0.0, leafRadius - nodeRadius);
        float angle = ofRandom(ofDegToRad(360));
        float x = cos(angle) * distance;
        float y = sin(angle) * distance;
        
        // checking position of new attractor
        int nodeX = floor(ofMap(x, -leafRadius, leafRadius, 0, containerNum));
        int nodeY = floor(ofMap(y, -leafRadius, leafRadius, 0, containerNum));

        bool addToAttractors = true;
        for (int i = nodeX - 1; i <= nodeX + 1; i++)
            if (addToAttractors)
                for (int j = nodeY - 1; j <= nodeY + 1; j++)
                    if (addToAttractors)
                        if (i >= 0 && i < containersAttractors.size() && j >= 0 && j < containersAttractors[i].size())
                            for (int k = 0; k < containersAttractors[i][j].size(); k++)
                                if (addToAttractors)
                                {
                                    ofVec2f attractor = attractors[containersAttractors[i][j][k]];
                                    float dist = ofDist(x, y, attractor.x, attractor.y);
                                    if (dist < nodeRadius * 2) addToAttractors = false;
                                }

        // adding new attractor
        if (addToAttractors)
        {
            containersAttractors[nodeX][nodeY].push_back(attractors.size());
            attractorIndices.push_back(attractors.size());
            attractors.push_back(ofVec2f(x, y));
            attractorContainer.push_back(vector <int> {nodeX, nodeY});
        }
        
        counter++;
    }

    // setting up containers
    containers.resize(containerNum);
    for (int i = 0; i < containers.size(); i++) containers[i].resize(containerNum);
    
    // putting initial nodes into containers
    for (int i = 0; i < nodes.size(); i++)
    {
        int nodeX = floor(ofMap(nodes[i].x, -leafRadius, leafRadius, 0, containerNum));
        int nodeY = floor(ofMap(nodes[i].y, -leafRadius, leafRadius, 0, containerNum));
        containers[nodeX][nodeY].push_back(i);
        nodeContainer.push_back(vector <int> {nodeX, nodeY});
    }
}

void Venation2DClosed::update()
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

void Venation2DClosed::draw()
{
    // draw containers
    ofSetColor(50);
    for (int i = 0; i <= containerNum; i++)
        for (int j = 0; j <= containerNum; j++)
        {
            float x = ofMap(i, 0, containerNum, -leafRadius, leafRadius);
            float y = ofMap(j, 0, containerNum, -leafRadius, leafRadius);
            ofDrawLine(x, -leafRadius, x, leafRadius);
            ofDrawLine(-leafRadius, y, leafRadius, y);
        }
    
    // draw passed containers
    ofFill();
    ofSetColor(0, 255, 0, 30);
    for (int i = 0; i < passedContainers.size(); i++)
    {
        ofDrawRectangle(passedContainers[i][0] * containerLength - containerLength * containerNum / 2, passedContainers[i][1] * containerLength - containerLength * containerNum / 2, containerLength, containerLength);
    }
    
    // draw leaf
    ofNoFill();
    ofSetColor(200);
    ofDrawCircle(0, 0, leafRadius);
    
    // draw attractors
    ofSetColor(255, 150, 150);
    for (int i = 0; i < attractors.size(); i++) ofDrawCircle(attractors[i].x, attractors[i].y, nodeRadius);
    
    // draw nodes (colors based on placement in containers (R for X, G for Y, B for Z))
    ofFill();
    for (int i = 0; i < containers.size(); i++)
        for (int j = 0; j < containers[i].size(); j++)
            if (!containers[i][j].empty())
                for (int k = 0; k < containers[i][j].size(); k++)
                {ofFill();
                    int index = containers[i][j][k];
                    float r = (nodeThickness.size() > 0) ? nodeThickness[index] / 2.0 : nodeRadius;
                    float blue = (hasChildren.size() > 0) ? !hasChildren[index] * 255 : 0;
                    ofSetColor((255 / containers.size()) * i, (255 / containers[i].size()) * j, 0);
                    ofDrawCircle(nodes[index].x, nodes[index].y, r);
                    ofNoFill();
                    ofSetColor(255);
                    if (hasChildren.size() > 0)
                        if (!hasChildren[index])
                            ofDrawCircle(nodes[index].x, nodes[index].y, r);
                }
    
    // draw tree lines (with thickness at the end)
    ofPushStyle();
    ofSetColor(0, 255, 255, 150);
    ofSetLineWidth(4);
    if (finalLines.size() > 0)
        for (int i = 0; i < finalLines.size(); i++)
        {
            ofVec2f a = nodes[finalLines[i][0]];
            ofVec2f b = nodes[finalLines[i][1]];
            float w = (nodeThickness[finalLines[i][0]] + nodeThickness[finalLines[i][1]]) / 2.0;
            ofSetLineWidth(w);
            ofDrawLine(a.x, a.y, b.x, b.y);
        }
    else
        for (int i = 0; i < lines.size(); i++)
        {
            if (i < newLinesIndex) ofSetColor(0, 0, 255, 150);
            else ofSetColor(255, 0, 0, 100);
            ofVec2f a = nodes[lines[i][0]];
            ofVec2f b = nodes[lines[i][1]];
            ofDrawLine(a.x, a.y, b.x, b.y);
        }
    ofPopStyle();
    
    //draw RNG growth lines
    ofSetLineWidth(1);
    ofSetColor(0, 150, 0);
    if (nodeNeighbors.size() > 0)
        for (int i = 0; i < nodeNeighbors.size(); i++)
            if (nodeNeighbors[i].size() > 0)
                for (int j = 0; j < nodeNeighbors[i].size(); j++)
                    ofDrawLine(nodes[i].x, nodes[i].y, attractors[nodeNeighbors[i][j]].x, attractors[nodeNeighbors[i][j]].y);
}

void Venation2DClosed::attractorCheck()
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
                attractorDist = ofDist(attractors[i].x, attractors[i].y, nodes[neighbor].x, nodes[neighbor].y);
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
            containersAttractors[attractorX][attractorY].erase(remove(containersAttractors[attractorX][attractorY].begin(), containersAttractors[attractorX][attractorY].end(), attractorIndices[i]), containersAttractors[attractorX][attractorY].end());
            attractorIndices.erase(attractorIndices.begin() + i);
            attractors.erase(attractors.begin() + i);
        }
    }
}

void Venation2DClosed::nodePassedCheck()
{
    for (int mei = 0; mei < containerNum; mei++)
        for (int mej = 0; mej < containerNum; mej++)
            if (!(find(passedContainers.begin(), passedContainers.end(), vector <int> {mei, mej}) != passedContainers.end()))
            {
                bool passed = true;
                for (int i = mei - 1; i <= mei + 1; i++)
                    if (passed)
                        for (int j = mej - 1; j <= mej + 1; j++)
                            if (passed)
                                if (i >= 0 && i < containers.size() && j >= 0 && j < containers[i].size())
                                    if (containersAttractors[i][j].size() > 0) passed = false;
                if (passed)
                {
                    passedContainers.push_back(vector <int> {mei, mej});
                    for (int k = 0; k < containers[mei][mej].size(); k++)
                        passedNodes.push_back(containers[mei][mej][k]);
                }
            }
}

void Venation2DClosed::generateRng()
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
            float ijDist = ofDist(nodes[i].x, nodes[i].y, attractors[j].x, attractors[j].y);
            
            int nodeX = nodeContainer[i][0];
            int nodeY = nodeContainer[i][1];
            int attractorX = attractorContainer[j][0];
            int attractorY = attractorContainer[j][1];
            
            float continerDist = containerNum * containerLength / 4.0;
            if (abs(nodeX - attractorX) <  continerDist && abs(nodeY - attractorY) < continerDist)
            {
                for (int k = 0; k < nSize + aSize; k++)
                    if (i != k && j + nSize != k)
                    {
                        float ikDist;
                        float jkDist;
                        if (k < nSize)
                        {
                            ikDist = ofDist(nodes[i].x, nodes[i].y, nodes[k].x, nodes[k].y);
                            jkDist = ofDist(attractors[j].x, attractors[j].y, nodes[k].x, nodes[k].y);
                        }
                        else
                        {
                            ikDist = ofDist(nodes[i].x, nodes[i].y, attractors[k - nSize].x, attractors[k - nSize].y);
                            jkDist = ofDist(attractors[j].x, attractors[j].y, attractors[k - nSize].x, attractors[k - nSize].y);
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

void Venation2DClosed::generateNewNodes()
{
    newLinesIndex = lines.size();
    vector <ofVec2f> newNodes;
    vector <int> newNodeParents;
    vector <vector <int>> newLines;
    ofVec2f newNode;
    ofVec2f attractorNode;
    vector <int> newLine;
    int n = nodes.size();
    for (int i = 0 ; i < n; i++)
    {
        newNode.set(0, 0);
        int nodeAttractorSize = nodeNeighbors[i].size();
        if (nodeAttractorSize > 0)
        {
            newLine.push_back(i);
            newNodeParents.push_back(i);
            for (int j = 0; j < nodeAttractorSize; j++)
            {
                attractorNode = attractors[nodeNeighbors[i][j]] - nodes[i];
                if (attractorNode.length() < nodeRadius * 2) break;
                attractorNode.normalize();
                newNode += attractorNode;
            }
            if (newNode.length() < nodeRadius * 0.5) newNode = attractorNode;
            newNode.normalize();
            newNode *= nodeRadius * 2;
            newNode += nodes[i];
            
            int nodeX = floor(ofMap(newNode.x, -leafRadius, leafRadius, 0, containerNum));
            int nodeY = floor(ofMap(newNode.y, -leafRadius, leafRadius, 0, containerNum));
            
            // check proximity to existing nodes
            float proximityRatio = 0.8;
            bool acceptableNodeProximity = true;
            for (int i = nodeX - 1; i <= nodeX + 1; i++)
                if (acceptableNodeProximity)
                    for (int j = nodeY - 1; j <= nodeY + 1; j++)
                        if (acceptableNodeProximity)
                            if (i >= 0 && i < containers.size() && j >= 0 && j < containers[i].size())
                                if (containers[i][j].size() > 0)
                                    for (int k = 0; k < containers[i][j].size(); k++)
                                        if (acceptableNodeProximity)
                                        {
                                            ofVec2f otherNode = nodes[containers[i][j][k]];
                                            float dist = ofDist(newNode.x, newNode.y, otherNode.x, otherNode.y);
                                            if (dist < nodeRadius * proximityRatio) acceptableNodeProximity = false;
                                        }
            if (acceptableNodeProximity)
            {
                // add the new data to the existing
                containers[nodeX][nodeY].push_back(nodes.size());
                nodeContainer.push_back(vector <int> {nodeX, nodeY});
                newLine.push_back(nodes.size());
                lines.push_back(newLine);
                nodes.push_back(newNode);
                nodeParents.push_back(i);
            }
            newLine.clear();
        }
    }
}

void Venation2DClosed::calculateThickness()
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

void Venation2DClosed::finalRngStructure()
{
    // getting max distances for each pair
    for (int i = 0; i < nodes.size(); i++)
        for (int j = 0; j < i; j++)
        {
            bool relativeNeighborhood = true;
            float ijDist = ofDist(nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y);
            
            int iNodeX = nodeContainer[i][0];
            int iNodeY = nodeContainer[i][1];
            int jNodeX = nodeContainer[j][0];
            int jNodeY = nodeContainer[j][1];
            float continerDist = containerNum * containerLength / 4.0;
            if (abs(iNodeX - jNodeX) <  continerDist && abs(iNodeY - jNodeY) < continerDist)
            {
                for (int k = 0; k < nodes.size(); k++)
                    if (i != k && j != k)
                    {
                        float ikDist;
                        float jkDist;
                        
                        ikDist = ofDist(nodes[i].x, nodes[i].y, nodes[k].x, nodes[k].y);
                        jkDist = ofDist(nodes[j].x, nodes[j].y, nodes[k].x, nodes[k].y);
                        
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
