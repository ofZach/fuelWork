#include "ofApp.h"

vector < string > lines ;

static string dataPath = "../../../sharedData/";

void ofApp::setup() {
	ofSetVerticalSync(true);
	
	useEasyCam = false;

	light.enable();
	light.setPosition(+500, +500, +500);

	//depthCalib.load(dataPath + "calibration/depthCalib.yml");
	loadCalibration(
			dataPath + "saturday_test_two/matrices/rgbCalib.yml", 
			dataPath + "saturday_test_two/matrices/depthCalib.yml", 
			dataPath + "saturday_test_two/matrices/rotationDepthToRGB.yml", 
			dataPath + "saturday_test_two/matrices/translationDepthToRGB.yml");
	
	faceShift.setup();
	faceShift.import(dataPath + "OBJs");
	string parseFile = dataPath + "saturday_test_two/20140725_AlexanderFunTimeSaturdayThree.txt";
    lines = ofSplitString(ofBufferFromFile(parseFile), "\n");
	player.parseFrames(parseFile);
    faceShift.parse(lines[0]);

	testOverlay.loadImage(dataPath + "/saturday_test_two/frametest_saturday_2.png");

	backdrop.loadMovie(dataPath + "saturday_test_two/20140725_AlexanderFunTimeSaturdayThree.mov");
	backdrop.play();
	backdrop.setVolume(0);

	targetFbo.allocate(rgbCalibration.getDistortedIntrinsics().getImageSize().width,
					   rgbCalibration.getDistortedIntrinsics().getImageSize().height,GL_RGB);

//    vid.loadMovie("time_lapse.mov");
//    vid.play();
//    vid.setPaused(true);
//    snd.loadSound("SequenceLonger_withav.wav");
//    snd.play();

}

int lastFame = -1;
void ofApp::update() {
    
    //int millis = snd.getPositionMS();
	int millis = ofGetElapsedTimeMillis();
    faceShiftFrame frame = player.getLineForTimeMillis(millis, true);
	
	
    if (frame.frameNum != lastFame){
		faceShift.parse(frame.frameString);
        lastFame = frame.frameNum;
    }

	//player.getLineForTimeMillis(ofGetElapsedTimeMillis(), true);
    //cout << frame.frameNum << endl;

	backdrop.update();
}

void ofApp::draw(){
    
	targetFbo.begin();
	ofClear(0,0,0,0);

	ofPushStyle();
	ofSetColor(255);
	//cout << "BLEEP BLEEP " << backdrop.getPosition() << endl;
	backdrop.draw(0,0);
	ofPopStyle();
	if(useEasyCam){
		cam.begin();
	}
	else{
		ofVec3f camPos(0,0,0);
		camPos = extrinsics * camPos;

		baseCamera.setPosition(0,0,0);
		baseCamera.lookAt(ofVec3f(0,0,-1));
		baseCamera.setFov( rgbCalibration.getDistortedIntrinsics().getFov().y );
		baseCamera.begin(ofRectangle(0,0,targetFbo.getWidth(),targetFbo.getHeight()));
	}

	ofEnableDepthTest();
	ofEnableLighting();

	ofPushMatrix();

	if(ofGetKeyPressed('m')){
		ofMultMatrix(extrinsics);		
	}

	ofVec3f neckTranslation(0.01883798, -1.526634, -0.6242198);
	ofTranslate(-neckTranslation*100);
	ofMultMatrix(faceShift.getRotationMatrix());
	ofTranslate(faceShift.getPosition());

	ofSetColor(255);
	faceShift.getBlendMesh().draw();
	
	ofDisableLighting();
	ofSetColor(0);
	faceShift.getBlendMesh().drawWireframe();
	ofPopMatrix();
	
	ofDisableLighting();
	ofDisableDepthTest();

	if(useEasyCam){
		cam.end();
	}
	else{
		baseCamera.end();
	}



	targetFbo.end();

	ofRectangle videoRect(0,0,targetFbo.getWidth(),targetFbo.getHeight());
	ofRectangle screenRect(0,0,ofGetWidth(),ofGetHeight());
	videoRect.scaleTo(screenRect);

	ofSetColor(255);
	targetFbo.getTextureReference().draw(videoRect);

}

