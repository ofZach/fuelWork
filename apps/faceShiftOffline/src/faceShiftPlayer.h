
#include "ofMain.h"
#include "ofxFaceShift.h"

typedef struct {
    float   frameTimeMillis;
    string  frameString; // ASCII REP of this string
    int     frameNum;
} faceShiftFrame;

class faceShiftPlayer {
    
public:
    
    void parseFrames( string fileName );
    float parseOutTime( string line );
    vector < faceShiftFrame > frames;
    
    float getDurationMillis( ){
        return frames[frames.size()-1].frameTimeMillis + (1.0/30.0) * 1000.0;  // add a little for the last frame;
    }
    
    float getDurationFloat( ){
        return (frames[frames.size()-1].frameTimeMillis + (1.0/30.0) * 1000.0) / 1000.0;  // add a little for the last frame;
    }
    
    faceShiftFrame getLineForTimeMillis( float millis, bool bWrap = false);     // wrap over on loop
    faceShiftFrame getLineForTimeFloat( float t, bool bWrap = false);
};
