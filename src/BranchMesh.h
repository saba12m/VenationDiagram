#pragma once

#include "ofMain.h"

class BranchMesh {
public:
    void setup(ofVec3f sPoint, ofVec3f ePoint, int res, float _sRadius, float _eRadius);
    void draw();
private:
    void setupCylinder(int res, float sRadius, float eRadius, float z, ofMatrix4x4 m);
    void setupSphere(int res, float radius, ofMatrix4x4 m);
    ofMesh cylinderMesh;
    ofMesh sphereMesh;
};
