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

   ofBackground(0);
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
   // ofSetColor(gui->color);
   // ofDrawCircle(ofGetWidth() * 0.5, ofGetWidth() * 0.5, gui->radius);

   drawVid(nikes, DD_NIKES);
}

void ofApp::drawVid(ofVideoPlayer& vid, ddVideos flag) {
   ofParameterGroup params;
   string prefix = "";
   switch (flag) {
      case DD_MONK:
         params = gui->monkParams;
         prefix = "monk";
         break;
      case DD_NIKES:
         params = gui->nikesParams;
         prefix = "nikes";
         break;
      case DD_EXPLO:
         params = gui->exploParams;
         prefix = "explo";
         break;
   }

   // cout << gui->allParams.get("nikesParams").get("nikesX") << endl;
   int x = params.getInt(prefix + "X") + params.getInt(prefix + "LO") + params.getInt(prefix + "RO");
   int y = params.getInt(prefix + "Y") + params.getInt(prefix + "TO") + params.getInt(prefix + "BO");
   int w = params.getInt(prefix + "W");
   vid.draw(x, y, w, w);
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
