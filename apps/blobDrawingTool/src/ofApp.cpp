#include "ofApp.h"
#include "ofxObjLoader.h"
#include "ofxBinaryMesh.h"

static string dataPath = "../../../sharedData/";

void ofApp::setup() {
	ofSetVerticalSync(true);
}

void ofApp::update() {

}

void ofApp::draw(){
	ofBackground(ofColor::bisque);

	ofSetColor(ofColor::bisque.getInverted());
	blob.blobMesh.draw();    

	ofSetColor(ofColor::black);
	blob.blobMesh.drawWireframe();
}

void ofApp::exit(){
//	adjustGui->saveSettings("adjustments.xml");
}


void ofApp::mousePressed(ofMouseEventArgs& args){
	blob.clear();

	blob.addGesture(ofVec3f(args.x,args.y,0));
}

void ofApp::mouseDragged(ofMouseEventArgs& args){
	blob.addGesture(ofVec3f(args.x,args.y,0));
}

void ofApp::mouseReleased(ofMouseEventArgs& args){
	blob.addGesture(ofVec3f(args.x,args.y,0));
	blob.buildMesh();
}


void ofApp::keyPressed(ofKeyEventArgs& args){

}
