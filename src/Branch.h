#pragma once

#include "ofMain.h"

class Branch {
public:
    void addExtension(vector <int> v);
    void addChildren(vector <vector <int>> listOfChildren);
    void draw(vector<ofVec2f> nodes, float width);
    
    vector <vector <int>> branches;
    vector <Branch> children;
};
