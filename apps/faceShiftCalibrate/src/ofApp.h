#pragma once

#include "ofMain.h"
#include "ofxFaceShift.h"
#include "faceShiftPlayer.h"

#include "ofxCv.h"

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
    ofSoundPlayer snd;

	bool loadCalibration(string rgbIntrinsicsPath, 
						 string depthIntrinsicsPath,
						 string rotationPath, 
						 string translationPath);

    ofxCv::Calibration depthCalibration, rgbCalibration;
	cv::Mat rotationDepthToRGB, translationDepthToRGB;

	ofMatrix4x4 depthToRGBView;
	ofMatrix4x4 rgbProjection;
	ofMatrix4x4 rgbMatrix;
	ofMatrix4x4 depthProjection;

	ofVec2f depthPrincipalPoint;
	ofVec2f depthFOV;
	ofRectangle depthImageSize;
	
	ofVec2f colorPrincipalPoint;
	ofVec2f colorFOV;
	ofRectangle colorImageSize;

	float depthToRGBRotation[9];
	ofVec3f depthToRGBTranslation;
	ofMatrix4x4 extrinsics;
	ofMatrix4x4 getAdjustedMatrix();
	
	ofVec3f distortionK;
	ofVec2f distortionP;

	ofVideoPlayer backdrop;
	ofFbo targetFbo;

	ofImage testOverlay;
	
};

