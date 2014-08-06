
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

	ofMesh traceDebug;
	ofMesh blobMesh;
	ofMesh debugLines;

	void loadBrushSource(string path);
	ofImage brushSource;
	vector<float> leftSources;
	vector<float> rightSources;

};
