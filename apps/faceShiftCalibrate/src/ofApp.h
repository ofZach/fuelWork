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

	ofEasyCam cam;
	ofCamera baseCamera;
	ofLight light;
	
	bool useEasyCam;
	ofxFaceShift faceShift;
	faceShiftPlayer player;
    int lastFrame;

	ofVideoPlayer backdrop;
	ofFbo targetFbo;

	ofMesh unityObjTestMesh;
	ofImage testOverlay;
	vector<ofMesh> meshes;
	int curMesh;

	void drawObjSequence();
	void drawBlendShape();
	void drawMesh(ofMesh& m, ofFloatColor color);

	//draw from nose;
	//vector<ofVec3f> noseDrawPoints;
	ofMesh noseDrawPoints;
	ofIndexType noseVertexIndex;
	float noseDrawDistance;
	ofVec3f noseDrawPivot;
	ofVec3f nosePos;
	ofxUISuperCanvas* adjustGui;
	ofVec3f adjustments;
	int offsetShiftMillis;
	bool showWireframe;
	bool showFilled;
	bool showObjSequence;
	bool showBlendShape;
	float videoAlpha;

	bool loadCalibration(string rgbIntrinsicsPath, 
						 string depthIntrinsicsPath,
						 string rotationPath, 
						 string translationPath);

    ofxCv::Calibration depthCalibration, rgbCalibration;
	cv::Mat rotationDepthToRGB, translationDepthToRGB;



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


};

