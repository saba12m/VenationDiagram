#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetCircleResolution(60);
    ofSetSphereResolution(8);
    
    ofEnableDepthTest();
    ofEnableAlphaBlending();
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
    
//    v.setup(100, 2, 1200);
    v.setup(100, 2, 480);
//    v.setup(50, 2, 20);
    run = false;
    
    // model test
    model.loadModel("Geometry Test 1.obj");
    modelMesh = model.getMesh(0);
    
    // get bounding box
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
    
    // raycasting
    int faceNum = modelMesh.getUniqueFaces().size();
    
    // input mesh faces
    for (int i = 0; i < faceNum; i++)
         {
             auto face = modelMesh.getFace(i);

             FaceTri tri;
             auto v = face.getVertex(0);
             tri.v0 = ofPoint(v.x, v.y, v.z);
             v = face.getVertex(1);
             tri.v1 = ofPoint(v.x, v.y, v.z);
             v = face.getVertex(2);
             tri.v2 = ofPoint(v.x, v.y, v.z);
             tris.push_back(tri);
         }
    
    int num = 20;
    for (int i = 0; i < num; i ++)
    {
        rays.clear();
        float x = ofRandom(minX, maxX);
        float y = ofRandom(minY, maxY);
        float z = ofRandom(minZ, maxZ);
        ofVec3f dir = ofVec3f(ofRandom(1), ofRandom(1), ofRandom(1));
        dir.normalize();
        dir *= (maxX - minX) + (maxY - minY) + (maxZ - minZ);
        Ray ray;
        ray.rayOrig.set(x, y, z);
        ray.rayEnd.set(x + dir.x, y + dir.y, z + dir.z);
        rays.push_back(ray);
        auto results = rtIntersect.checkMeshIntersection(rays, tris);
        if (results.size() % 2 == 1) pIn.push_back(ofVec3f(x, y, z));
        else pOut.push_back(ofVec3f(x, y, z));
    }
    
//    ofxCsv csvRecorder;
//    csvRecorder.clear();
//    ofxCsvRow row;
//    row.setInt(0, 876);
//    row.setInt(1, 543);
//    csvRecorder.addRow(row);
//    row.setString(0, "hello");
//    row.setString(1, "hi");
//    row.setString(2, "hey there");
//    row.setInt(3, 456);
//    row.setInt(0, 79);
//    row.setString(6, "me");
//    csvRecorder.addRow(row);
//    string d = "Data";
//    int w = 143;
//    d.append(ofToString(w));
//    d.append(".csv");
//    csvRecorder.save(d);
    
}

//--------------------------------------------------------------
void ofApp::update(){
//    dirLight1.setPosition(-100, 0, 0);
//    dirLight1.lookAt(ofVec3f(0, 0, 0));
//    dirLight2.setPosition(0, -100, 0);
//    dirLight2.lookAt(ofVec3f(0, 0, 0));
//    dirLight3.setPosition(0, 0, -100);
//    dirLight3.lookAt(ofVec3f(0, 0, 0));
//    dirLight4.setPosition(100, 0, 0);
//    dirLight4.lookAt(ofVec3f(0, 0, 0));
//    dirLight5.setPosition(0, 100, 0);
//    dirLight5.lookAt(ofVec3f(0, 0, 0));
//    dirLight6.setPosition(0, 0, 100);
//    dirLight6.lookAt(ofVec3f(0, 0, 0));
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
    
    // venation
//    v.draw();
    
    //raycasting
//    rtIntersect.drawDebug();
//    rtIntersect.drawRayDebug();

    // bounding box
//    boundingBox.drawWireframe();
//
//    // model
////    ofSetColor(255, 255, 255);
////    model.draw(OF_MESH_WIREFRAME);
//
//    // mesh of the model
//    ofSetColor(255, 0, 0);
//    modelMesh.drawWireframe();
//
//    // drawing points
//    ofPushStyle();
//    ofFill();
//    ofSetColor(0, 255, 255);
//    for (int i = 0; i < pIn.size(); i++)
//        ofDrawSphere(pIn[i].x, pIn[i].y, pIn[i].z, 0.2);
//    ofSetColor(255, 0, 0);
//    for (int i = 0; i < pOut.size(); i++)
//        ofDrawSphere(pOut[i].x, pOut[i].y, pOut[i].z, 0.2);
//    ofPopStyle();
    
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
//    v.setup(240, 2, 1200);
        v.setup(100, 2, 480);
//        v.setup(50, 2, 20);
        cam.reset();
    }
    if (key == 's') v.saveFile();
    if (key == 'p')
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
