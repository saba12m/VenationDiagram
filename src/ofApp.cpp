#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Venation Structure");
    ofBackground(0);
    ofSetCircleResolution(60);
    ofSetSphereResolution(8);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofSetVerticalSync(false);
    cam.setDistance(200);
    
    // set lights
    dirLight.setDiffuseColor(ofColor(255, 255, 255));
    dirLight.setDirectional();
    dirLight.setPosition(-100, -100, -100);
    dirLight.lookAt(ofVec3f(0, 0, 0));
    
    run = false;
    
    v.setup(100, 2, 480);
    vv.setup(480, 2, 0.3);
}

//--------------------------------------------------------------
void ofApp::update(){
    if (run) v.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    dirLight.enable();

    cam.begin();
    
    // venation
//    v.draw();
    vv.draw();
    
    // draw axes
    ofSetColor(255, 0, 0);
    ofDrawLine(0, 0, 0, 100, 0, 0);
    ofSetColor(0, 255, 0);
    ofDrawLine(0, 0, 0, 0, 100, 0);
    ofSetColor(0, 0, 255);
    ofDrawLine(0, 0, 0, 0, 0, 100);
    
//    m.draw();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') run = true;
    if (key == 'c') cam.reset();
    if (key == 'r')
    {
        v.setup(100, 2, 480);
        vv.setup(480, 2, 0.3);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == ' ') run = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
