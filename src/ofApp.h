#pragma once

#include "ofxAssimpModelLoader.h"
#include "ofxSTL.h"
#include "ofMain.h"
#include "Venation2DOpen.h"
#include "Venation2DClosed.h"
#include "Venation3DOpen.h"
#include "Venation3DClosed.h"
#include "BranchMesh.h"
#include "Branch.h"
#include "ofxRayTriangleIntersection.h"

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
		
    ofEasyCam cam;
    ofLight dirLight1, dirLight2, dirLight3, dirLight4, dirLight5, dirLight6;

//    Venation2DOpen v;
    Venation2DClosed v;
//    Venation3DOpen v;
//    Venation3DClosed v;
    
    bool run;
    ofImage screen;
    
    BranchMesh m;
    
    ofxAssimpModelLoader model;
    ofPoint p, min, max, cen;

    ofxSTLImporter stl;
    ofxSTLExporter stlExp;
    ofBoxPrimitive boundingBox;
    
    ofMesh modelMesh;
};
