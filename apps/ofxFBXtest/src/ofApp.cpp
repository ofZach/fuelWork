#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    scene.load("tbo_test.fbx");
    
}

//--------------------------------------------------------------
void ofApp::update(){

    scene.setFrame(mouseX);
    
    //scene.setTime( ofGetElapsedTimeMillis() );
}

ofEasyCam cam;
//--------------------------------------------------------------
void ofApp::draw(){

    
    cam.begin();
    for (int i = 0; i < scene.getMeshes().size(); i++){
        scene.getMeshes()[i]->drawWireframe();
    }
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
