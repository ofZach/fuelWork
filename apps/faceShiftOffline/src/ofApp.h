#pragma once

#include "ofMain.h"
#include "ofxFaceShift.h"
#include "faceShiftPlayer.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	ofxFaceShift faceShift;
	faceShiftPlayer player;
    
	ofEasyCam cam;
	ofLight light;
    ofVideoPlayer vid;
    ofSoundPlayer snd;
    
    
};

