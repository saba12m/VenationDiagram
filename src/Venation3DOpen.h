#pragma once

#include "ofMain.h"

#include "ofxCsv.h"

class Venation3DOpen {
    
public:
    void setup(vector <ofVec3f> _attractors, vector <ofVec3f> _originNodes, int _nodeRadius);
    void setup(int _leafRadius, int _nodeRadius, int _noOfAttractors);
    void update();
    void draw();
    void saveFile();
    bool done();
    vector <ofVec3f> getNodes();
    vector <vector <int>> getLines();
    vector <int> getParents();
    
private:
    void attractorCheck();
    void generateProximity();
    void generateNewNodes();
    void calculateThickness();
    
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
