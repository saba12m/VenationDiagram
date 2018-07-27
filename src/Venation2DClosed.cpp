#include "Venation2DClosed.h"

void Venation2DClosed::setup(int _leafRadius, int _nodeRadius, int _noOfAttractors)
{
    attractors.clear();
    attractorsOriginal.clear();
    nodes.clear();
    lines.clear();
    rng.clear();
    strayLinesNodesIndex.clear();
    branches.clear();
    currentBranches.clear();
    containers.clear();
    leafRadius = _leafRadius;
    nodeRadius = _nodeRadius;
    initial = true;
    
    // setting starting Point
    nodes.push_back(ofVec2f(0, 0));
//    nodes.push_back(ofVec2f(0, -(leafRadius - nodeRadius)));
    
    // setting attraction points
    for (int i = 0; i < _noOfAttractors; i++)
    {
         float distance = ofRandom(leafRadius * 0.0, leafRadius - nodeRadius);
        float angle = ofRandom(ofDegToRad(360));
        float x = cos(angle) * distance;
        float y = sin(angle) * distance;
        attractors.push_back(ofVec2f(x, y));
    }
    
    // checking attraction point overlaps
    int removed = 0;
    int size = attractors.size();
    for (int i = size - 1; i >= 0; i--)
        for (int j = size - 1; j >= 0; j--)
            if (i != j && j < size - removed)
                if (ofDist(attractors[i].x, attractors[i].y,
                           attractors[j].x, attractors[j].y) < nodeRadius * 2)
                {
                    attractors.erase(attractors.begin() + j);
                    removed++;
                }
    
    // refilling till attractor numbers are met
//    size = attractors.size();
//    while (size < _noOfAttractors - 1)
//    {
//        float distance = ofRandom(leafRadius * 0.0, leafRadius - nodeRadius);
//        float angle = ofRandom(ofDegToRad(360));
//        float x = cos(angle) * distance;
//        float y = sin(angle) * distance;
//        ofVec2f v = ofVec2f(x, y);
//        bool add = true;
//        
//        for (int j = size - 1; j >= 0; j--)
//            if (ofDist(v.x, v.y, attractors[j].x, attractors[j].y) < nodeRadius * 2)
//            {
//                add = false;
//                break;
//            }
//        
//        if (add) attractors.push_back(v);
//    }
    
//    while (size < _noOfAttractors - 1)
//    {
//        for (int i = 0; i < _noOfAttractors - size; i++)
//        {
//            float distance = ofRandom(leafRadius * 0.0, leafRadius - nodeRadius);
//            float angle = ofRandom(ofDegToRad(360));
//            float x = cos(angle) * distance;
//            float y = sin(angle) * distance;
//            attractors.push_back(ofVec2f(x, y));
//        }
//
//        for (int i = size; i < _noOfAttractors; i++)
//            for (int j = _noOfAttractors - 1; j >= 0; j--)
//                if (i != j && j < size - removed)
//                    if (ofDist(attractors[i].x, attractors[i].y,
//                               attractors[j].x, attractors[j].y) < nodeRadius * 2)
//                    {
//                        attractors.erase(attractors.begin() + j);
//                        removed++;
//                    }
//    }
    
    attractorsOriginal = attractors;
}

void Venation2DClosed::update()
{
    if (!initial) attractorCheck();
    generateRng();
//    if (!initial) newNodesPositionCheck();
    generateNewNodes();
//    fixStrayLines();
    //    if (attractors.empty()) fixStrayLines();
    initial = false;
}

