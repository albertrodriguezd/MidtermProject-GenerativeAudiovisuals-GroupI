#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	bool loadWavSamples(string filename);

    ofSoundPlayer song;
	ofxPanel gui;

    vector<float> audioSamples;
    float sampleRate = 44100;

    static const int numPoints = 180;
    static const int numBands = 128;

    float audioBands[numBands] = {};

    float audioLevel = 0;
    float smoothedLevel = 0;
    float time = 0;

    float baseRadius = 180;
};