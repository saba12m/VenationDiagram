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
    void nodeOptimization();
    
    int leafRadius;
    int nodeRadius;
    int rngSize;
    int tempNodesSize;
    int newLinesIndex;
    int newNodesIndex;
    int containerSize;
    float containerLength;
    
    bool initial;
    bool addStrayLines;
    bool optimizeNodes;
    

    vector <ofVec2f> attractors;
    vector <ofVec2f> attractorsOriginal;
    vector <ofVec2f> nodes;
    vector <ofVec2f> nodesOptimized;
    vector <ofVec2f> nodesDeleted;
    
    vector <int> strayLinesNodesIndex;
    vector <vector <int>> strayLines;
    vector <vector <int>> nodeContainer;
    vector <vector <int>> lines;
    vector <vector <int>> nodeNeighbors;
    vector <vector <int>> attractorNeighbors;
    vector <vector <int>> nodeNodeNeighbors;
    vector <vector <bool>> rng;
    vector <vector <vector <int>>> containers;
    
    vector <Branch> branches;
    vector <Branch> currentBranches;
};
