#include "Venation3DOpen.h"

void Venation3DOpen::setup(vector <ofVec3f> _attractors, vector <ofVec3f> _originNodes, int _nodeRadius)
{
    hasChildren.clear();
    
    attractors.clear();
    nodes.clear();
    
    nodeThickness.clear();
    
    nodeParents.clear();
    lines.clear();
    nodeClosestPoints.clear();
    
    nodeRadius = _nodeRadius;
    
    finalize = true;
    
    // setting starting Point
    for (int i = 0; i < _originNodes.size(); i++)
    {
        nodes.push_back(_originNodes[i]);
        nodeParents.push_back(-1);
    }
    
    // setting attraction points
    for (int i = 0; i < _attractors.size(); i++)
        attractors.push_back(_attractors[i]);
}

void Venation3DOpen::setup(int _leafRadius, int _nodeRadius, int _noOfAttractors)
{
    hasChildren.clear();
    
    attractors.clear();
    nodes.clear();
    
    nodeParents.clear();
    lines.clear();
    nodeClosestPoints.clear();
    
    leafRadius = _leafRadius;
    nodeRadius = _nodeRadius;
    
    finalize = true;
    
    // setting starting Point
    nodes.push_back(ofVec3f(0, 0, 0));
    nodes.push_back(ofVec3f(-(leafRadius - nodeRadius), 0, 0));
    nodes.push_back(ofVec3f(0, -(leafRadius - nodeRadius), 0));
    
    for (int i = 0; i < nodes.size(); i++)
        nodeParents.push_back(-1);
    
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
        
        bool addToAttractors = true;
        for (int i = 0; i <attractors.size(); i++)
            if (addToAttractors)
            {
                float dist = ofDist(x, y, z, attractors[i].x, attractors[i].y, attractors[i].z);
                if (dist < nodeRadius * 2) addToAttractors = false;
            }
        
        // adding new attractor
        if (addToAttractors) attractors.push_back(ofVec3f(x, y, z));
        
        counter++;
    }
    
    newNodesIndex = 0;
}

void Venation3DOpen::update()
{
    attractorCheck();
    generateProximity();
    generateNewNodes();
    
    if (attractors.empty() && finalize)
    {
        calculateThickness();
//        saveFile();
        finalize = false;
    }
}

void Venation3DOpen::draw()
{
    // draw leaf
    if (leafRadius > 0)
    {
        ofNoFill();
        ofSetColor(200, 100);
        ofDrawSphere(0, 0, leafRadius);
    }
    
    // draw tree lines
    ofPushStyle();
    ofSetLineWidth(5);
    ofSetColor(0, 200, 200);
    for (int i = 0; i < lines.size(); i++)
        ofDrawLine(nodes[lines[i][0]].x, nodes[lines[i][0]].y, nodes[lines[i][0]].z, nodes[lines[i][1]].x, nodes[lines[i][1]].y, nodes[lines[i][1]].z);
    ofPopStyle();
    
    // draw attractor lines
    ofSetColor(0, 100, 255);
    for (int i = 0; i < nodeClosestPoints.size(); i++)
        for (int j = 0; j < nodeClosestPoints[i].size(); j++)
            ofDrawLine(nodes[i].x, nodes[i].y, nodes[i].z, attractors[nodeClosestPoints[i][j]].x, attractors[nodeClosestPoints[i][j]].y, attractors[nodeClosestPoints[i][j]].z);
    
    // draw attraction points
    ofFill();
    ofSetColor(255, 150, 150);
    for (int i = 0; i < attractors.size(); i++) ofDrawSphere(attractors[i].x, attractors[i].y, attractors[i].z, nodeRadius);
    
    // draw nodes
    for (int i = 0; i < nodes.size(); i++)
    {
        float r = (nodeThickness.size() > 0) ? nodeThickness[i]: nodeRadius;
        ofFill();
        ofSetColor(0, 255, 255);
        ofDrawSphere(nodes[i].x, nodes[i].y, nodes[i].z, r);
        
        // drawing the branch tips
        ofNoFill();
        ofSetColor(255);
        if (hasChildren.size() > 0)
            if (!hasChildren[i])
                ofDrawSphere(nodes[i].x, nodes[i].y, nodes[i].z, r);
    }
}

