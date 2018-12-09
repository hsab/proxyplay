#pragma once

#include "GuiApp.h"
#include "ofMain.h"
#include "ofxTitles.h"
#include "srtparser.h"

class ofApp : public ofBaseApp {
   enum ddVideos : short {
      DD_MONK = 0x01,
      DD_NIKES = 0x02,
      DD_EXPLO = 0x03
   };

  public:
   shared_ptr<GuiApp> gui;
   ofVideoPlayer monk;
   ofVideoPlayer nikes;
   ofVideoPlayer explo;

   int exploFrames = 4306;
   int exploDuration = 179.41667175292969;
   float exploSpeed = 1.0f;
   vector<int> exploShots{397, 1275, 1658, 2153, 3031, 3414, 3909};
   int exploPlayhead = 0;
   int centerX;
   int centerY;

   ofTrueTypeFont infoFont;

   SubtitleParserFactory* subParserFactory;
   SubtitleParser* parser;
   vector<SubtitleItem*> sub;
   ofxTitles mTitles;
   ofColor subColor;

   //--------------------------------------------------------------
   void setup() {
      ofSetEscapeQuitsApp(false);

      // subColor = ofColor(255, 245, 0);
      subColor = ofColor(255, 255, 255);

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

      infoFont.load("verdana.ttf", 12, true, true);
      infoFont.setLineHeight(18.0f);
      infoFont.setLetterSpacing(1.037);

      subParserFactory = new SubtitleParserFactory(ofToDataPath("sub.srt", true));
      parser = subParserFactory->getParser();
      sub = parser->getSubtitles();

      mTitles.bindTime(2820);
      mTitles.loadFont("Tahoma.ttf", 20);
      setupSubtitle();
      mTitles.setLoopState(OF_LOOP_NORMAL);
      mTitles.play();

      ofBackground(0);
   }

   void setupSubtitle() {
      int i = 1;

      for (SubtitleItem* element : sub) {
         // cout << "BEGIN" << endl;
         // cout << "start : " << element->getStartTime() << endl;
         // cout << "end : " << element->getEndTime() << endl;
         // cout << "text : " << element->getText() << endl;
         // cout << "END" << endl
         //      << endl;
         mTitles.add(element->getText(), i, element->getStartTime(), element->getEndTime());
         i++;
      }
   }

   //--------------------------------------------------------------
   void update() {
      centerX = ofGetWidth() / 2;
      centerY = ofGetHeight() / 2;

      nikes.update();
      monk.update();
      explo.update();
   }

   //--------------------------------------------------------------
   void draw() {
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

      mTitles.drawCenter(ofGetWidth() / 2, ofGetWidth() / 2, ofGetHeight() - 100, subColor);
   }

   void drawVid(ofVideoPlayer& vid, ddVideos flag) {
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
      if (flag == DD_EXPLO)
         drawExploControls(params, x, y, w);
   }

   void drawExploControls(ofParameterGroup& params, int x, int y, int w) {
      int vertOffset = params.getInt("vertOffset");
      int horizOffset = params.getInt("horizOffset");
      int nX = x + w + vertOffset;
      int nY = y + w + horizOffset;

      ofSetLineWidth(1.0f);
      ofDrawLine(nX, y, nX, w + horizOffset);
      int speedLimit = params.getInt("speedLimit");
      int speedCircleY = ofMap(exploSpeed, -speedLimit, speedLimit, w, 0);

      ofSetLineWidth(2.0f);
      ofDrawLine(nX - 4, y, nX + 4, y);
      // ofDrawLine(nX - 4, w, nX + 4, w);
      ofDrawLine(nX - 4, w / 2, nX + 4, w / 2);

      std::stringstream ss;
      ss << std::fixed << std::setprecision(2) << speedLimit;
      ofBitmapFont f;
      ofRectangle rect = f.getBoundingBox(ss.str(), 0, 0);
      ofDrawBitmapString("  " + ss.str() + "x", nX + 4, y + rect.height / 2 + params.getFloat("topTextOffset"));
      ofDrawBitmapString(" -" + ss.str() + "x", nX + 4, w - rect.height / 2 + params.getFloat("bottomTextOffset"));
      ofDrawBitmapString("  0x", nX + 4, (w / 2) + (rect.height / 2) + +params.getFloat("midTextOffset"));

      ofSetLineWidth(4.0f);
      ofDrawLine(nX - 6, speedCircleY, nX + 6, speedCircleY);

      ofSetLineWidth(1.0f);

      nX = nX - vertOffset;
      ofDrawLine(x, nY, nX + vertOffset, nY);
      ofSetLineWidth(2.0f);
      ofDrawLine(x, nY - 4, x, nY + 4);
      // ofDrawLine(nX, nY - 4, nX, nY + 4);
      ofSetLineWidth(1.0f);
      for (auto shot : exploShots) {
         int shotX = ofMap(shot, 0, exploFrames, x, nX);
         ofDrawLine(shotX, nY - 4, shotX, nY + 4);
      }
      if (explo.getCurrentFrame() > 0)
         exploPlayhead = ofMap(explo.getCurrentFrame(), 0, exploFrames, x, nX);
      ofSetLineWidth(4.0f);
      ofDrawLine(exploPlayhead, nY - 6, exploPlayhead, nY + 6);
   }

   //--------------------------------------------------------------
   void keyPressed(int key) {
      if (key == 'f')
         ofToggleFullscreen();
      if (key == 's')
         gui->nextStyle();
   }

   //--------------------------------------------------------------
   void keyReleased(int key) {
   }

   //--------------------------------------------------------------
   void mouseMoved(int x, int y) {
      int width = ofGetWidth();
      float pct = (float)x / (float)width;
      exploSpeed = (2 * pct - 1) * gui->exploParams.getInt("speedLimit");
      explo.setSpeed(exploSpeed);
   }

   //--------------------------------------------------------------
   void mouseDragged(int x, int y, int button) {
   }

   //--------------------------------------------------------------
   void mousePressed(int x, int y, int button) {
   }

   //--------------------------------------------------------------
   void mouseReleased(int x, int y, int button) {
   }

   //--------------------------------------------------------------
   void mouseEntered(int x, int y) {
   }

   //--------------------------------------------------------------
   void mouseExited(int x, int y) {
   }

   //--------------------------------------------------------------
   void windowResized(int w, int h) {
   }

   //--------------------------------------------------------------
   void gotMessage(ofMessage msg) {
   }

   //--------------------------------------------------------------
   void dragEvent(ofDragInfo dragInfo) {
   }
};
