
#include "blobGesture.h"


void blobGesture::addGesture(ofVec3f pos){
	blobGesturePoint bgp;
	bgp.pos = pos;
	bgp.millis = ofGetElapsedTimeMillis() - strokeStartTime;
	traceDebug.addVertex(bgp.pos);
	trace.push_back(bgp);
}

void blobGesture::buildMesh(){
	
	blobMesh.clear();
	blobMesh.addVertex(trace[0].pos);
	blobMesh.addVertex(trace[0].pos);
	blobMesh.addTexCoord(trace[0].pos);
	blobMesh.addTexCoord(trace[0].pos);
	blobMesh.addNormal(ofVec3f());
	blobMesh.addNormal(ofVec3f());

	ofPolyline p;
	for(int i = 0; i < trace.size(); i++){
		p.curveTo( trace[i].pos );
	}	

	ofPolyline resampled = p.getResampledBySpacing(5.0);
	//build timing curve
	//we need to retroactively apply the timing info from the base stroke
	//into the reampled curve. Do this by stepping through the timed points 
	//in the stroke and applying them through the curve
	int curCurveIndex = 0;
	timingCurve.clear();
	timingCurve.resize(resampled.size());
	for(int i = 1; i < trace.size(); i++){
		float lastDist = FLT_MAX;
		float deltaDist = -10;
		int startCurveIndex = curCurveIndex;   
		
		cout << "SAMPLE " << i << " TIME " << trace[i-1].millis << " TO " << trace[i].millis << endl;
		while(curCurveIndex < resampled.size()){
			float dist = trace[i].pos.distance( resampled.getVertices()[curCurveIndex] );
			deltaDist = dist - lastDist;
			if(deltaDist > 0){
				break;
			}
			curCurveIndex++;
			lastDist = dist;
		}
		//curCurveIndex--;
		cout << "	COVERS CURVE " << startCurveIndex << " TO " << curCurveIndex << endl;

		//now reassign all the times based on their linear position along the curve
		int curveSteps = curCurveIndex - startCurveIndex;
		int startMillis = trace[i-1].millis;
		int endMillis = trace[i].millis;
		//normalized timing
		for(int c = startCurveIndex; c < curCurveIndex; c++){
			timingCurve[c] = ofMap( c, startCurveIndex, curCurveIndex,startMillis,endMillis) / trace.back().millis;
			cout << "	TIME FOR INDEX " << c << " IS " << timingCurve[c] << endl;
		}
	}
	
	ofVec3f pastNormL;
	ofVec3f pastNormR;
	for(int i = 1; i < resampled.size(); i++){

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
		
		//POSITION BASED TIMING
		//float percentAlongCurve = (1.0*i) / (resampled.size()-1);
		//MILLISECOND BASED TIMING

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

		blobMesh.addTexCoord( ofVec2f(source.x,source.y) );
		blobMesh.addTexCoord( ofVec2f(source.x,source.y) );

		float percentAlongCurve = timingCurve[i];
		blobMesh.addNormal( ofVec3f(percentAlongCurve,0,0) );
		blobMesh.addNormal( ofVec3f(percentAlongCurve,0,0) );

		debugLines.addColor(ofFloatColor::white*percentAlongCurve);
		debugLines.addVertex(source);
		debugLines.addColor(ofFloatColor::purple);
		debugLines.addVertex(pointLeft);
		debugLines.addColor(ofFloatColor::white*percentAlongCurve);
		debugLines.addVertex(source);
		debugLines.addColor(ofFloatColor::green);
		debugLines.addVertex(pointRight);
	}
}

ofMesh blobGesture::generateAnimatedMesh(float percent){
	ofMesh m;
	//percent = ofMap(percent, 0, 1.0, 0, 1.0 + .1, true);
	for(int i = 0; i < blobMesh.getNumVertices(); i++){
		if(blobMesh.getNormals()[i].x < percent){
			float expand = powf(ofMap(blobMesh.getNormals()[i].x, percent-.1,percent, 1.0, 0.0, true), 2.0);
			expand = 1.0;
			m.addVertex( ofVec3f(blobMesh.getTexCoord(i).getInterpolated( blobMesh.getVertex(i), expand)) );
		}
	}

	m.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	return m;
}

void blobGesture::loadBrushSource(string path){
	if(brushSource.loadImage(path)){

		strokeDuration = ofGetElapsedTimeMillis() - strokeStartTime;
		
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

	strokeStartTime = ofGetElapsedTimeMillis();
}
