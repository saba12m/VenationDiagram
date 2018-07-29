#include "Venation2DClosed.h"

void Venation2DClosed::setup(int _leafRadius, int _nodeRadius, int _noOfAttractors)
{
    attractors.clear();
    attractorsOriginal.clear();
    nodes.clear();
    lines.clear();
    linesOptimized.clear();
    rng.clear();
    rngOptimized.clear();
    strayLinesNodesIndex.clear();
    strayLines.clear();
    containers.clear();
    containersOptimized.clear();
    containersLinesOptimized.clear();
    nodeContainer.clear();
    nodeContainerOptimized.clear();
    lineContainer.clear();
    nodesOptimized.clear();
    nodesDeleted.clear();
    branches.clear();
    currentBranches.clear();
    leafRadius = _leafRadius;
    nodeRadius = _nodeRadius;
    initial = true;
    optimizeNodes = true;
    // setting starting Point
    nodes.push_back(ofVec2f(0, 0));
//    nodes.push_back(ofVec2f(0, -(leafRadius - nodeRadius)));
    
    
    // setting up container sizes
    containerSize = 16;
    containerLength = (float) leafRadius * 2 / (float) containerSize;
    while (containerLength < nodeRadius * 2) // just in case the container sizes are larger than the nodes
    {
        containerLength * 2;
        containerSize / 2;
    }
    
    // using containers for attractors here
    containers.resize(containerSize);
    for (int i = 0; i < containers.size(); i++) containers[i].resize(containerSize);
    vector <vector <int>> attractorContainer;
    attractorContainer.clear();
    
    // setting attraction points
    int counter = 0;
    while (attractors.size() < _noOfAttractors && counter < _noOfAttractors * 2)
    {
        // create new attractor
        float distance = ofRandom(leafRadius * 0.2, leafRadius - nodeRadius);
        float angle = ofRandom(ofDegToRad(360));
        float x = cos(angle) * distance;
        float y = sin(angle) * distance;
        
        // checking position of new attractor
        int nodeX = floor(ofMap(x, -leafRadius, leafRadius, 0, containerSize));
        int nodeY = floor(ofMap(y, -leafRadius, leafRadius, 0, containerSize));

        bool addToAttractors = true;
        for (int i = nodeX - 1; i <= nodeX + 1; i++)
            if (addToAttractors)
                for (int j = nodeY - 1; j <= nodeY + 1; j++)
                    if (addToAttractors)
                        if (i >= 0 && i < containers.size() && j >= 0 && j < containers[i].size())
                            if (containers[i][j].size() > 0)
                                for (int k = 0; k < containers[i][j].size(); k++)
                                    if (addToAttractors)
                                    {
                                        ofVec2f attractor = attractors[containers[i][j][k]];
                                        float dist = ofDist(x, y, attractor.x, attractor.y);
                                        if (dist < nodeRadius * 2) addToAttractors = false;
                                    }
        // adding new attractor
        if (addToAttractors)
        {
            containers[nodeX][nodeY].push_back(attractors.size());
            attractors.push_back(ofVec2f(x, y));
            attractorContainer.push_back(vector <int> {nodeX, nodeY});
        }
        counter++;
    }

    // setting up containers
    containers.clear();
    containers.resize(containerSize);
    for (int i = 0; i < containers.size(); i++) containers[i].resize(containerSize);
    
    // putting initial nodes into containers
    for (int i = 0; i <nodes.size(); i++)
    {
        int nodeX = floor(ofMap(nodes[i].x, -leafRadius, leafRadius, 0, containerSize));
        int nodeY = floor(ofMap(nodes[i].y, -leafRadius, leafRadius, 0, containerSize));
        containers[nodeX][nodeY].push_back(i);
        nodeContainer.push_back(vector <int> {nodeX, nodeY});
    }
    
    attractorsOriginal = attractors;
}

void Venation2DClosed::update()
{
    if (!initial) attractorCheck();
    generateRng();
    generateNewNodes();
    if (attractors.empty() && optimizeNodes)
    {
        removeCloseNodes();
        finalRng();
        optimizeNodes = false;
    }
    initial = false;
}

