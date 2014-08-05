#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxCv.h"
#include "blobGesture.h"

class ofApp : public ofBaseApp{
  public:
	void setup();
	void update();
	void draw();
	void exit();

	void mousePressed(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);

	void keyPressed(ofKeyEventArgs& args);

	blobGesture blob;


};

