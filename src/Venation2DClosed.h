#pragma once

#include "ofMain.h"

class Venation2DClosed {
    
public:
    void setup(int _leafRadius, int _nodeRadius, int _noOfAttractors);
    void update();
    void draw();
    
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
    float containerLength;
    
    bool initial;
    bool finalize;

    vector <bool> hasChildren;
    
    vector <ofVec2f> attractors;
    vector <ofVec2f> nodes;
    
    vector <int> attractorIndices;
    vector <int> nodeParents;
    vector <int> nodeThickness;
    vector <int> passedNodes;
    vector <vector <int>> lines;
    vector <vector <int>> finalLines;
    vector <vector <int>> nodeContainer;
    vector <vector <int>> attractorContainer;
    vector <vector <int>> lineContainer;
    vector <vector <int>> nodeNeighbors;
    vector <vector <int>> attractorNeighbors;
    vector <vector <int>> passedContainers;
    vector <vector <vector <int>>> containers;
    vector <vector <vector <int>>> containersAttractors;
};
