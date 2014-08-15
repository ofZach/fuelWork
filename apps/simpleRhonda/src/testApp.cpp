#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    
    midScreen.set(ofGetWidth()/2, ofGetHeight()/2);
    
    
    ofSetupScreenOrtho();
    
    
    
    
    mouseDownCounter = 0;
    
    
    
}

//--------------------------------------------------------------
void testApp::update(){

    
    
    //cam.setFov(mouseX*10);
    
    if (ofGetKeyPressed('a') == true){
        cam.rotate(1, 0, 1, 0);
    }
    
    if (ofGetKeyPressed('s') == true){
        cam.rotate(-1, 0, 1, 0);
    }
    
    
    if (ofGetKeyPressed('z') == true){
        cam.rotate(1, 1, 0, 0);
    }
    
    if (ofGetKeyPressed('x') == true){
        cam.rotate(-1, 1, 0, 0);
    }
    
    
    
    
    if (ofGetMousePressed() == true){
        //mat = ofMatrix4x4::newIdentityMatrix();
        mat = cam.getModelViewMatrix();
    } else {
        mat = cam.getModelViewMatrix();
    }
    
    
    
}

//--------------------------------------------------------------
void testApp::draw(){

    
    ofBackground(0);

    
    
    
    ofTranslate(midScreen);
    
    ofMatrix4x4 mat = cam.getModelViewMatrix();
    mat.setTranslation(0,0,0);

    
    
    ofSetColor(255,255,255,255);

    
    
    ofMesh mesh; mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    
    if (line.size() > 2){
        
        for (int i = 0; i < line.getVertices().size(); i++){

            int i_m_1 = MAX(0, i-1);
            int i_p_1 = MIN(line.size()-1, i+1);
            
            ofPoint onScreenA = mat*line[i_m_1];
            ofPoint onScreenB = mat*line[i_p_1];
            ofPoint screenPosA(onScreenA.x, onScreenA.y, 0);            // remove z components
            ofPoint screenPosB(onScreenB.x, onScreenB.y, 0);
            ofPoint dif = screenPosB - screenPosA;
            dif.normalize();
            ofPoint difCross = dif.cross(ofPoint(0,0,1));
            
            ofPoint a = mat*(line[i]) + 8 * difCross;
            ofPoint b = mat*(line[i]) - 8 * difCross;
            
            mesh.addVertex(b);
            mesh.addVertex(a);

        }
        mesh.draw();
    }
    
    
    for (int j = 0; j < drawnLines.size(); j++){
        mesh.clear();
        for (int i = 0; i < drawnLines[j].getVertices().size(); i++){
            
            int i_m_1 = MAX(0, i-1);
            int i_p_1 = MIN(drawnLines[j].size()-1, i+1);
            
            ofPoint onScreenA = mat*drawnLines[j][i_m_1];
            ofPoint onScreenB = mat*drawnLines[j][i_p_1];
            ofPoint screenPosA(onScreenA.x, onScreenA.y, 0);            // remove z components
            ofPoint screenPosB(onScreenB.x, onScreenB.y, 0);
            ofPoint dif = screenPosB - screenPosA;
            dif.normalize();
            ofPoint difCross = dif.cross(ofPoint(0,0,1));
            
            ofPoint a = mat*(drawnLines[j][i]) + 8 * difCross;
            ofPoint b = mat*(drawnLines[j][i]) - 8 * difCross;
            
            mesh.addVertex(b);
            mesh.addVertex(a);
            
        }
        mesh.draw();
    }
    
    
    
    
    cam.begin();
    cam.end();
    
    //ofSetColor(ofColor::aqua);
    //line.draw();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if (key == ' ' ){
        drawnLines.clear();
        line.clear();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}



//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    
    ofMatrix4x4 inverseCamera;
	inverseCamera.makeInvertOf(cam.getModelViewMatrix());
    ofPoint ptToAdd = ofPoint(x,y) - midScreen;
    ptToAdd = inverseCamera * ptToAdd;
    line.addVertex( ptToAdd );

    line = line.getResampledByCount(500);
    
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    line.clear();
    ofMatrix4x4 inverseCamera;
	inverseCamera.makeInvertOf(cam.getModelViewMatrix());
    ofPoint ptToAdd = ofPoint(x,y) - midScreen;
    ptToAdd = inverseCamera * ptToAdd;
    line.addVertex( ptToAdd );
    
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    if (line.size() > 2){
        drawnLines.push_back(line);
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
