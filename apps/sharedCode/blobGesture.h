
#include "ofMain.h"

//-- capture timing in resampled stroke
//-- create a brush head
//-- stroke threshold


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

	ofMesh generateAnimatedMesh(float percent);
	int strokeStartTime;
	int strokeDuration;
	void loadBrushSource(string path);
	ofImage brushSource;
	vector<float> timingCurve;
	vector<float> leftSources;
	vector<float> rightSources;

};
