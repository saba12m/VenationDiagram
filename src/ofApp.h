#pragma once

#include "ofMain.h"

#include "Venation2DOpen.h"
#include "Venation2DClosed.h"
#include "Venation3D.h"
#include "Venation3DOpen.h"
#include "Venation3DClosed.h"
#include "BranchMesh.h"

//#include "ofxAssimpModelLoader.h"
//#include "ofxRayTriangleIntersection.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
		
private:
    ofEasyCam cam;
    ofLight dirLight;

//    Venation2DOpen v;
//    Venation2DClosed v;
    Venation3DOpen v;
//    Venation3DClosed v;
    
    Venation3D vv;
    
    bool run;
    
    BranchMesh m;
    
//    // mesh
//    ofxAssimpModelLoader geometry;
//    ofMesh geometryMesh;
//    ofxAssimpModelLoader surface;
//    ofMesh surfaceMesh;
//    ofBoxPrimitive boundingBox;
//    
//    vector <ofVec3f> points;
    
//    // raycasting
//    ofxRayTriangleIntersection rtIntersect;
//    vector <FaceTri> tris;
//    vector <Ray> rays;
//    vector <ofVec3f> pIn;
//    vector <ofVec3f> pOut;
};
