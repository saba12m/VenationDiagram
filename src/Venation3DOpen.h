#pragma once

#include "ofMain.h"

class Venation3DOpen {
    
public:
    void setup(int _leafRadius, int _nodeRadius, int _noOfAttractors);
    void update();
    void draw();
    
private:
    int leafRadius = 0;
    int nodeRadius = 0;
    vector <ofVec3f> attractors;
    vector <ofVec3f> nodes;
    vector <vector <ofVec3f>> lines;
    vector <vector <int>> nodeClosestPoints;
    vector <ofVec3f> tempNewNodes;
    vector <ofVec3f> deletedAttractors;
};
