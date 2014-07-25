#include "ofApp.h"

vector < string > lines ;

void ofApp::setup() {
	ofSetVerticalSync(true);
	faceShift.setup();
	faceShift.import("OBJs");
	
	useEasyCam = false;

	light.enable();
	light.setPosition(+500, +500, +500);
    
    lines = ofSplitString(ofBufferFromFile("SequenceLonger_absolute_mm.txt"), "\n");
    //lines = ofSplitString(ofBufferFromFile("SequenceLonger_neutral.txt"), "\n");
    
	//cout << lines[0] << endl;
    
    faceShift.parse(lines[0]);
}

void ofApp::update() {
    
   // cout <<  lines.size() << endl;
    
    faceShift.parse(lines[ ofGetFrameNum() % lines.size() ]);
	//faceShift.update();
}

void ofApp::draw(){
	
	if(useEasyCam){
		ofBackground(128);
		cam.begin();

//		ofRotateX(180);
//		ofTranslate(faceShift.getPosition());
//		ofScale(-1, 1, 1); // for some reason the rotation matrix x is flipped
//		glMultMatrixf((GLfloat*) faceShift.getRotationMatrix().getPtr());
//		ofScale(-1, 1, 1); // then we flip it back
	}
	else{
		ofBackground(0);
		ofPushStyle();
		ofSetColor(128);
		ofRect(0,0,640,480);
		ofPopStyle();

		baseCamera.setPosition(0,0,0);
		baseCamera.lookAt(ofVec3f(0,0,1));
		baseCamera.begin(ofRectangle(0,0,640,480));


	}
	cout << "position is " << faceShift.getPosition() << " " << mouseX << endl;

	ofTranslate(faceShift.getPosition() * mouseX);
	ofMultMatrix(faceShift.getRotationMatrix());

	glEnable(GL_DEPTH_TEST);
	
	ofEnableLighting();
	ofSetColor(255);
	faceShift.getBlendMesh().draw();
	
	ofDisableLighting();
	ofSetColor(0);
	faceShift.getBlendMesh().drawWireframe();
	
	if(useEasyCam){
		cam.end();
	}
	else{
		baseCamera.end();
	}

}

void ofApp::keyPressed(ofKeyEventArgs& args){
	if(args.key == ' '){
		useEasyCam = !useEasyCam;
	}
}