void Venation2DClosed::draw()
{
    // draw containers
    ofSetColor(50);
    for (int i = 0; i <= containerSize; i++)
        for (int j = 0; j <= containerSize; j++)
        {
            float x = ofMap(i, 0, containerSize, -leafRadius, leafRadius);
            float y = ofMap(j, 0, containerSize, -leafRadius, leafRadius);
            ofDrawLine(x, -leafRadius, x, leafRadius);
            ofDrawLine(-leafRadius, y, leafRadius, y);
        }
    
    // draw leaf
    ofNoFill();
    ofSetColor(200);
    ofDrawCircle(0, 0, leafRadius);
    
    // draw attractors copy
//    ofPushStyle();
//    ofFill();
//    ofSetColor(80, 0, 0);
//    for (int i = 0; i < attractorsOriginal.size(); i++) ofDrawCircle(attractorsOriginal[i].x, attractorsOriginal[i].y, nodeRadius);
//    ofPopStyle();
    
    // draw attractors
    ofSetColor(255, 150, 150);
    for (int i = 0; i < attractors.size(); i++) ofDrawCircle(attractors[i].x, attractors[i].y, nodeRadius);
    
    // draw nodes in containers based on RGB (R for X, G for Y, B for Z)
    ofFill();
    for (int i = 0; i < containers.size(); i++)
        for (int j = 0; j < containers[i].size(); j++)
            if (!containers[i][j].empty())
                for (int k = 0; k < containers[i][j].size(); k++)
                {
                    ofSetColor((255 / containers.size()) * i, (255 / containers[i].size()) * j, 0, 150);
                    int index = containers[i][j][k];
                    ofDrawCircle(nodes[index].x, nodes[index].y, nodeRadius);
                }
    
    // draw optimized nodes
    ofFill();
    ofSetColor(0, 0, 255, 50);
    for (int i = 0; i < nodesOptimized.size(); i++) ofDrawCircle(nodesOptimized[i].x, nodesOptimized[i].y, nodeRadius);
    
    // draw deleted nodes
    ofSetColor(255, 0, 0, 50);
    for (int i = 0; i < nodesDeleted.size(); i++) ofDrawCircle(nodesDeleted[i].x, nodesDeleted[i].y, nodeRadius);

    // draw nodes
    ofNoFill();
    ofSetColor(255);
    for (int i = 0; i < nodes.size(); i++) ofDrawCircle(nodes[i].x, nodes[i].y, nodeRadius);
    
    // draw branches
    ofSetColor(255, 255, 0);
    ofPushStyle();
    for (int i = 0; i < branches.size(); i++)
        branches[i].draw(nodesOptimized, 8);
    ofPopStyle();
    
    // draw tree lines
    ofSetColor(0, 255, 255);
    ofPushStyle();
    ofSetLineWidth(4);
    for (int i = 0; i < lines.size(); i++)
    {
        if (i < newLinesIndex) ofSetColor(0, 255, 255);
        else ofSetColor(255, 150, 0);
        ofVec2f a = nodes[lines[i][0]];
        ofVec2f b = nodes[lines[i][1]];
        ofDrawLine(a.x, a.y, b.x, b.y);
    }
    ofPopStyle();
    
    // draw RNG
    ofSetLineWidth(2);
    ofSetColor(0, 100, 255);
    ofVec2f v, w;
    int nSize = tempNodesSize; //because the nodes vector has grown since last RNG

    for (int i = 0; i < rngSize; i++)
        for (int j = 0; j < i; j++)
            if (rng[i][j])
            {
                if (i < nSize && j < nSize)
                {
                    v = nodes[i];
                    w = nodes[j];
                }
                else if (i >= nSize && j < nSize)
                {
                    v = attractors[i - nSize];
                    w = nodes[j];
                }
                else if (i < nSize && j >= nSize)
                {
                    v = nodes[i];
                    w = attractors[j - nSize];
                }
                else if (i >= nSize && j >= nSize)
                {
                    v = attractors[i - nSize];
                    w = attractors[j - nSize];
                }
                ofDrawLine(v.x, v.y, w.x, w.y);
            }
    
    //draw RNG growth lines
    ofSetLineWidth(1);
    ofSetColor(0, 150, 0);
    if (nodeNeighbors.size() > 0)
        for (int i = 0; i < nodeNeighbors.size(); i++)
            if (nodeNeighbors[i].size() > 0)
                for (int j = 0; j < nodeNeighbors[i].size(); j++)
                    ofDrawLine(nodes[i].x, nodes[i].y, attractors[nodeNeighbors[i][j]].x, attractors[nodeNeighbors[i][j]].y);
    
    // draw optimized RNG
    ofSetColor(255, 0, 0);
    if (rngOptimized.size() > 0)
        for (int i = 0; i < rngOptimized.size(); i++)
            for (int j = 0; j < i; j++)
                if (rngOptimized[i][j])
                {
                    v = nodesOptimized[i];
                    w = nodesOptimized[j];
                    ofDrawLine(v.x, v.y, w.x, w.y);
                }
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
        if (deleteAttractor) attractors.erase(attractors.begin() + i);
    }
}

