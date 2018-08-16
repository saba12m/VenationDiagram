#pragma once

#include "ofMain.h"

#include "ofxAssimpModelLoader.h"
#include "ofxRayTriangleIntersection.h"

#include "Venation3DOpen.h"
#include "Venation3DClosed.h"

class Venation3D {
    
public:
    void setup(int _noOfAttractors, int _nodeRadius, float degreeOfOpenness, int _number);
    void update();
    void draw();
    
private:
    int number;
    int nodeRadius;
    bool save;
    bool switchGrowth;
    float minX, maxX, minY, maxY, minZ, maxZ;
    
    vector <ofVec3f> openAttractors;
    vector <ofVec3f> closedAttractors;
    
    Venation3DOpen vOpen;
    Venation3DClosed vClosed;
};