bool Venation3DOpen::done()
{
    if (nodeThickness.size() > 0) return true;
    else return false;
}

vector <ofVec3f> Venation3DOpen::getNodes()
{
    return nodes;
}

vector <vector <int>> Venation3DOpen::getLines()
{
    return lines;
}

vector <int> Venation3DOpen::getParents()
{
    return nodeParents;
}

void Venation3DOpen::attractorCheck()
{
    float attractorDist = 0;
    for (int i = attractors.size() - 1; i >= 0 ; i--)
        for (int j = newNodesIndex; j < nodes.size(); j++)
        {
            attractorDist = ofDist(attractors[i].x, attractors[i].y, attractors[i].z, nodes[j].x, nodes[j].y, nodes[j].z);
            if (attractorDist < nodeRadius * 2)
            {
                attractors.erase(attractors.begin() + i);
                break;
            }
        }
}

void Venation3DOpen::generateProximity()
{
    nodeClosestPoints.clear();
    nodeClosestPoints.resize(nodes.size());
    float dist = 0;
    float tempDist = 0;
    int index = 0;
    for (int i = 0; i < attractors.size(); i++)
    {
        dist = FLT_MAX;
        index = 0;
        for (int j = 0; j < nodes.size(); j++)
        {
            tempDist = ofDist(attractors[i].x, attractors[i].y, attractors[i].z, nodes[j].x, nodes[j].y, nodes[j].z);
            if (tempDist < dist)
            {
                dist = tempDist;
                index = j;
            }
        }
        nodeClosestPoints[index].push_back(i);
    }
}

void Venation3DOpen::generateNewNodes()
{
    newNodesIndex = nodes.size();
    vector <ofVec3f> newNodes;
    ofVec3f newNode;
    ofVec3f attractorNode;
    vector <int> newLine;
    int n = nodes.size();
    for (int i = 0 ; i < n; i++)
    {
        newNode.set(0, 0, 0);
        int nodeAttractorSize = nodeClosestPoints[i].size();
        if (nodeAttractorSize > 0)
        {
            newLine.push_back(i);
            for (int j = 0; j < nodeAttractorSize; j++)
            {
                attractorNode = attractors[nodeClosestPoints[i][j]] - nodes[i];
                attractorNode.normalize();
                newNode += attractorNode;
            }
            if (newNode.length() < nodeRadius * 0.5) newNode = attractorNode;
            newNode.normalize();
            newNode *= nodeRadius * 2;
            newNode += nodes[i];
            
            newLine.push_back(nodes.size());
            lines.push_back(newLine);
            nodes.push_back(newNode);
            nodeParents.push_back(i);
            newLine.clear();
        }
    }
}

void Venation3DOpen::calculateThickness()
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
            while (index > -1)
            {
                nodeThickness[index]++;
                index = nodeParents[index];
            }
        }
    
    for (int i = 0; i < nodes.size(); i++)
        nodeThickness[i] = pow(nodeThickness[i], 1.0 / 2.4);
}

void Venation3DOpen::saveFile()
{
    ofxCsv csvRecorder;
    csvRecorder.clear();
    for (int i = 0; i < lines.size(); i++)
    {
        ofxCsvRow row;
        
        // index
        row.setInt(0, i);
        
        // start point
        int startIndex = lines[i][0];
        float startX = nodes[startIndex].x;
        float startY = nodes[startIndex].y;
        float startZ = nodes[startIndex].z;
        
        row.setFloat(1, startX);
        row.setFloat(2, startY);
        row.setFloat(3, startZ);
        
        // end point
        int endIndex = lines[i][1];
        float endX = nodes[endIndex].x;
        float endY = nodes[endIndex].y;
        float endZ = nodes[endIndex].z;
        
        row.setFloat(4, endX);
        row.setFloat(5, endY);
        row.setFloat(6, endZ);
        
        // thicknesses
        int startThickness = nodeThickness[startIndex];
        int endThickness = nodeThickness[endIndex];
        
        row.setInt(7, startThickness);
        row.setInt(8, endThickness);
        
        csvRecorder.addRow(row);
    }
    string d = "VenationOpen_";
    d.append(ofToString(ofGetFrameNum()));
    d.append(".csv");
    csvRecorder.save(d);
}
