#include "ofApp.h"
#include <fstream>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <algorithm>

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(10, 10, 20);
    // Carregar les mostres del WAV
    loadWavSamples("tearinmyheart.wav");

    // Reproduir la cançó
    if (song.load("tearinmyheart.wav")) {
        song.setLoop(true);
        song.setVolume(0.7);
        song.play();
    }

    for (int i = 0; i < numBands; i++) {
        audioBands[i] = 0;
        bassBands[i] = 0;
    }
    // Carregar l'audio
    /*if (song.load("tearinmyheart.wav")) {*/
   /*     song.setLoop(true);
        song.setVolume(0.8);
        ofSoundUpdate();
        song.play();*/
    /*}*/
    /*else {
        ofLogError() << "No s'ha pogut carregar la cançó";
    }*/

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
void ofApp::update() {
    ofSoundUpdate();

    size_t windowSize = 4096;

    if (audioSamples.size() < windowSize || !song.isPlaying()) {
        return;
    }

    // Posició actual de la cançó
    float position = song.getPosition();

    size_t start = position *
        (audioSamples.size() - windowSize);

    start = std::min(start, audioSamples.size() - windowSize);

    float cutoffBass = 200.0f;
    float alphaBass = exp(-TWO_PI * cutoffBass / sampleRate);

    float previousY_Bass = 0;
    vector<float> bassFiltered(windowSize);

    // Filtre passa-baix
    for (size_t i = 0; i < windowSize; i++) {
        float x = audioSamples[start + i];

        float y = alphaBass * previousY_Bass
            + (1.0f - alphaBass) * x;

        bassFiltered[i] = y;
        previousY_Bass = y;
    }

    // Dividir els greus en segments
    int bassSegmentSize = windowSize / numBands;

    float maxBassEnergy = 0;

    for (int b = 0; b < numBands; b++) {

        float energy = 0;

        for (int j = 0; j < bassSegmentSize; j++) {
            float value = bassFiltered[b * bassSegmentSize + j];
            energy += value * value;
        }

        energy = sqrt(energy / bassSegmentSize);

        float target = ofClamp(energy * 12.0f, 0.0f, 1.0f);

        float speed = target > bassBands[b] ? 0.6f : 0.15f;

        bassBands[b] += (target - bassBands[b]) * speed;

        maxBassEnergy = max(maxBassEnergy, bassBands[b]);
    }

    bassLevel = maxBassEnergy;
    smoothedBass += (bassLevel - smoothedBass) * 0.2f;

    // Filtre passa-alts: conservar freqüències superiors
    // aproximadament a 2500 Hz
    float cutoff = 2500.0f;

    float alpha = exp(
        -TWO_PI * cutoff / sampleRate
    );

    float previousX = audioSamples[start];
    float previousY = 0;

    vector<float> filtered(windowSize);

    // Filtrar les mostres
    for (size_t i = 0; i < windowSize; i++) {

        float x = audioSamples[start + i];

        float y = alpha * (previousY + x - previousX);

        filtered[i] = y;

        previousX = x;
        previousY = y;
    }

    // Dividir l'àudio en 128 segments
    int segmentSize = windowSize / numBands;

    float maxEnergy = 0;

    for (int b = 0; b < numBands; b++) {

        float energy = 0;

        for (int j = 0; j < segmentSize; j++) {

            float value = filtered[b * segmentSize + j];

            energy += value * value;
        }

        energy = sqrt(energy / segmentSize);

        // Amplificar la resposta
        float target = ofClamp(energy * 18.0f, 0, 1);

        // Resposta ràpida i retorn suau
        float speed = target > audioBands[b] ? 0.6 : 0.15;

        audioBands[b] +=
            (target - audioBands[b]) * speed;

        maxEnergy = max(maxEnergy, audioBands[b]);
    }

    audioLevel = maxEnergy;

    smoothedLevel +=
        (audioLevel - smoothedLevel) * 0.2;

    time += 0.02;
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);

    ofPushMatrix();

    ofTranslate(
        ofGetWidth() / 2,
        ofGetHeight() / 2
    );

    ofNoFill();
    ofSetLineWidth(3);
    ofSetColor(255, 32, 25);

    ofBeginShape();

    for (int i = 0; i < numPoints; i++) {

        float angle = TWO_PI * i / numPoints;

        // Assignar una banda a cada punt
        int band = ofMap(
            i, 0, numPoints - 1,
            0, numBands - 1
        );

        band = ofClamp(band, 0, numBands - 1);

        // Intensitat dels aguts
        float amplitude = audioBands[band];

        // Crear deformació
        float wave1 = sin(angle * 5 + time * 2);
        float wave2 = sin(angle * 9 - time * 1.5);

        float organicWave =
            (wave1 * 0.7 + wave2 * 0.3) * 12.0;

        float deformation = amplitude * 100.0;

        float radius =
            baseRadius + deformation + organicWave;

        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        ofVertex(x, y);
    }

    ofEndShape(true);

    ofNoFill();
    ofSetLineWidth(3);
    ofSetColor(255);

    ofBeginShape();

    for (int i = 0; i < numPoints; i++) {

        float angle = TWO_PI * i / numPoints;

        // Assignar una banda de greus a cada punt
        int band = ofMap(
            i, 0, numPoints - 1,
            0, numBands - 1
        );

        band = ofClamp(band, 0, numBands - 1);

        // Intensitat dels greus
        float amplitude = bassBands[band];

        // Deformacions orgàniques
        float wave1 = sin(angle * 5 + time * 2);
        float wave2 = sin(angle * 9 - time * 1.5);

        float organicWave =
            (wave1 * 0.7f + wave2 * 0.3f) * 8.0f;

        // Deformació segons els greus
        float deformation = amplitude * 65.0f;

        float radius =
            bassRadius + deformation + organicWave;

        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        ofVertex(x, y);
    }

    ofEndShape(true);

    ofPopMatrix();

    // Diagnòstic
    ofSetColor(255);

    ofDrawBitmapString(
        "Playing: " + ofToString(song.isPlaying()) +
        "\nHigh frequencies: " + ofToString(audioLevel, 4),
        20, 30
    );
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

