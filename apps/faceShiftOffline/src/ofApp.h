#pragma once

#include "ofMain.h"
#include "ofxFaceShift.h"
#include "faceShiftPlayer.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(ofKeyEventArgs& args);

	ofxFaceShift faceShift;
	
	bool useEasyCam;
	faceShiftPlayer player;
    
	ofEasyCam cam;
	ofCamera baseCamera;

	ofLight light;
    ofVideoPlayer vid;
    ofSoundPlayer snd;
    
    
};

