#pragma once

#include "ofMain.h"

#include "ofxCsv.h"

class Venation3DClosed {
    
public:
    void setup(int _leafRadius, int _nodeRadius, int _noOfAttractors);
    void update();
    void draw();
    void saveFile();
    
private:
    void attractorCheck();
    void nodePassedCheck();
    void generateRng();
    void generateNewNodes();
    void calculateThickness();
    void finalRngStructure();
    
    int progressCounter;
    int leafRadius;
    int nodeRadius;
    int rngSize;
    int tempNodesSize;
    int newLinesIndex;
    int containerNum;
    
    float containerLength; // will have to change these based on point filling
    float containerDist;
    
    bool initial;
    bool finalize;
    
    vector <ofVec3f> attractors;
    vector <ofVec3f> nodes;
    
    vector <bool> hasChildren;
    
    vector <float> nodeThickness;
    
    vector <int> attractorIndices;
    vector <int> nodeParents;
    vector <int> passedNodes;

    vector <vector <int>> nodeNeighbors;
    vector <vector <int>> attractorNeighbors;
    vector <vector <int>> passedContainers;
    vector <vector <int>> nodeContainer;
    vector <vector <int>> attractorContainer;
    vector <vector <int>> lines;
    vector <vector <int>> finalLines;
    vector <vector <vector <vector <int>>>> containers;
    vector <vector <vector <vector <int>>>> containersAttractors;
};
