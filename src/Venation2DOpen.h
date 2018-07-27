#pragma once

#include "ofMain.h"

class Venation2DOpen {
    
public:
    void setup(int _leafRadius, int _nodeRadius, int _noOfAttractors);
    void update();
    void draw();
    
private:
    int leafRadius = 0;
    int nodeRadius = 0;
    vector <ofVec2f> attractors;
    vector <ofVec2f> attractorsOriginal;
    vector <ofVec2f> nodes;
    vector <vector <ofVec2f>> lines;
    vector <vector <int>> nodeClosestPoints;
    vector <ofVec2f> tempNewNodes;
};
