#pragma once

#include "ofMain.h"

#include "ofxCsv.h"

class Venation3DClosed {
    
public:
    void setup(vector <ofVec3f> _attractors, vector <ofVec3f> _nodes, vector <vector <int>> _lines, vector <int> _nodeParents,
               int _nodeRadius, float _minX, float _maxX, float _minY, float _maxY, float _minZ, float _maxZ);
    void setup(int _leafRadius, int _nodeRadius, int _noOfAttractors);
    void update();
    void draw();
    void saveFile();
    void saveFile(int num);
    bool done();
    
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
    int initialNodesSize;
    int initialLinesSize;
    int containerNum;
    int containerNumX;
    int containerNumY;
    int containerNumZ;
    
    float containerLength;
    float containerDist;
    float minX, maxX, minY, maxY, minZ, maxZ;
    
    bool initial;
    bool finalize;
    
    vector <ofVec3f> attractors;
    vector <ofVec3f> nodes;
    
    vector <bool> hasChildren;
    
    vector <double> nodeThickness;
    
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
