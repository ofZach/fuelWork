#include "testApp.h"
#include "ofxBinaryMesh.h"
#include "ofxObjLoader.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::exit()
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

	for(int i = 0; i < dragInfo.files.size(); i++){
		recurseForOBJ(dragInfo.files[i], dragInfo.files[i]);
	}
}

void testApp::recurseForOBJ(string path, string root){
	if( ofFile(path).isDirectory() ) {
		cout << "***SEARCHING " << path << endl;
		ofDirectory dir(path);
		dir.listDir();
		for(int f = 0; f < dir.numFiles(); f++){
			recurseForOBJ(dir.getPath(f), root);
		}
	}
	else if(ofToLower(ofFilePath::getFileExt(path)) == "obj"){
		cout << "CONVERTING " << path << " with root " << root << endl;
		convertObj(path, root);
	}
}

void testApp::convertObj(string path, string root){
	string outputPath = path;	
	ofStringReplace(outputPath,root,string(root+"_OBMS/") );
	string pathWithoutExtension = ofFilePath::removeExt(outputPath);

	string enclosingDirectory = ofFilePath::getEnclosingDirectory(outputPath);
	if(!ofDirectory(enclosingDirectory).exists()){
		ofDirectory(enclosingDirectory).create(true);
	}
	cout << "OBM Enclosing Directory is  " << enclosingDirectory << endl;

	ofMesh m;
	cout << "LOADING " << outputPath << endl;
	ofxObjLoader::load_oldway( path, m, false);
	cout << "SAVING " << pathWithoutExtension << ".obm" << endl;
	ofxBinaryMesh::save( pathWithoutExtension + ".obm", m);
	cout << "Saved " << pathWithoutExtension << ".obm" << endl;
}