void Venation2DClosed::generateRng()
{
    rngSize = nodes.size() + attractors.size();
    int nSize = nodes.size();
    float pairDist[rngSize][rngSize];
    
    rng.clear();
    rng.resize(rngSize);
    
    // 1. getting pair distances
    for (int i = 0; i < rngSize; i++)
        for (int j = 0; j <= i; j++)
        {
            if (i == j) pairDist[i][j] = 0;
            else if (i < nodes.size() && j < nodes.size())
            {
                float d = ofDist(nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y);
                pairDist[i][j] = d;
                pairDist[j][i] = d;
            }
            else if (i >= nodes.size() && j < nodes.size())
            {
                float d = ofDist(attractors[i - nSize].x, attractors[i - nSize].y, nodes[j].x, nodes[j].y);
                pairDist[i][j] = d;
                pairDist[j][i] = d;
            }
            else if (i < nodes.size() && j >= nodes.size())
            {
                float d = ofDist(nodes[i].x, nodes[i].y, attractors[j - nSize].x, attractors[j - nSize].y);
                pairDist[i][j] = d;
                pairDist[j][i] = d;
            }
            else if (i >= nodes.size() && j >= nodes.size())
            {
                float d = ofDist(attractors[i - nSize].x, attractors[i - nSize].y, attractors[j - nSize].x, attractors[j - nSize].y);
                pairDist[i][j] = d;
                pairDist[j][i] = d;
            }
        }
    
    // 2. getting max distances for each pair
    for (int i = 0; i < rngSize; i++)
        for (int j = 0; j < rngSize; j++)
            if (i != j)
            {
                bool relativeNeighborhood = true;
                float ijDist = pairDist[i][j];
                for (int k = 0; k < rngSize; k++)
                    if (i != k && j != k)
                    {
                        float maxDist = max(pairDist[i][k], pairDist[j][k]);
                        // 3. checking if there is a distance smaller than the pair distance
                        if (maxDist < ijDist)
                        {
                            relativeNeighborhood = false;
                            break;
                        }
                }
                rng[i].push_back((relativeNeighborhood) ? true : false);
            }
            else rng[i].push_back(false);
    
    // 4. calculating the relative neighbors for each node (of type attractor)
    nodeNeighbors.clear();
    nodeNeighbors.resize(nodes.size());
    for (int i = 0; i < nodes.size(); i++)
        for (int j = nodes.size(); j < rngSize; j++)
            if (rng[i][j]) nodeNeighbors[i].push_back((int) (j - nodes.size())); // beacuse we need the index of the attractor!

    // 5. calculating the relative neighbors for each attractor (of type node)
    attractorNeighbors.clear();
    attractorNeighbors.resize(attractors.size());
    for (int i = nodes.size(); i < rngSize; i++)
        for (int j = 0; j < nodes.size(); j++)
            if (rng[i][j]) attractorNeighbors[i - nodes.size()].push_back(j); // beacuse we need the index of the attractor!
    
    // 6. calculating the relative neighbors for each node (of type node)
    nodeNodeNeighbors.clear();
    nodeNodeNeighbors.resize(nodes.size());
    for (int i = 0; i < nodes.size(); i++)
        for (int j = 0; j < rngSize; j++)
            if (rng[i][j]) nodeNodeNeighbors[i].push_back(j); // beacuse we need the index of the attractor!
}

void Venation2DClosed::generateNewNodes()
{
    tempNodesSize = nodes.size();
    vector <ofVec2f> newNodes;
    vector <vector <int>> newLines;
    ofVec2f newNode;
    ofVec2f attractorNode;
    vector <int> newLine;
    for (int i = 0 ; i < nodes.size(); i++)
    {
        newNode.set(0, 0);
        int nodeAttractorSize = nodeNeighbors[i].size();
        if (nodeAttractorSize > 0)
        {
            newLine.push_back(i);
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
            
            if (!(find(nodes.begin(), nodes.end(), newNode) != nodes.end()))
                if (!(find(newNodes.begin(), newNodes.end(), newNode) != newNodes.end()))
                {
                    newNodes.push_back(newNode);
                    newLine.push_back(nodes.size() + newNodes.size() - 1);
                    newLines.push_back(newLine);
                }
            newLine.clear();
        }
    }
    
    newLinesIndex = nodes.size();
    for (int i = 0; i < newLines.size(); i++) lines.push_back(newLines[i]);
    
    //putting nodes in containers
    newNodesIndex = nodes.size();
    for (int i = 0; i < newNodes.size(); i++)
    {
        int nodeX = floor(ofMap(newNodes[i].x, -leafRadius, leafRadius, 0, containerSize));
        int nodeY = floor(ofMap(newNodes[i].y, -leafRadius, leafRadius, 0, containerSize));
        containers[nodeX][nodeY].push_back(nodes.size());
        nodeContainer.push_back(vector <int> {nodeX, nodeY});
        nodes.push_back(newNodes[i]);
    }
}

