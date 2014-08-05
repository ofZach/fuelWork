
#include "blobGesture.h"


void blobGesture::addGesture(ofVec3f pos){
	blobGesturePoint bgp;
	bgp.pos = pos;
	bgp.millis = ofGetElapsedTimeMillis();

	trace.push_back(bgp);
}

void blobGesture::buildMesh(){
	
	blobMesh.clear();
	blobMesh.addVertex(trace[0].pos);
	blobMesh.addVertex(trace[0].pos);
	for(int i = 1; i < trace.size(); i++){

		ofVec3f source = trace[i].pos; 
		ofVec3f dir = (trace[i].pos - trace[i-1].pos).normalized();
		ofVec3f normal1 = dir.getRotated(90, ofVec3f(0,0,1));
		ofVec3f normal2 = dir.getRotated(-90, ofVec3f(0,0,1));

		float thickness = 10;
		blobMesh.addVertex(source + normal1*thickness);
		blobMesh.addVertex(source + normal2*thickness);

	}

	blobMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
}

void blobGesture::clear(){
	trace.clear();
	blobMesh.clear();
}
