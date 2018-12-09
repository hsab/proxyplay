#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
   ofSetEscapeQuitsApp(false);

   ofBackground(255);
   ofSetCircleResolution(200);

   centerX = ofGetWidth() / 2;
   centerY = ofGetHeight() / 2;

   nikes.load("vids/Nikes.mp4");
   nikes.setLoopState(OF_LOOP_NORMAL);
   nikes.play();

   monk.load("vids/Monk.mp4");
   monk.setLoopState(OF_LOOP_NORMAL);
   monk.play();

   explo.load("vids/Explo.mov");
   explo.setLoopState(OF_LOOP_NORMAL);
   explo.play();

   ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update() {
   centerX = ofGetWidth() / 2;
   centerY = ofGetHeight() / 2;

   nikes.update();
   monk.update();
   explo.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
   // ofSetColor(gui->color);
   // ofDrawCircle(ofGetWidth() * 0.5, ofGetWidth() * 0.5, gui->radius);

   ofPushMatrix();
   int canvasW = gui->offsetParams.getInt("maxX") - gui->offsetParams.getInt("minX");
   int canvasY = gui->offsetParams.getInt("maxY") - gui->offsetParams.getInt("minY");
   ofTranslate((ofGetWidth() - canvasW) / 2, (ofGetHeight() - canvasY) / 2);
   {
      drawVid(monk, DD_MONK);
      drawVid(nikes, DD_NIKES);
      drawVid(explo, DD_EXPLO);
   }
   ofPopMatrix();
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
   ofSetHexColor(0xffffff);
   vid.draw(x, y, w, w);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
   if (key == 'f')
      ofToggleFullscreen();
   if (key == 's')
      gui->nextStyle();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
   int width = ofGetWidth();
   float pct = (float)x / (float)width;
   float speed = (2 * pct - 1) * 10.0f;
   explo.setSpeed(speed);
   cout << speed << endl;
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
