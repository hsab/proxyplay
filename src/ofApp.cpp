#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
   ofBackground(255);
   ofSetCircleResolution(200);

   centerX = ofGetWidth() / 2;
   centerY = ofGetHeight() / 2;

   nikes.load("vids/Nikes.mp4");
   nikes.setLoopState(OF_LOOP_NORMAL);
   nikes.play();

   monk.load("vids/Nikes.mp4");
   monk.setLoopState(OF_LOOP_NORMAL);
   monk.play();
}

//--------------------------------------------------------------
void ofApp::update() {
   centerX = ofGetWidth() / 2;
   centerY = ofGetHeight() / 2;

   nikes.update();
   monk.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
   ofSetColor(gui->color);
   ofDrawCircle(ofGetWidth() * 0.5, ofGetWidth() * 0.5, gui->radius);
   ofSetColor(0);
   ofDrawBitmapString(ofGetFrameRate(), 20, 20);
}

void ofApp::drawVid(ofVideoPlayer& vid) {
   ofSetHexColor(0xFFFFFF);
   vid.draw(20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
   if (key == 'f')
      ofToggleFullscreen();
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
