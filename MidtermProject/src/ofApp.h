#pragma once
#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();

    ofSoundPlayer song;

    static const int nBands = 128;
    float fftSmoothed[nBands];

    float audioLevel = 0;
    float smoothedLevel = 0;
};
