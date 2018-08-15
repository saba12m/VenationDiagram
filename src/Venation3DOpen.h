#pragma once

#include "ofMain.h"

#include "ofxCsv.h"

class Venation3DOpen {
    
public:
    void setup(int _leafRadius, int _nodeRadius, int _noOfAttractors);
    void update();
    void draw();
    
private:
    void attractorCheck();
    void generateProximity();
    void generateNewNodes();
    void calculateThickness();
    void saveFile();
    
    int leafRadius;
    int nodeRadius;
    int newNodesIndex;
    
    bool finalize;
    
    vector <ofVec3f> attractors;
    vector <ofVec3f> nodes;
    
    vector <bool> hasChildren;
    
    vector <float> nodeThickness;
    
    vector <int> nodeParents;
    vector <vector <int>> lines;
    vector <vector <int>> nodeClosestPoints;
};
