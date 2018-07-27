#include "Branch.h"

void Branch::addExtension(vector <int> v)
{
    branches.push_back(v);
}

void Branch::addChildren(vector <vector <int>> listOfChildren)
{
    for (int i = 0; i < children.size(); i++)
    {
        Branch child;
        child.addExtension(listOfChildren[0]);
        children.push_back(child);
    }
}

void Branch::draw(vector<ofVec2f> nodes, float width)
{
    ofSetLineWidth(width);
    for (int i = 0; i < branches.size(); i++)
    {
        ofVec2f a = nodes[branches[i][0]];
        ofVec2f b = nodes[branches[i][1]];
        ofDrawLine(a.x, a.y, b.x, b.y);
    }
    
    if (children.size() > 0)
        for (int i = 0; i < children.size(); i++)
            children[i].draw(nodes, width * 0.75);
}