bool ofApp::loadCalibration(string rgbIntrinsicsPath, 
							string depthIntrinsicsPath,
							string rotationPath, 
							string translationPath)
{

	if(!ofFile::doesFileExist(rgbIntrinsicsPath) ||
	   !ofFile::doesFileExist(depthIntrinsicsPath) ||
	   !ofFile::doesFileExist(rotationPath) ||
	   !ofFile::doesFileExist(translationPath))
	{
		ofLogError() << "ofxRGBDRenderer::setup -- Missing one or more matrix files! " << rgbIntrinsicsPath << " " <<
			depthIntrinsicsPath << " " <<
			rotationPath << " " <<
			translationPath;
		return false;
	}
	
	depthCalibration.load(depthIntrinsicsPath);
	rgbCalibration.load(rgbIntrinsicsPath);

	ofxCv::loadMat(rotationDepthToRGB, rotationPath);
	ofxCv::loadMat(translationDepthToRGB, translationPath);

	//NO LONGER USED------
	/*
	depthToRGBView = ofxCv::makeMatrix(rotationDepthToRGB, translationDepthToRGB);

	ofPushView();
	rgbCalibration.getDistortedIntrinsics().loadProjectionMatrix();
	glGetFloatv(GL_PROJECTION_MATRIX, rgbProjection.getPtr());
	ofPopView();

	ofPushView();
	depthCalibration.getDistortedIntrinsics().loadProjectionMatrix();
	glGetFloatv(GL_PROJECTION_MATRIX, depthProjection.getPtr());
	ofPopView();
	rgbMatrix = (depthToRGBView * rgbProjection);
	*/
	///-------
	
	//	Point2d fov = depthCalibration.getUndistortedIntrinsics().getFov();
	//	fx = tanf(ofDegToRad(fov.x) / 2) * 2;
	//	fy = tanf(ofDegToRad(fov.y) / 2) * 2;
	//	fx = depthCalibration.getUndistortedIntrinsics().getCameraMatrix().at<double>(0,0);
	//	fy = depthCalibration.getUndistortedIntrinsics().getCameraMatrix().at<double>(1,1);
	//	principalPoint = depthCalibration.getUndistortedIntrinsics().getPrincipalPoint();
	//	imageSize = depthCalibration.getUndistortedIntrinsics().getImageSize();

	//intrinsics
	depthFOV.x = depthCalibration.getDistortedIntrinsics().getCameraMatrix().at<double>(0,0);
	depthFOV.y = depthCalibration.getDistortedIntrinsics().getCameraMatrix().at<double>(1,1);
	depthPrincipalPoint = ofxCv::toOf(depthCalibration.getDistortedIntrinsics().getPrincipalPoint());
	depthImageSize = ofRectangle(0, 0,
								 depthCalibration.getDistortedIntrinsics().getImageSize().width,
								 depthCalibration.getDistortedIntrinsics().getImageSize().height);

	colorFOV.x = rgbCalibration.getDistortedIntrinsics().getCameraMatrix().at<double>(0,0);
	colorFOV.y = rgbCalibration.getDistortedIntrinsics().getCameraMatrix().at<double>(1,1);
	colorPrincipalPoint = ofxCv::toOf( rgbCalibration.getDistortedIntrinsics().getPrincipalPoint() );
	colorImageSize = ofRectangle(0,0,
								 rgbCalibration.getDistortedIntrinsics().getImageSize().width,
								 rgbCalibration.getDistortedIntrinsics().getImageSize().height);
	
	//extrinsics
	depthToRGBTranslation = ofVec3f(translationDepthToRGB.at<double>(0,0),
									translationDepthToRGB.at<double>(1,0),
									translationDepthToRGB.at<double>(2,0));
	cv::Mat rx3;

	if(rotationDepthToRGB.rows == 3 && rotationDepthToRGB.cols == 3) {
		cout << "LOADING 3x3 ROTATION!" << endl;
		rotationDepthToRGB.copyTo(rx3);
		float rotation3fv[9] = {
			float(rx3.at<double>(0,0)),float(rx3.at<double>(1,0)),float(rx3.at<double>(2,0)),
			float(rx3.at<double>(0,1)),float(rx3.at<double>(1,1)),float(rx3.at<double>(2,1)),
			float(rx3.at<double>(0,2)),float(rx3.at<double>(1,2)),float(rx3.at<double>(2,2))
		};
		memcpy(depthToRGBRotation, rotation3fv, sizeof(float)*3*3);
	}
	else {
		//openFrameworkds needs a better Matrix3x3 class...
		cv::Rodrigues(rotationDepthToRGB, rx3);
		float rotation3fv[9] = {
			float(rx3.at<double>(0,0)),float(rx3.at<double>(1,0)),float(rx3.at<double>(2,0)),
			float(rx3.at<double>(0,1)),float(rx3.at<double>(1,1)),float(rx3.at<double>(2,1)),
			float(rx3.at<double>(0,2)),float(rx3.at<double>(1,2)),float(rx3.at<double>(2,2))
		};
		memcpy(depthToRGBRotation, rotation3fv, sizeof(float)*3*3);
	}
	
	float mat4x4[16] = {
		depthToRGBRotation[0],depthToRGBRotation[1],depthToRGBRotation[2],0,
		depthToRGBRotation[3],depthToRGBRotation[4],depthToRGBRotation[5],0,
		depthToRGBRotation[6],depthToRGBRotation[7],depthToRGBRotation[8],0,
		depthToRGBTranslation.x,depthToRGBTranslation.y,depthToRGBTranslation.z,1
	};
	
	extrinsics = ofMatrix4x4(mat4x4);

	//windows seems to load these differently sometimes
	cv::Mat dis = rgbCalibration.getDistCoeffs();
	if(dis.cols == 1){
		distortionK = ofVec3f(dis.at<double>(0,0),
							  dis.at<double>(1,0),
							  dis.rows == 5 ? dis.at<double>(4,0) : 0);
		distortionP = ofVec2f(dis.at<double>(2,0),dis.at<double>(3,0));
	}
	else if(dis.rows == 1){
		distortionK = ofVec3f(dis.at<double>(0,0),
							  dis.at<double>(0,1),
							  dis.cols == 5 ? dis.at<double>(0,4) : 0);
		distortionP = ofVec2f(dis.at<double>(0,2),dis.at<double>(0,3));	
	}

	//distortionK = ofVec3f(dis.at<double>(0,0),
	//					  dis.at<double>(0,1),
	//					  dis.rows == 5 ? dis.at<double>(0,4) : 0);
	//distortionP = ofVec2f(dis.at<double>(2,0),dis.at<double>(3,0));

	return true;
}

void ofApp::keyPressed(ofKeyEventArgs& args){
	if(args.key == ' '){
		useEasyCam = !useEasyCam;
	}
}
