#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetCircleResolution(60);
//    ofSetSphereResolution(4);
    
//    ofEnableDepthTest();
    ofSetVerticalSync(true);
    cam.setDistance(400);
    
    // set lights
    dirLight1.setDiffuseColor(ofColor(255, 100, 100));
    dirLight1.setDirectional();
    dirLight2.setDiffuseColor(ofColor(100, 255, 100));
    dirLight2.setDirectional();
    dirLight3.setDiffuseColor(ofColor(100, 100, 255));
    dirLight3.setDirectional();
    dirLight4.setDiffuseColor(ofColor(100, 50, 50));
    dirLight4.setDirectional();
    dirLight5.setDiffuseColor(ofColor(50, 100, 50));
    dirLight5.setDirectional();
    dirLight6.setDiffuseColor(ofColor(50, 50, 100));
    dirLight6.setDirectional();
    
//    v.setup(360, 2, 2400);
    v.setup(100, 2, 480);
//    v.setup(50, 2, 20);
    run = false;
    
    // model test
    model.loadModel("Geometry Test 1.obj");
    model.calculateDimensions();
    p = model.getPosition();
    min = model.getSceneMin();
    max = model.getSceneMax();
    modelMesh = model.getMesh(0);
    model.getMeshHelper(0);
    
    float minX =   std::numeric_limits<float>::max();
    float maxX = - std::numeric_limits<float>::max();
    float minY =   std::numeric_limits<float>::max();
    float maxY = - std::numeric_limits<float>::max();
    float minZ =   std::numeric_limits<float>::max();
    float maxZ = - std::numeric_limits<float>::max();
    
    auto verts = modelMesh.getVertices();
    for (int i = 0; i < verts.size(); i++)
    {
        auto v = verts[i];
        if(v.x < minX) minX = v.x;
        if(v.x > maxX) maxX = v.x;
        if(v.y < minY) minY = v.y;
        if(v.y > maxY) maxY = v.y;
        if(v.z < minZ) minZ = v.z;
        if(v.z > maxZ) maxZ = v.z;
    }
    
    boundingBox.set(maxX - minX, maxY - minY, maxZ - minZ);
    boundingBox.setPosition((maxX + minX) / 2, (maxY + minY) / 2, (maxZ + minZ) / 2);
    // STL test
    stl.loadSTL(ofToDataPath("Geometry Test 1 Binary.stl"));
//    stl.rescaleModel(400);
    
    // mesh test
    m.setup(ofVec3f(10, 10, 0), ofVec3f(20, 30, 40), 6, 10, 20);
}

//--------------------------------------------------------------
void ofApp::update(){
    dirLight1.setPosition(-100, 0, 0);
    dirLight1.lookAt(ofVec3f(0, 0, 0));
    dirLight2.setPosition(0, -100, 0);
    dirLight2.lookAt(ofVec3f(0, 0, 0));
    dirLight3.setPosition(0, 0, -100);
    dirLight3.lookAt(ofVec3f(0, 0, 0));
    dirLight4.setPosition(100, 0, 0);
    dirLight4.lookAt(ofVec3f(0, 0, 0));
    dirLight5.setPosition(0, 100, 0);
    dirLight5.lookAt(ofVec3f(0, 0, 0));
    dirLight6.setPosition(0, 0, 100);
    dirLight6.lookAt(ofVec3f(0, 0, 0));
    if (run) v.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
//    dirLight1.enable();
//    dirLight2.enable();
//    dirLight3.enable();
//    dirLight4.enable();
//    dirLight5.enable();
//    dirLight6.enable();

    cam.begin();
    v.draw();
//    boundingBox.drawWireframe();
//    ofSetColor(255, 255, 255);
////    model.draw(OF_MESH_FILL);
//    model.draw(OF_MESH_WIREFRAME);
//    ofSetColor(255, 0, 0);
//    modelMesh.drawWireframe();
    
//    stl.drawFacets();
//    ofSetColor(0, 255, 0);
//    stl.drawWireFrame();
//    stl.drawNormals();
//    stl.shiftModelPosition(ofVec3f(1, 0, 0));
//    cen = stl.getModelCenter();
//
//    ofSetColor(255, 255, 255);
//    ofDrawSphere(cen.x, cen.y, cen.z, 2);
//    
//    ofSetColor(255, 0, 0);
//    ofDrawSphere(p.x, p.y, p.z, 2);
//    ofSetColor(0, 255, 0);
//    ofDrawSphere(min.x, min.y, min.z, 2);
//    ofSetColor(0, 0, 255);
//    ofDrawSphere(max.x, max.y, max.z, 2);

    
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
    if (key == 'r')
    {
//        v.setup(360, 2, 2400);
        v.setup(100, 2, 480);
//        v.setup(50, 2, 20);
        cam.reset();
    }
    if (key == 's')
    {
        screen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        screen.save("ScreenCapture_" + ofGetTimestampString() + ".jpg");
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