void Venation2DClosed::draw()
{
    ofNoFill();
    
    // draw leaf
    ofSetColor(200);
    ofDrawCircle(0, 0, leafRadius);
    
    // draw attractors copy
    ofPushStyle();
    ofFill();
    ofSetColor(100, 0, 0);
    for (int i = 0; i < attractorsOriginal.size(); i++) ofDrawCircle(attractorsOriginal[i].x, attractorsOriginal[i].y, nodeRadius);
    ofPopStyle();
    
    // draw attractors
    ofSetColor(255, 150, 150);
    for (int i = 0; i < attractors.size(); i++) ofDrawCircle(attractors[i].x, attractors[i].y, nodeRadius);
    
    // draw nodes
    ofSetColor(255);
    for (int i = 0; i < nodes.size(); i++) ofDrawCircle(nodes[i].x, nodes[i].y, nodeRadius);
    
    // draw branches
    ofSetColor(255, 255, 0);
    ofPushStyle();
    for (int i = 0; i < branches.size(); i++)
        branches[i].draw(nodes, 8);
    ofPopStyle();
    
    // draw tree lines
    ofSetColor(0, 255, 255);
    ofPushStyle();
    ofSetLineWidth(5);
    for (int i = 0; i < lines.size(); i++)
    {
        if (i < newLinesIndex) ofSetColor(0, 255, 255, 100);
        else ofSetColor(255, 150, 0, 100);
        ofVec2f a = nodes[lines[i][0]];
        ofVec2f b = nodes[lines[i][1]];
        ofDrawLine(a.x, a.y, b.x, b.y);
    }
    ofPopStyle();
    cout << lines.size() << endl;
    
    // draw RNG
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
    ofSetColor(0, 150, 0);
    if (nodeNeighbors.size() > 0)
        for (int i = 0; i < nodeNeighbors.size(); i++)
            if (nodeNeighbors[i].size() > 0)
                for (int j = 0; j < nodeNeighbors[i].size(); j++)
                    ofDrawLine(nodes[i].x, nodes[i].y, attractors[nodeNeighbors[i][j]].x, attractors[nodeNeighbors[i][j]].y);
}

void Venation2DClosed::attractorCheck()
{
    if (!initial)
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

void Venation2DClosed::newNodesPositionCheck()
{
    for (int i = nodes.size() - 1; i >= newNodesIndex; i--)
    {
        float dist = nodeRadius * 2;
        int index = 0;
        for (int j = 0; j < nodeNodeNeighbors[i].size(); j++)
        {
            int neighbor = nodeNodeNeighbors[i][j];
//            if (neighbor < newNodesIndex)
            {
                float tempDist = ofDist(nodes[i].x, nodes[i].y, nodes[neighbor].x, nodes[neighbor].y);
                if (tempDist < dist)
                {
                    dist = tempDist;
                    index = neighbor;
                }
            }
        }
        if (dist < nodeRadius * 2) nodes[i].set(ofVec2f(nodes[index].x, nodes[index].y));
    }
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
            newNodes.push_back(newNode);
            newLine.push_back(nodes.size() + newNodes.size() - 1);
            newLines.push_back(newLine);
            newLine.clear();
        }
    }
    
    newLinesIndex = nodes.size();
    for (int i = 0; i < newLines.size(); i++) lines.push_back(newLines[i]);
    
    // setting up the branch structure
    if (nodes.size() < 12)
        for (int i = 0; i < lines.size(); i++)
            if (lines[i][0] == 0)
            {
                Branch b;
                b.addExtension(lines[i]);
                branches.push_back(b);
                currentBranches.push_back(b);
            }
    
    newNodesIndex = nodes.size();
    for (int i = 0; i < newNodes.size(); i++)
    {
//        newNodes[i];
        nodes.push_back(newNodes[i]);
    }
}

void Venation2DClosed::fixStrayLines()
{
    for (int i = 0; i < nodeNeighbors.size(); i++)
        if (!(find(strayLinesNodesIndex.begin(), strayLinesNodesIndex.end(), i) != strayLinesNodesIndex.end())) // if strayLinesNodesIndex doesn't contain i
            if (nodeNeighbors[i].size() == 0)
            {
                bool addTostrayLinesNodesIndex = false;
                for (int j = 0; j < nodeNodeNeighbors[i].size(); j++)
                {
                    float dist = ofDist(nodes[i].x, nodes[i].y, nodes[nodeNodeNeighbors[i][j]].x, nodes[nodeNodeNeighbors[i][j]].y);
//                    if (dist != nodeRadius * 2)
                    if (dist < nodeRadius * 2 || dist > nodeRadius * 2)
                    if (!(find(strayLines.begin(), strayLines.end(), vector <int> {i, nodeNodeNeighbors[i][j]}) != strayLines.end()))
                            if (!(find(strayLines.begin(), strayLines.end(), vector <int> {nodeNodeNeighbors[i][j], i}) != strayLines.end()))
                            {
                                vector <int> newLine;
                                newLine.push_back(i);
                                newLine.push_back(nodeNodeNeighbors[i][j]);
                                lines.push_back(newLine);
                                addTostrayLinesNodesIndex = true;
                                strayLines.push_back(newLine);
                            }
                }
//                if (addTostrayLinesNodesIndex) strayLinesNodesIndex.push_back(i);
            }
}