void Venation2DClosed::removeCloseNodes()
{
    vector <int> nodesToDelete;
    for (int i = 0; i < nodes.size(); i++) nodesOptimized.push_back(nodes[i]);
    
    // optimizing nodes to remove close proximity nodes
    float proximityRatio = 0.8;
    for (int n = 0; n < nodesOptimized.size(); n++)
    {
        bool addToOptimizedNodes = true;
        int mei = nodeContainer[n][0];
        int mej = nodeContainer[n][1];
        for (int i = mei - 1; i <= mei + 1; i++)
            if (addToOptimizedNodes)
                for (int j = mej - 1; j <= mej + 1; j++)
                    if (addToOptimizedNodes)
                        if (i >= 0 && i < containers.size() && j >= 0 && j < containers[i].size())
                            if (containers[i][j].size() > 0)
                                for (int k = 0; k < containers[i][j].size(); k++)
                                    if (addToOptimizedNodes)
                                        if (containers[i][j][k] != n)
                                        {
                                            ofVec2f meNode = nodesOptimized[n];
                                            ofVec2f otherNode = nodesOptimized[containers[i][j][k]];
                                            float dist = ofDist(meNode.x, meNode.y, otherNode.x, otherNode.y);
                                            if (dist < nodeRadius * proximityRatio) addToOptimizedNodes = false;
                                        }
        
        if (!addToOptimizedNodes) nodesToDelete.push_back(n);
    }
    
    //getting rid of the extra nodes in the close proximity ones
    int removed = 0;
    for (int i = nodesToDelete.size() - 1; i >= 0 ; i--)
    {
        bool deleteNode = false;
        for (int j = i; j >= 0; j--)
            if (i != j && j < nodesToDelete.size() - removed)
                if (ofDist(nodesOptimized[nodesToDelete[i]].x, nodesOptimized[nodesToDelete[i]].y, nodesOptimized[nodesToDelete[j]].x, nodesOptimized[nodesToDelete[j]].y) < nodeRadius * proximityRatio)
                {
                    deleteNode = true;
                    removed++;
                    break;
                }
        if (deleteNode) nodesToDelete.erase(nodesToDelete.begin() + i);
    }
    
    // resetting the 3 main databases (1.nodes, 2.containers & 3.nodeContainer)
    
    // 1
    // need to sort this first so that removed nodes by the stacking effect aren't being removed at the end
    sort(nodesToDelete.begin(), nodesToDelete.begin() + nodesToDelete.size());
    for (int i = nodesToDelete.size() - 1; i >= 0; i--)
    {
        nodesOptimized.erase(nodesOptimized.begin() + nodesToDelete[i]);
        nodesDeleted.push_back(nodes[nodesToDelete[i]]);
    }

    // 2 & 3
    containersOptimized.resize(containerSize);
    for (int i = 0; i < containersOptimized.size(); i++) containersOptimized[i].resize(containerSize);
    
    for (int i = 0; i <nodesOptimized.size(); i++)
    {
        int nodeX = floor(ofMap(nodesOptimized[i].x, -leafRadius, leafRadius, 0, containerSize));
        int nodeY = floor(ofMap(nodesOptimized[i].y, -leafRadius, leafRadius, 0, containerSize));
        containersOptimized[nodeX][nodeY].push_back(i);
        nodeContainerOptimized.push_back(vector <int> {nodeX, nodeY});
    }
}

