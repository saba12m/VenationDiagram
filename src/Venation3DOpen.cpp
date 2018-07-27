#include "Venation3DOpen.h"

void Venation3DOpen::setup(int _leafRadius, int _nodeRadius, int _noOfAttractors)
{
    nodeClosestPoints.clear();
    deletedAttractors.clear();
    attractors.clear();
    nodes.clear();
    lines.clear();
    leafRadius = _leafRadius;
    nodeRadius = _nodeRadius;
    
    // setting starting Point
    nodes.push_back(ofVec3f(0, 0, 0));
//    nodes.push_back(ofVec3f(0, -(leafRadius - nodeRadius), 0));
    
    float r = 0;
    float phi = 0;
    float theta = 0;
    
    float x = 0;
    float y = 0;
    float z = 0;
    
    // setting attraction points
    for (int i = 0; i < _noOfAttractors; i++)
    {
        r = ofRandom(leafRadius * 0.1, leafRadius - nodeRadius);
        phi = ofRandom(ofDegToRad(360));
        theta = ofRandom(ofDegToRad(180));
        x = r * sin(theta) * cos(phi);
        y = r * sin(theta) * sin(phi);
        z = r * cos(theta);
        attractors.push_back(ofVec3f(x, y, z));
    }
    
    // checking attraction point overlaps
    int removed = 0;
    int size = attractors.size();
    for (int i = size - 1; i >= 0; i--)
        for (int j = size - 1; j >= 0; j--)
            if (i != j && j < size - removed)
                if (ofDist(attractors[i].x, attractors[i].y, attractors[i].z,
                           attractors[j].x, attractors[j].y, attractors[j].z) < nodeRadius * 4)
                {
                    deletedAttractors.push_back(attractors[j]);
                    attractors.erase(attractors.begin() + j);
                    removed++;
                }
    
    tempNewNodes = nodes;
}

void Venation3DOpen::update()
{
    // checking which attractors have been reached and need to be removed
    float attractorDist = 0;
    for (int i = attractors.size() - 1; i >= 0 ; i--)
    {
        for (int j = 0; j < tempNewNodes.size(); j++)
        {
            attractorDist = ofDist(attractors[i].x, attractors[i].y, attractors[i].z,
                                   tempNewNodes[j].x, tempNewNodes[j].y, tempNewNodes[j].z);
            if (attractorDist < nodeRadius * 2)
            {
                attractors.erase(attractors.begin() + i);
                break;
            }
        }
    }
    
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
            tempDist = ofDist(attractors[i].x, attractors[i].y, attractors[i].z,
                              nodes[j].x, nodes[j].y, nodes[j].z);
            if (tempDist < dist)
            {
                dist = tempDist;
                index = j;
            }
        }
        nodeClosestPoints[index].push_back(i);
    }
    
    vector <ofVec3f> newNodes;
    ofVec3f newNode;
    ofVec3f attractorNode;
    vector <ofVec3f> newLine;
    for (int i = 0 ; i < nodes.size(); i++)
    {
        newNode.set(0, 0, 0);
        int nodeAttractorSize = nodeClosestPoints[i].size();
        if (nodeAttractorSize > 0)
        {
                        newLine.push_back(nodes[i]);
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
            newNodes.push_back(newNode);
            newLine.push_back(newNode);
            lines.push_back(newLine);
            newLine.clear();
        }
    }
    
    for (int i = 0; i < newNodes.size(); i++) nodes.push_back(newNodes[i]);
    
    tempNewNodes = newNodes;
}

void Venation3DOpen::draw()
{
    // draw leaf
    ofNoFill();
    ofSetColor(200, 100);
    ofDrawSphere(0, 0, leafRadius);
    
    // draw deleted attractors
    ofFill();
    ofSetColor(0, 255, 255);
    for (int i = 0; i < deletedAttractors.size(); i++)
        ofDrawSphere(deletedAttractors[i].x, deletedAttractors[i].y, deletedAttractors[i].z, nodeRadius);
    
    // draw tree lines
    ofSetColor(0, 200, 200);
    for (int i = 0; i < lines.size(); i++)
        ofDrawLine(lines[i][0].x, lines[i][0].y, lines[i][0].z, lines[i][1].x, lines[i][1].y, lines[i][1].z);
    
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
    ofSetColor(255);
    for (int i = 0; i < nodes.size(); i++) ofDrawSphere(nodes[i].x, nodes[i].y, nodes[i].z, nodeRadius);
    
}
