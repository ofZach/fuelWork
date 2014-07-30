#include "ofApp.h"
#include "ofxObjLoader.h"
#include "ofxBinaryMesh.h"

vector < string > lines ;

static string dataPath = "../../../sharedData/";

void ofApp::setup() {
	ofSetVerticalSync(true);
	
	useEasyCam = false;

	light.enable();
	light.setPosition(+500, +500, +500);

	loadCalibration(
			dataPath + "saturday_test_two/matrices/rgbCalib.yml", 
			dataPath + "saturday_test_two/matrices/depthCalib.yml", 
			dataPath + "saturday_test_two/matrices/rotationDepthToRGB.yml", 
			dataPath + "saturday_test_two/matrices/translationDepthToRGB.yml");
	
	//faceShift.setup();
	//faceShift.import(dataPath + "OBJs");
	//string parseFile = dataPath + "saturday_test_two/20140725_AlexanderFunTimeSaturdayThree.txt";
    //lines = ofSplitString(ofBufferFromFile(parseFile), "\n");
	//player.parseFrames(parseFile);
    //faceShift.parse(lines[0]);

	testOverlay.loadImage(dataPath + "/saturday_test_two/frametest_saturday_2.png");

	curMesh = 0;
	ofDirectory dir(dataPath + "saturday_test_two/objsequence");
	dir.allowExt("obm");
	dir.listDir();
	meshes.resize(dir.numFiles());
	for(int i = 0; i < dir.numFiles(); i++){
		cout << "cur obm Is " << i << endl;
//		ofxObjLoader::load(dir.getPath(i), meshes[i], false, false);
		ofxBinaryMesh::load(dir.getPath(i), meshes[i]);
	}

	backdrop.loadMovie(dataPath + "saturday_test_two/20140725_AlexanderFunTimeSaturdayThree.mov");
	backdrop.play();
	backdrop.setVolume(0);

	targetFbo.allocate(rgbCalibration.getDistortedIntrinsics().getImageSize().width,
					   rgbCalibration.getDistortedIntrinsics().getImageSize().height,GL_RGB);

	//ofxObjLoader::load(dataPath + "saturday_test_two/unitytest5.obj", unityObjTestMesh, false, false);
	//unityTestObjAssimp.loadModel(dataPath + "saturday_test_two/unitytest2.obj");


	adjustGui = new ofxUISuperCanvas("ADJUST", 0,0, 300,500);
	adjustGui->addMinimalSlider("ADJUST X", -50, 50, &adjustments.x);
	adjustGui->addMinimalSlider("ADJUST Y", -50, 50, &adjustments.y);
	adjustGui->addMinimalSlider("ADJUST Z", -50, 50, &adjustments.z);
	adjustGui->loadSettings("adjustments.xml");
//	adjust->load
}

int lastFame = -1;
void ofApp::update() {
    
    //int millis = snd.getPositionMS();
	//int millis = ofGetElapsedTimeMillis();
	//int millis = ofMap(mouseX, 0, ofGetWidth(), 0, player.getDurationMillis(), true);
	int faceShiftClapMillis = 6168.22;
	int videoClapMillis = 2.3499*1000;
	
	//int offsetShift = ofMap(mouseX,0, ofGetWidth(), -500, 500, true);
	//cout << offsetShift << endl;
	int offsetShift = 193;

	int millis = backdrop.getPosition() * backdrop.getDuration() * 1000 + (faceShiftClapMillis - videoClapMillis) + offsetShift;// - ;
	
	/*
	faceShiftFrame frame = player.getLineForTimeMillis(millis, true);
	if(frame.frameNum == 184){
		cout << "frame millis is " << frame.frameTimeMillis << endl;
	}
    if (frame.frameNum != lastFame){

		faceShift.parse(frame.frameString);
        lastFame = frame.frameNum;
    }
	*/

	curMesh = ofClamp( (millis/1000.0) * 30.0, 0,meshes.size()-1);

	//player.getLineForTimeMillis(ofGetElapsedTimeMillis(), true);
    //cout << frame.frameNum << endl;

	backdrop.update();
}