bool ofApp::loadWavSamples(string filename) {

    string path = ofToDataPath(filename, true);

    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        ofLogError() << "No s'ha pogut obrir el WAV";
        return false;
    }

    vector<uint8_t> bytes(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    if (bytes.size() < 12 ||
        memcmp(bytes.data(), "RIFF", 4) != 0 ||
        memcmp(bytes.data() + 8, "WAVE", 4) != 0) {
        ofLogError() << "Fitxer WAV no valid";
        return false;
    }

    uint16_t format = 0;
    uint16_t channels = 0;
    uint16_t bits = 0;

    uint32_t rate = 0;
    size_t dataOffset = 0;
    size_t dataSize = 0;

    auto read16 = [&](size_t p) {
        return uint16_t(bytes[p] | (bytes[p + 1] << 8));
        };

    auto read32 = [&](size_t p) {
        return uint32_t(bytes[p])
            | (uint32_t(bytes[p + 1]) << 8)
            | (uint32_t(bytes[p + 2]) << 16)
            | (uint32_t(bytes[p + 3]) << 24);
        };

    // Llegir els blocs del WAV
    size_t pos = 12;

    while (pos + 8 <= bytes.size()) {

        uint32_t chunkSize = read32(pos + 4);
        size_t chunkData = pos + 8;

        if (chunkData + chunkSize > bytes.size()) {
            break;
        }

        if (memcmp(bytes.data() + pos, "fmt ", 4) == 0) {

            if (chunkSize < 16) return false;

            format = read16(chunkData);
            channels = read16(chunkData + 2);
            rate = read32(chunkData + 4);
            bits = read16(chunkData + 14);
        }

        if (memcmp(bytes.data() + pos, "data", 4) == 0) {
            dataOffset = chunkData;
            dataSize = chunkSize;
        }

        pos = chunkData + chunkSize + (chunkSize % 2);
    }

    // Comprovar que és PCM de 16 bits
    if (format != 1 || bits != 16 ||
        channels == 0 || rate == 0 || dataSize == 0) {

        ofLogError() << "El WAV ha de ser PCM de 16 bits";
        return false;
    }

    sampleRate = (float)rate;

    size_t bytesPerFrame = channels * 2;
    size_t frames = dataSize / bytesPerFrame;

    audioSamples.clear();
    audioSamples.reserve(frames);

    // Convertir l'àudio a mono
    for (size_t i = 0; i < frames; i++) {

        float sample = 0;

        for (int c = 0; c < channels; c++) {

            size_t p = dataOffset + i * bytesPerFrame + c * 2;

            int16_t value = (int16_t)read16(p);

            sample += value / 32768.0f;
        }

        sample /= channels;

        audioSamples.push_back(sample);
    }

    ofLogNotice() << "WAV carregat correctament";
    ofLogNotice() << "Mostres: " << audioSamples.size();

    return true;
}