void Venation2DClosed::finalRng()
{
//    vector <vector <int>> linesOptimized;
//    vector <vector <bool>> rngOptimized;
    int size = nodesOptimized.size();
    float pairDist[size][size];
    rngOptimized.resize(size);
    
    // 1. getting pair distances
    for (int i = 0; i < size; i++)
        for (int j = 0; j <= i; j++)
        {
            if (i == j) pairDist[i][j] = 0;
            else
            {
                float d = ofDist(nodesOptimized[i].x, nodesOptimized[i].y, nodesOptimized[j].x, nodesOptimized[j].y);
                pairDist[i][j] = d;
                pairDist[j][i] = d;
            }
        }
    
    // 2. getting max distances for each pair
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (i != j)
            {
                bool relativeNeighborhood = true;
                float ijDist = pairDist[i][j];
                for (int k = 0; k < size; k++)
                    if (i != k && j != k)
                    {
                        float maxDist = max(pairDist[i][k], pairDist[j][k]);
                        // 3. checking if there is a distance smaller than the pair distance
                        if (maxDist < ijDist)
                        {
                            relativeNeighborhood = false;
                            break;
                        }
                    }
                rngOptimized[i].push_back((relativeNeighborhood) ? true : false);
            }
            else rngOptimized[i].push_back(false);
    
    containersLinesOptimized.resize(containerSize);
    for (int i = 0; i < containersLinesOptimized.size(); i++) containersLinesOptimized[i].resize(containerSize);
    
    if (rngOptimized.size() > 0)
        for (int i = 0; i < rngOptimized.size(); i++)
            for (int j = 0; j < i; j++)
                if (rngOptimized[i][j])
                {
                    // setting up the line containers
                    ofVec2f iNode = nodesOptimized[i];
                    ofVec2f jNode = nodesOptimized[j];
                    ofVec2f mNode = (iNode + jNode) / 2;
                    int nodeX = floor(ofMap(mNode.x, -leafRadius, leafRadius, 0, containerSize));
                    int nodeY = floor(ofMap(mNode.y, -leafRadius, leafRadius, 0, containerSize));
                    containersLinesOptimized[nodeX][nodeY].push_back(linesOptimized.size());
                    lineContainer.push_back(vector <int> {nodeX, nodeY});
                    
                    // creating new lines
                    vector<int> newLine;
                    newLine.push_back(i);
                    newLine.push_back(j);
                    linesOptimized.push_back(newLine);
                }
    
    // setting up the branch structure
    
    // initializing with the initial node
    vector <bool> branchedLines;
    branchedLines.clear();
    branchedLines.resize(linesOptimized.size(), false);
    
    int mei = nodeContainerOptimized[0][0];
    int mej = nodeContainerOptimized[0][1];
    for (int i = mei - 1; i <= mei + 1; i++)
        for (int j = mej - 1; j <= mej + 1; j++)
            if (i >= 0 && i < containersLinesOptimized.size() && j >= 0 && j < containersLinesOptimized[i].size())
                if (containersLinesOptimized[i][j].size() > 0)
                    for (int k = 0; k < containersLinesOptimized[i][j].size(); k++)
                    {
                        int index = containersLinesOptimized[i][j][k];
                        if (!branchedLines[index])
                        {
                            bool flip = false;
                            bool addToBranch = false;
                            if (linesOptimized[index][0] == 0)
                            {
                                addToBranch = true;
                            }
                            else if (linesOptimized[index][1] == 0)
                            {
                                addToBranch = true;
                                flip = true; // to check the line direction
                            }
                            
                            if (addToBranch)
                            {
                                Branch b;
                                b.addExtension(flip ? vector <int> {linesOptimized[index][1], linesOptimized[index][0]} : linesOptimized[index]);
                                branches.push_back(b);
                                currentBranches.push_back(b);
                                branchedLines[index] = true;
                            }
                        }
                    }
    
    // adding the rest of the branches
    
//    for (int n = 0; n < currentBranches.size(); n++)
//    int mei = nodeContainerOptimized[0][0];
//    int mej = nodeContainerOptimized[0][1];
//    for (int i = mei - 1; i <= mei + 1; i++)
//        for (int j = mej - 1; j <= mej + 1; j++)
//            if (i >= 0 && i < containersLinesOptimized.size() && j >= 0 && j < containersLinesOptimized[i].size())
//                if (containersLinesOptimized[i][j].size() > 0)
//                    for (int k = 0; k < containersLinesOptimized[i][j].size(); k++)
//                    {
//                        int index = containersLinesOptimized[i][j][k];
//                        if (!branchedLines[index])
//                        {
//                            bool flip = false;
//                            bool addToBranch = false;
//                            if (linesOptimized[index][0] == 0)
//                            {
//                                addToBranch = true;
//                            }
//                            else if (linesOptimized[index][1] == 0)
//                            {
//                                addToBranch = true;
//                                flip = true; // to check the line direction
//                            }
//
//                            if (addToBranch)
//                            {
//                                Branch b;
//                                b.addExtension(flip ? vector <int> {linesOptimized[index][1], linesOptimized[index][0]} : linesOptimized[index]);
//                                branches.push_back(b);
//                                currentBranches.push_back(b);
//                                branchedLines[index] = true;
//                            }
//                        }
//                    }

}
