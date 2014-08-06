#include "ofApp.h"
#include "ofxObjLoader.h"
#include "ofxBinaryMesh.h"

static string dataPath = "../../../sharedData/";

void ofApp::setup() {
	ofSetVerticalSync(true);

	blob.loadBrushSource(dataPath+"strokes/blob_stroke.png");
	//blob.loadBrushSource(dataPath+"strokes/spike_stroke.png");

}

void ofApp::update() {

}

void ofApp::draw(){
	ofBackground(ofColor::bisque);
	
	ofColor c = ofColor::bisque.getInverted();
	c.setBrightness(128);
	ofSetColor(c);

	blob.blobMesh.draw();    

	ofDisableDepthTest();

	ofSetColor(ofColor::black);
	//blob.blobMesh.drawWireframe();

	ofSetColor(ofColor::white, 100);
	blob.traceDebug.draw();
	if(ofGetKeyPressed('d')){
		blob.debugLines.draw();
	}
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
