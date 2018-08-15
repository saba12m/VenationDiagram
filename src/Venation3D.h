#pragma once

#include "ofMain.h"

#include "ofxAssimpModelLoader.h"
#include "ofxRayTriangleIntersection.h"

#include "Venation3DOpen.h"
#include "Venation3DClosed.h"

class Venation3D {
    
public:
    void setup(int _noOfAttractors, int _nodeRadius, float degreeOfOpenness);
    void update();
    void draw();
    
private:
    
    ofxAssimpModelLoader geometry;
    ofMesh geometryMesh;
    ofxAssimpModelLoader surface;
    ofMesh surfaceMesh;
    
    ofBoxPrimitive boundingBox; //
    vector <ofVec3f> openAttractors; //
    vector <ofVec3f> closedAttractors; //
    
    // raycasting
    ofxRayTriangleIntersection rtIntersect;
    vector <FaceTri> tris;
    vector <Ray> rays;
    
//    void attractorCheck();
//    void generateProximity();
//    void generateNewNodes();
//    void calculateThickness();
//    
//    int leafRadius;
//    int nodeRadius;
//    int newNodesIndex;
//    
//    bool finalize;
//    
//    vector <ofVec3f> attractors;
//    vector <ofVec3f> nodes;
//    
//    vector <bool> hasChildren;
//    
//    vector <float> nodeThickness;
//    
//    vector <int> nodeParents;
//    vector <vector <int>> lines;
//    vector <vector <int>> nodeClosestPoints;
};
