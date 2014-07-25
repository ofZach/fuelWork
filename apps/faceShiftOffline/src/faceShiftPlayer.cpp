
#include "faceShiftPlayer.h"



void faceShiftPlayer::parseFrames( string fileName ){
    vector < string > lines ;
    lines = ofSplitString(ofBufferFromFile(fileName), "\n", true, true);
    for (int i = 0; i < lines.size(); i++){
        faceShiftFrame f;
        f.frameString = lines[i];
        f.frameTimeMillis = parseOutTime(lines[i] );
        f.frameNum = i;
        frames.push_back(f);
    }
    
    //cout << lines.size() << endl;
    //std::exit(0);
    
}


float faceShiftPlayer::parseOutTime(string line){
    vector < string > tokens = ofSplitString(line, " ");
    for (int i = 0; i < tokens.size(); i++){
        if (tokens[i] == "I"){
            float time = ofToFloat(tokens[ i + 1 ]);
            return time;
        }
    }
}


faceShiftFrame faceShiftPlayer::getLineForTimeMillis( float millis, bool bWrap){
    if (bWrap == true){
        int millisInt = (int)millis;
        millisInt %= (int)getDurationMillis();
        millis = millisInt;
    }
    
    int which = 0;
    for (int i = 0; i < frames.size(); i++){
        
        //cout << millis << " " <<frames[i].frameTimeMillis << endl;
        if (millis > frames[i].frameTimeMillis){
            which = i;
        }
    }
    return frames[which];
}

faceShiftFrame faceShiftPlayer::getLineForTimeFloat( float t, bool wrap ){
    return getLineForTimeMillis(t * 1000.0);
}

