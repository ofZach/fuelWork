
#include "blobGesture.h"


void blobGesture::addGesture(ofVec3f pos){
	blobGesturePoint bgp;
	bgp.pos = pos;
	bgp.millis = ofGetElapsedTimeMillis();
	traceDebug.addVertex(bgp.pos);
	trace.push_back(bgp);
}

void blobGesture::buildMesh(){
	
	blobMesh.clear();
	blobMesh.addVertex(trace[0].pos);
	blobMesh.addVertex(trace[0].pos);

	ofPolyline p;
	for(int i = 1; i < trace.size(); i++){
//		p.addVertex( trace[i].pos );
		p.curveTo( trace[i].pos );
	}	

	ofPolyline resampled = p.getResampledBySpacing(5.0);
//	ofPolyline resampled = p;
	ofVec3f pastNormL;
	ofVec3f pastNormR;

	for(int i = 1; i < resampled.size(); i++){

		//ofVec3f source = trace[i].pos; 
		//ofVec3f past = trace[i-1].pos;

		ofVec3f source = resampled.getVertices()[i]; 
		ofVec3f past = resampled.getVertices()[i-1];

		ofVec3f delta = (source - past);
		ofVec3f dir	= delta.getNormalized();

		//int millisDelta = resampled[i].millis - resampled[i-1].millis;
		float distanceDelta = delta.length();
		ofVec3f normalL = dir.getRotated( 90, ofVec3f(0,0,1));
		ofVec3f normalR = dir.getRotated(-90, ofVec3f(0,0,1));

		//JG Trying to limit the max angle with limited success...
		/*
		if(i != 1){
			float maxNormalDelta = .5;
			float signedAngleL = RAD_TO_DEG*(atan2(pastNormL.y,pastNormL.x) - atan2f(normalL.y,normalL.x));
			float signedAngleR = RAD_TO_DEG*(atan2(pastNormR.y,pastNormR.x) - atan2f(normalR.y,normalR.x));
			//cout << "angle dif amounts " << abs(signedAngleL) -  pastNormL.angle(normalL) << endl;
			if(abs(signedAngleL) > maxNormalDelta ){
				float oldSignedL = signedAngleL;
				normalL = pastNormL.getRotated(maxNormalDelta * (signedAngleL < 0 ? 1 : -1), ofVec3f(0,0,1));
				float newSignedL = RAD_TO_DEG * (atan2(pastNormL.y,pastNormL.x) - atan2f(normalL.y,normalL.x));
			}
			if(abs(signedAngleR) > maxNormalDelta ){
				float oldSignedR = signedAngleR;
				normalR = pastNormR.getRotated(maxNormalDelta  * (signedAngleR < 0 ? 1 : -1), ofVec3f(0,0,1));
				//cout <<  oldSignedR << " vs " << RAD_TO_DEG * atan2( pastNormR.x*normalR.y - pastNormR.y*normalR.x, pastNormR.x*normalR.x + pastNormR.y*normalR.y ) << endl;
			}
		}
		pastNormL = normalL;
		pastNormR = normalR;
		*/

		//cout << "millis delta is " << millisDelta << " distance delta " << delta.length() << endl;
		
		float imageIndexL = ofMap(i, 0, resampled.size()-1, 0, leftSources.size()-1, true);
		float imageIndexR = ofMap(i, 0, resampled.size()-1, 0, rightSources.size()-1,true);

		float interpAlphaL = imageIndexL - floor(imageIndexL);
		float interpAlphaR = imageIndexR - floor(imageIndexR);
		
		//cout << "lerp amounts " << interpAlphaR << " " << interpAlphaL << endl;

		float thickMultL = ofLerp(leftSources[floor(imageIndexL)], leftSources[ceil(imageIndexL)], interpAlphaL);
		float thickMultR = ofLerp(rightSources[floor(imageIndexR)], rightSources[ceil(imageIndexR)], interpAlphaR);

		float leftThickness  = ofMap(leftSources[imageIndexL],  0, 1.0, 0, 100, true);
		float rightThickness = ofMap(rightSources[imageIndexR], 0, 1.0, 0, 100, true);

		ofVec2f pointLeft  = source + normalL * leftThickness;
		ofVec2f pointRight = source + normalR * rightThickness;

		blobMesh.addVertex(pointLeft);
		blobMesh.addVertex(pointRight);

		debugLines.addColor(ofFloatColor::white);
		debugLines.addVertex(source);

		debugLines.addColor(ofFloatColor::purple);
		debugLines.addVertex(pointLeft);

		debugLines.addColor(ofFloatColor::white);
		debugLines.addVertex(source);

		debugLines.addColor(ofFloatColor::green);
		debugLines.addVertex(pointRight);


	}
}

void blobGesture::loadBrushSource(string path){
	if(brushSource.loadImage(path)){
		leftSources.clear();
		rightSources.clear();
		leftSources.resize(brushSource.getWidth());
		rightSources.resize(brushSource.getWidth());

		for(int x = 0; x < brushSource.getWidth(); x++){

			for(int y = brushSource.getHeight()/2; y < brushSource.getHeight(); y++){
				rightSources[x] = 1.0;
				if(brushSource.getColor(x,y).getBrightness() > 200){
					rightSources[x] = ofMap(y, brushSource.getHeight()/2, brushSource.getHeight(), 0.0, 1.0);
					break;
				}
			}

			for(int y = brushSource.getHeight()/2; y >= 0; y--){
				leftSources[x] = 1.0;
				if(brushSource.getColor(x,y).getBrightness() > 200){
					leftSources[x] = ofMap(y, brushSource.getHeight()/2, 0, 0.0, 1.0);
					break;
				}
			}

		}

	}
}

void blobGesture::clear(){
	trace.clear();
	traceDebug.clear();
	blobMesh.clear();
	debugLines.clear();

	blobMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	traceDebug.setMode(OF_PRIMITIVE_LINE_STRIP);
	debugLines.setMode(OF_PRIMITIVE_LINES);
}
