#pragma once

#include "ofMain.h"
#include "ofxFaceShift.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(ofKeyEventArgs& args);

	ofxFaceShift faceShift;
	
	bool useEasyCam;
	ofEasyCam cam;
	ofCamera baseCamera;

	ofLight light;
};

