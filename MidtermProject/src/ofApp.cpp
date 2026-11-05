#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(10, 10, 20);

    // Carregar l'audio
    song.load("tearinmyheart.mp3");
    song.setLoop(true);
    song.setVolume(0.8);
    song.play();

    // Interficie de control
    gui.setup("Audio Visualizer");

    gui.add(sensitivity.set(
        "Sensitivity", 3.0, 0.1, 10.0
    ));

    gui.add(circleSize.set(
        "Circle Size", 100.0, 20.0, 300.0
    ));
}

//--------------------------------------------------------------
void ofApp::update(){
    // Obtenir el volum de l'audio
    audioLevel = song.getPosition();

    // Suavitzar el moviment
    smoothedLevel +=
        (audioLevel - smoothedLevel) * 0.1;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    gui.draw();

    // Crear una visualitzacio reactiva
    float radius = circleSize * smoothedLevel;

    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

    ofNoFill();

    for (int i = 0; i < 20; i++) {

        float r = radius + i * 15;

        ofSetColor(
            100 + i * 5,
            150,
            255 - i * 5
        );

        ofSetLineWidth(2);
        ofDrawCircle(0, 0, r);
    }

    ofPopMatrix();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        if (song.isPlaying()) {
            song.stop();
        }
        else {
            song.play();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
