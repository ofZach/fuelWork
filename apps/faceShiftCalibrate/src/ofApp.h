#pragma once

#include "ofMain.h"
#include "ofxFaceShift.h"
#include "faceShiftPlayer.h"
#include "ofxUI.h"

#include "ofxCv.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(ofKeyEventArgs& args);

	ofxFaceShift faceShift;
	
	bool useEasyCam;
	faceShiftPlayer player;
    int lastFrame;

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

	ofxUISuperCanvas* adjustGui;
	ofVec3f adjustments;
	int offsetShiftMillis;
	
	bool showWireframe;
	bool showFilled;

	bool showObjSequence;
	bool showBlendShape;

	ofVec2f depthPrincipalPoint;
	ofVec2f depthFOV;
	ofRectangle depthImageSize;
	
	ofVec2f colorPrincipalPoint;
	ofVec2f colorFOV;
	ofRectangle colorImageSize;

	float depthToRGBRotation[9];
	ofVec3f depthToRGBTranslation;
//	ofMatrix4x4 depthToRGBRotation;
	ofMatrix4x4 extrinsics;
	ofMatrix4x4 getAdjustedMatrix();
	
	ofVec3f distortionK;
	ofVec2f distortionP;

	ofVideoPlayer backdrop;
	ofFbo targetFbo;

	ofMesh unityObjTestMesh;
	ofImage testOverlay;
	vector<ofMesh> meshes;
	//ofVbo curObjMesh;
	int curMesh;

	void drawObjSequence();
	void drawBlendShape();

};

