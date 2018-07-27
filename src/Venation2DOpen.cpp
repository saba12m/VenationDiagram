
#include "Venation2DOpen.h"

void Venation2DOpen::setup(int _leafRadius, int _nodeRadius, int _noOfAttractors)
{
    nodeClosestPoints.clear();
    attractors.clear();
    attractorsOriginal.clear();
    nodes.clear();
    lines.clear();
    leafRadius = _leafRadius;
    nodeRadius = _nodeRadius;
    
    // setting starting Point
    nodes.push_back(ofVec2f(0, 0));
    //    nodes.push_back(ofVec2f(0, -(leafRadius - nodeRadius), 0));
    
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
                if (ofDist(attractors[i].x, attractors[i].y, attractors[j].x, attractors[j].y) < nodeRadius * 4)
                {
                    attractors.erase(attractors.begin() + j);
                    removed++;
                }
    
    attractorsOriginal = attractors;
    tempNewNodes = nodes;
}

void Venation2DOpen::update()
{
    // checking which attractors have been reached and need to be removed
    float attractorDist = 0;
    for (int i = attractors.size() - 1; i >= 0 ; i--)
    {
        for (int j = 0; j < tempNewNodes.size(); j++)
        {
            attractorDist = ofDist(attractors[i].x, attractors[i].y, tempNewNodes[j].x, tempNewNodes[j].y);
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
            tempDist = ofDist(attractors[i].x, attractors[i].y, nodes[j].x, nodes[j].y);
            if (tempDist < dist)
            {
                dist = tempDist;
                index = j;
            }
        }
        nodeClosestPoints[index].push_back(i);
    }
    
    vector <ofVec2f> newNodes;
    ofVec2f newNode;
    ofVec2f attractorNode;
    vector <ofVec2f> newLine;
    for (int i = 0 ; i < nodes.size(); i++)
    {
        newNode.set(0, 0);
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

void Venation2DOpen::draw()
{
    // draw leaf
    ofNoFill();
    ofSetColor(200, 100);
    ofDrawCircle(0, 0, leafRadius);
    
    // draw attractor lines
    ofSetColor(0, 100, 255);
    for (int i = 0; i < nodeClosestPoints.size(); i++)
        for (int j = 0; j < nodeClosestPoints[i].size(); j++)
            ofDrawLine(nodes[i].x, nodes[i].y, attractors[nodeClosestPoints[i][j]].x, attractors[nodeClosestPoints[i][j]].y);
    
    // draw attractors copy
    ofSetColor(100, 0, 0);
    ofPushStyle();
    ofFill();
    for (int i = 0; i < attractorsOriginal.size(); i++) ofDrawCircle(attractorsOriginal[i].x, attractorsOriginal[i].y, nodeRadius);
    ofPopStyle();
    
    // draw tree lines
    ofSetColor(0, 200, 200);
    ofPushStyle();
    ofSetLineWidth(3);
    for (int i = 0; i < lines.size(); i++)
        ofDrawLine(lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y);
    ofPopStyle();
    
    // draw attraction points
    ofSetColor(255, 150, 150);
    for (int i = 0; i < attractors.size(); i++) ofDrawCircle(attractors[i].x, attractors[i].y, nodeRadius);
    
    // draw nodes
    ofSetColor(255);
    for (int i = 0; i < nodes.size(); i++) ofDrawCircle(nodes[i].x, nodes[i].y, nodeRadius);
}
