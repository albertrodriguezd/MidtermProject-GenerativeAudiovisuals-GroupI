#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofSoundPlayer song;
		ofxPanel gui;

		ofParameter<float> sensitivity;
		ofParameter<float> circleSize;

		float audioLevel = 0.0;
		float smoothedLevel = 0.0;

		// Forma del cercle
		int numPoints = 180;
		float baseRadius = 180.0;
		float time = 0.0;
};