void ofApp::draw(){
    
	targetFbo.begin();
	ofClear(0,0,0,0);

	///draw the video layer behind
	ofPushStyle();
	ofSetColor(255);
	backdrop.draw(0,0);
	ofPopStyle();
	////////////////////

	if(useEasyCam){
		cam.begin();
		ofPushStyle();
		ofPushMatrix();
		ofNoFill();
		ofColor(255,0,0);
//		ofDrawSphere(depthToRGBTranslation, 10);
		ofNode n;
		n.setPosition(depthToRGBTranslation);
		n.draw();
		ofColor(0,250,0);
		ofSphere(0,0,0,10);
		ofFill();
		ofSetColor(255,0,0);
		if(ofGetKeyPressed('m')){
			ofMultMatrix(extrinsics);
		}
		ofSetLineWidth(5);
		ofLine(ofVec3f(0,0,0), ofVec3f(0,0,-100));
		ofPopMatrix();
		ofPopStyle();
	}
	else{
		ofVec3f camPos(0,0,0);
		camPos = extrinsics * camPos;
		if(ofGetKeyPressed('m')){
			baseCamera.setPosition( ofVec3f(0,0,0) );
			baseCamera.lookAt(ofVec3f(0,0,-1));
		}
		else{
			baseCamera.setTransformMatrix(extrinsics);			
		}
		baseCamera.setFov( rgbCalibration.getDistortedIntrinsics().getFov().y );
		baseCamera.begin(ofRectangle(0,0,targetFbo.getWidth(),targetFbo.getHeight()));
	}

	ofPushMatrix();

	ofVec3f neckTranslation(0.01883798, -1.526634, -0.6242198);
	//float multiplier = ofMap(mouseY, 0, ofGetHeight(), 0, 75, true); 
//	cout << multiplier << endl;
	float multiplier = 0; 

	neckTranslation *= multiplier;
	/*
	ofMatrix4x4 mat;
	mat.translate(-neckTranslation);
	mat *= faceShift.getRotationMatrix();
	//mat.translate(neckTranslation);
	mat.translate(faceShift.getPosition());
	ofMultMatrix(mat);
	*/

	//this is alexander specific translation copied from the FBX file. Not convinced of the units
	//ofMultMatrix(faceShift.getRotationMatrix());
	//if(ofGetKeyPressed('n')){
	//cout << "translating neck " << -neckTranslation*100 << endl;
	//ofTranslate(-neckTranslation*100);
//	ofMultMatrix(faceShift.getRotationMatrix().getInverse());
	//ofTranslate(faceShift.getPosition());

	///////////////////////////////
	//DEBUG NECK STUFF
	/*
	ofPushStyle();
	ofSetColor(255,0,0);
	ofSphere(10);
	ofSetColor(0,255,0);
	ofSphere(neckTranslation*100,10);
	ofPopStyle();
	*/
	////////////////////////////////
	
	ofSetColor(255);
	ofEnableDepthTest();
	ofEnableLighting();

	//faceShift.getBlendMesh().draw();
	
	ofDisableLighting();
	ofSetColor(0);
	//faceShift.getBlendMesh().drawWireframe();
	ofPopMatrix();
	
	ofPushMatrix();
	ofScale(-1,1,1);
	ofTranslate(adjustments);

	meshes[curMesh].drawWireframe();
	ofPopMatrix();
	//unityTestObjAssimp.drawFaces();

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

void ofApp::exit(){
	adjustGui->saveSettings("adjustments.xml");
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
	//rotationDepthToRGB.at<double>(0,1) *= -1;

	
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
//	cout << "translation is " << depthToRGBTranslation << endl;
//	depthToRGBTranslation /= 100;

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
	
	ofMatrix4x4 rot;
	/*
	float mat4x4[16] = {
		depthToRGBRotation[0],depthToRGBRotation[1],depthToRGBRotation[2],0,
		depthToRGBRotation[3],depthToRGBRotation[4],depthToRGBRotation[5],0,
		depthToRGBRotation[6],depthToRGBRotation[7],depthToRGBRotation[8],0,
		depthToRGBTranslation.x,depthToRGBTranslation.y,depthToRGBTranslation.z,1
	};
	extrinsics = ofMatrix4x4(mat4x4);
	*/
	
	float mat4x4[16] = {
		depthToRGBRotation[0],depthToRGBRotation[1],depthToRGBRotation[2],0,
		depthToRGBRotation[3],depthToRGBRotation[4],depthToRGBRotation[5],0,
		depthToRGBRotation[6],depthToRGBRotation[7],depthToRGBRotation[8],0,
		0,0,0,1
	};

	extrinsics = ofMatrix4x4(mat4x4).getInverse();
	extrinsics.translate(depthToRGBTranslation);
	

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

	return true;
}

void ofApp::keyPressed(ofKeyEventArgs& args){
	if(args.key == ' '){
		useEasyCam = !useEasyCam;
	}

	if(args.key == '1'){
		cout << backdrop.getCurrentFrame() << " " << backdrop.getDuration() * backdrop.getPosition()  << endl;
	}
}
