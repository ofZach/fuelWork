#include "ofApp.h"

vector < string > lines ;

void ofApp::setup() {
	ofSetVerticalSync(true);
	faceShift.setup();
	faceShift.import("OBJs");
	
	useEasyCam = true;

	light.enable();
	light.setPosition(+500, +500, +500);
    
	string parseFile = "SequenceLonger_absolute_mm.txt";
    lines = ofSplitString(ofBufferFromFile(parseFile), "\n");
	player.parseFrames(parseFile);
    
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
    
	//cout <<  lines.size() << endl;

    faceShiftFrame frame = player.getLineForTimeMillis(millis, false);  //player.getLineForTimeMillis(ofGetElapsedTimeMillis(), true);
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
    
	
	if(useEasyCam){
		ofBackground(128);
		cam.begin();

		//old way of moving head, commented this out for 
		ofRotateX(180);
		ofTranslate(faceShift.getPosition());
		ofScale(-1, 1, 1); // for some reason the rotation matrix x is flipped
		glMultMatrixf((GLfloat*) faceShift.getRotationMatrix().getPtr());
		ofScale(-1, 1, 1); // then we flip it back
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

		ofTranslate(faceShift.getPosition());
		ofMultMatrix(faceShift.getRotationMatrix());

	}
	cout << "position is " << faceShift.getPosition() << " " << mouseX << endl;


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
