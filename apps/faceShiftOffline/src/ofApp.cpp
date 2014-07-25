#include "ofApp.h"

vector < string > lines ;

void ofApp::setup() {
	ofSetVerticalSync(true);
	faceShift.setup();
	faceShift.import("OBJs");
	
	light.enable();
	light.setPosition(+500, +500, +500);
    
    lines = ofSplitString(ofBufferFromFile("SequenceLonger.txt"), "\n");
    //cout << lines[0] << endl;
    
    player.parseFrames("SequenceLonger.txt");
    
    faceShift.parse(lines[0]);
    
    vid.loadMovie("time_lapse.mov");
    vid.play();
    vid.setPaused(true);
    
    snd.loadSound("SequenceLonger_withav.wav");
    snd.play();
}

int lastFame = -1;
void ofApp::update() {
    
    
    int millis = snd.getPositionMS();
    
    
    //vid.update();
    //float t = vid.getDuration() * vid.getPosition();
    //cout << t << endl;
    
    
    faceShiftFrame frame =  player.getLineForTimeMillis(millis, false);  //player.getLineForTimeMillis(ofGetElapsedTimeMillis(), true);
    //cout << frame.frameNum << endl;
    
    if (frame.frameNum != lastFame){
        faceShift.parse(frame.frameString);
        lastFame = frame.frameNum;
        vid.setFrame(lastFame);
        vid.update();
    }
    //faceShift.parse(lines[ ofGetFrameNum() % lines.size() ]);
	//faceShift.update();
}

void ofApp::draw(){
    
   
	ofBackground(128);
    
    ofSetColor(255,255,255);
    vid.draw(0,0);
    
	
	cam.begin();
	glEnable(GL_DEPTH_TEST);
	ofRotateX(180);
	ofTranslate(faceShift.getPosition());
	ofScale(-1, 1, 1); // for some reason the rotation matrix x is flipped
	glMultMatrixf((GLfloat*) faceShift.getRotationMatrix().getPtr());
	ofScale(-1, 1, 1); // then we flip it back
	
	ofEnableLighting();
	ofSetColor(255);
	faceShift.getBlendMesh().draw();
	
	ofDisableLighting();
	ofSetColor(0);
	faceShift.getBlendMesh().drawWireframe();
	
	cam.end();
}
