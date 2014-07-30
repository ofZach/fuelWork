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
		if( ofFile(dragInfo.files[i]).isDirectory() ) {
			ofDirectory dir(dragInfo.files[i]);
			dir.allowExt("obj");
			dir.listDir();
			for(int f = 0; f < dir.numFiles(); f++){
				convertObj(dir.getPath(f));
			}
		}
		else if( ofToLower(ofFilePath::getFileExt(dragInfo.files[i])) == "obj" ){
			convertObj( dragInfo.files[i] );
		}
		else{
			cout << dragInfo.files[i] << " is not a directory or an obj file" << endl;
		}
	}
}

void testApp::convertObj(string path){
	string pathWithoutExtension = ofFilePath::removeExt(path);
	ofMesh m;
	cout << "LOADING " << path << endl;
	ofxObjLoader::load_oldway( path, m, false);
	cout << "SAVING " << pathWithoutExtension << ".obm" << endl;
	ofxBinaryMesh::save( pathWithoutExtension + ".obm", m);
	cout << "Saved " << pathWithoutExtension << ".obm" << endl;
}