#pragma once

#include "ofMain.h"
#include "Branch.h"

class Venation2DClosed {
    
public:
    void setup(int _leafRadius, int _nodeRadius, int _noOfAttractors);
    void update();
    void draw();
    
private:
    void attractorCheck();
    void generateRng();
    void newNodesPositionCheck(); //
    void generateNewNodes();
    void fixStrayLines();
    
    int leafRadius;
    int nodeRadius;
    int rngSize;
    int tempNodesSize;
    bool initial;
    int newLinesIndex;
    int newNodesIndex;
    vector <int> strayLinesNodesIndex;
    vector <vector <int>> strayLines;
    vector <ofVec2f> attractors;
    vector <ofVec2f> attractorsOriginal;
    vector <ofVec2f> nodes;
    vector <vector <int>> lines;
    vector <vector <int>> nodeNeighbors;
    vector <vector <int>> attractorNeighbors;
    vector <vector <int>> nodeNodeNeighbors;
    vector <vector <bool>> rng;
    vector <Branch> branches;
    vector <Branch> currentBranches;
    vector <vector <vector <int>>> containers;
};
