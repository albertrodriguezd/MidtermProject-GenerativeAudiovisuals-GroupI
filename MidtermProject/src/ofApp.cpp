#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(10, 10, 20);

    // Carregar l'audio
    if (song.load("tearinmyheart.mp3")) {
        song.setLoop(true);
        song.setVolume(0.8);
        song.play();
    }
    else {
        ofLogError() << "No s'ha pogut carregar la cançó";
    }

    // Interficie de control
    /*gui.setup("Audio Visualizer");

    gui.add(sensitivity.set(
        "Sensitivity", 3.0, 0.1, 10.0
    ));

    gui.add(circleSize.set(
        "Circle Size", 100.0, 20.0, 300.0
    ));*/
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSoundUpdate();

    const int nBands = 64;

    // Obtenir les frequencies de la canco
    float* spectrum = ofSoundGetSpectrum(nBands);

    audioLevel = 0;

    if (spectrum != nullptr) {

        // Sumar les frequencies dels baixos
        for (int i = 0; i < 10; i++) {
            audioLevel += spectrum[i];
        }

        audioLevel /= 10.0;
    }

    // Amplificar el senyal
    audioLevel *= 500.0;

    // Suavitzar el moviment
    smoothedLevel +=
        (audioLevel - smoothedLevel) * 0.2;

    time += 0.02;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    /*gui.draw();*/

    // Crear una visualitzacio reactiva
    float radius = 60;

    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

    ofNoFill();
    ofSetLineWidth(3);

    ofSetColor(100, 180, 255);

    ofBeginShape();

    for (int i = 0; i < numPoints; i++) {

        // Angle de cada punt
        float angle = TWO_PI * i / numPoints;

        // Ondulació orgànica
        float wave1 = sin(angle * 5 + time * 2);
        float wave2 = sin(angle * 9 - time * 1.5);

        // Deformació segons el volum de la música
        float deformation =
            (wave1 * 0.7 + wave2 * 0.3)
            * smoothedLevel * 3.0;

        // Radi final
        float radius = baseRadius + deformation;

        // Coordenades
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        ofVertex(x, y);
    }

    ofEndShape(true);
    ofSetColor(255);
    ofDrawBitmapString(
        "Playing: " + ofToString(song.isPlaying()) +
        " | Audio: " + ofToString(smoothedLevel, 4),
        20, 30
    );
    ofPopMatrix();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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
