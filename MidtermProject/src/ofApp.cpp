#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(10, 10, 15);
    ofSetCircleResolution(180);

    // Carregar i reproduir la cançó
    song.load("tearinmyheart.mp3");
    song.setLoop(true);
    song.setVolume(0.8);
    song.play();

    // Inicialitzar l'espectre
    for (int i = 0; i < nBands; i++) {
        fftSmoothed[i] = 0;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    // Actualitzar el reproductor
    ofSoundUpdate();

    // Comprovar que la cançó està carregada i reproduint-se
    if (!song.isLoaded()) {
        return;
    }

    // Obtenir l'espectre de freqüències
    float* spectrum = ofSoundGetSpectrum(nBands);

    if (spectrum != nullptr) {

        audioLevel = 0;

        for (int i = 0; i < nBands; i++) {

            // Suavitzar les freqüències
            fftSmoothed[i] *= 0.85;

            if (spectrum[i] > fftSmoothed[i]) {
                fftSmoothed[i] = spectrum[i];
            }

            // Detectar els baixos
            if (i < 20) {
                audioLevel += spectrum[i];
            }
        }

        audioLevel /= 20.0;
    }

    // Suavitzar la intensitat general
    smoothedLevel += (audioLevel - smoothedLevel) * 0.3;
}

//--------------------------------------------------------------
void ofApp::draw(){
   ofBackground(10, 10, 15);

    float centerX = ofGetWidth() / 2;
    float centerY = ofGetHeight() / 2;

    float baseRadius = 180;

    // Dibuixar el cercle
    ofNoFill();
    ofSetLineWidth(3);
    ofSetColor(100, 220, 255);

    ofBeginShape();

    int points = 180;

    for (int i = 0; i < points; i++) {

        // Angle de cada punt
        float angle = ofMap(i, 0, points, 0, TWO_PI);

        // Assignar una freqüència a cada punt
        int band = ofMap(i, 0, points - 1, 1, 40);
        band = ofClamp(band, 0, nBands - 1);

        // Intensitat de la freqüència
        float amplitude = fftSmoothed[band];

        // Multiplicar per fer més visible la deformació
        float deformation = amplitude * 700;

        // Calcular el radi final
        float radius = baseRadius + deformation;

        // Posició del punt
        float x = centerX + cos(angle) * radius;
        float y = centerY + sin(angle) * radius;

        ofVertex(x, y);
    }

    ofEndShape(true);

    // Indicador temporal per comprovar l'àudio
    ofSetColor(255);
    ofDrawBitmapString(
        "Playing: " + ofToString(song.isPlaying()) +
        " | Audio: " + ofToString(smoothedLevel, 6),
        20, 30
    );
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
