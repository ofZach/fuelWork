
#include "ofMain.h"

class blobGesturePoint {
  public:
	  int millis;
	  ofVec3f pos;
};

class blobGesture {  
  public:
	void addGesture(ofVec3f pos);
	vector<blobGesturePoint> trace;

	void buildMesh();
	void clear();

	ofMesh blobMesh;

};
