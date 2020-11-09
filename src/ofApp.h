#pragma once
// #ifndef USE_ENGINE_PDSP
// #define USE_ENGINE_PDSP
// #endif

#include "GuiApp.h"
#include "ofMain.h"
#include "ofxTitles.h"
#include "srtparser.h"

#ifdef USE_ENGINE_PDSP
#include "ofxMidi.h"
#include "ofxPDSP.h"
#include "synth.h"
#endif
class ofApp : public ofBaseApp
{
   enum ddVideos : short
   {
      DD_MONK = 0x01,
      DD_NIKES = 0x02,
      DD_EXPLO = 0x03
   };

public:
   shared_ptr<GuiApp> gui;
   ofVideoPlayer monk;
   ofVideoPlayer nikes;
   ofVideoPlayer explo;
   ofSoundPlayer sunless;

   int exploFrames = 4306;
   int exploDuration = 179.41667175292969;
   float exploSpeed = 1.0f;
   vector<int> exploShots{397, 1275, 1658, 2153, 3031, 3414, 3909};
   int exploPlayhead = 0;
   int centerX;
   int centerY;

   ofTrueTypeFont infoFont;

   SubtitleParserFactory *subParserFactory;
   SubtitleParser *parser;
   vector<SubtitleItem *> sub;
   ofxTitles mTitles;
   ofColor subColor;

   int col;
   int channel;

   int mode;

   ofParameter<float> smooth;
   void smoothCall(float &value);

   ofFbo waveplot;

#ifdef USE_ENGINE_PDSP
   pdsp::Engine engine;
   PolySynth synth;
#endif

   int camWidth;
   int camHeight;

   bool initSound = false;

#ifdef USE_ENGINE_PDSP
#ifdef USE_MIDI_KEYBOARD
   pdsp::midi::Keys keyboard;
   pdsp::midi::Input midiIn;
#else
   pdsp::ComputerKeyboard keyboard;
#endif
#endif

   //--------------------------------------------------------------
   void setup()
   {
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

      camWidth = explo.getWidth(); // try to grab at this size.
      camHeight = explo.getHeight();

      // ofLogNotice() << "Loading sound.";
      // sunless.load("vids/sunless.wav");
      // ofLogNotice() << "Sound loaded";
      // sunless.setLoop(true);
      // sunless.play();
      // sunless.setSpeed(0.3f);

      infoFont.load("verdana.ttf", 12, true, true);
      infoFont.setLineHeight(18.0f);
      infoFont.setLetterSpacing(1.037);

      subParserFactory = new SubtitleParserFactory(ofToDataPath("sub.srt", true));
      parser = subParserFactory->getParser();
      sub = parser->getSubtitles();

      mTitles.bindTime(1230);
      mTitles.loadFont("Tahoma.ttf", 20);
      setupSubtitle();
      mTitles.setLoopState(OF_LOOP_NORMAL);
      mTitles.play();

      ofBackground(0);

      channel = 0;
      col = 160; // col for getting pixels to wave
      mode = 0;
      waveplot.allocate(camHeight * 2 + 4, 170);

#ifdef USE_ENGINE_PDSP
      //patching-------------------------------
      keyboard.setPolyMode(8);

      int voicesNum = keyboard.getVoicesNumber();

      synth.datatable.setup(camHeight, camHeight); // as many samples as the webcam width
                                                   //synth.datatable.smoothing(0.5f);

      synth.setup(voicesNum);
      for (int i = 0; i < voicesNum; ++i)
      {
         // connect each voice to a pitch and trigger output
         keyboard.out_trig(i) >> synth.voices[i].in("trig");
         keyboard.out_pitch(i) >> synth.voices[i].in("pitch");
      }

      // patch synth to the engine
      synth.ch(0) >> engine.audio_out(0);
      synth.ch(1) >> engine.audio_out(1);
#endif

      smooth.set(0.3f);

      // audio / midi setup----------------------------
#ifdef USE_ENGINE_PDSP
#ifdef USE_MIDI_KEYBOARD
      midiIn.listPorts();
      midiIn.openPort(0); //set the right port !!!
      engine.addMidiController(keyboard, midiIn);
#endif
      engine.listDevices();
      engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
      engine.setup(44100, 512, 3);

      gui->soundGui.setup("", "settings.xml", 20, 1100);
      gui->soundGui.add(synth.ui);
#endif
   }

   void setupSubtitle()
   {
      int i = 1;

      for (SubtitleItem *element : sub)
      {
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
   void update()
   {
      centerX = ofGetWidth() / 2;
      centerY = ofGetHeight() / 2;

      if (gui->updateInApp)
      {
         float pct = gui->limitedViewWidth - gui->limitedViewerPosition;
         pct = pct / gui->limitedViewWidth;
         exploSpeed = (2 * pct - 1) * gui->exploParams.getInt("speedLimit");
         exploSpeed = -exploSpeed;
         explo.setSpeed(exploSpeed);
      }

      nikes.update();
      monk.update();
      explo.update();

#ifdef USE_ENGINE_PDSP
      if (explo.isFrameNew() && synth.datatable.ready())
      {
         initSound = true;
         ofPixels &pixels = explo.getPixels();

         // ------------------ GENERATING THE WAVE ----------------------

         // a pdsp::DataTable easily convert data to a waveform in real time
         // if you don't need to generate waves in real time but
         // interpolate between already stored waves pdsp::WaveTable is a better choice
         // for example if you want to convert an image you already have to a wavetable

         switch (mode)
         {
         case 0: // converting pixels to waveform samples
            synth.datatable.begin();
            for (int n = 0; n < camHeight; ++n)
            {
               float sample = ofMap(pixels.getData()[col * 3 + channel + n * 3 * camWidth], 0, 255, -0.5f, 0.5f);
               synth.datatable.data(n, sample);
            }
            synth.datatable.end(false);
            break; // remember, raw waveform could have DC offsets, we have filtered them in the synth using an hpf

         case 1: // converting pixels to partials for additive synthesis
            synth.datatable.begin();
            for (int n = 0; n < camHeight; ++n)
            {
               float partial = ofMap(pixels.getData()[col * 3 + channel + n * 3 * camWidth], 0, 255, 0.0f, 1.0f);
               synth.datatable.data(n, partial);
            }
            synth.datatable.end(true);
            break;
         }

         // ----------------- PLOTTING THE WAVEFORM ---------------------
         waveplot.begin();
         ofClear(0, 0, 0, 0);

         ofSetColor(255);
         ofDrawRectangle(1, 1, waveplot.getWidth() - 2, waveplot.getHeight() - 2);
         ofTranslate(2, 2);
         switch (mode)
         {
         case 0: // plot the raw waveforms
            ofBeginShape();
            for (int n = 0; n < camHeight; ++n)
            {
               float y = ofMap(pixels.getData()[col * 3 + channel + n * 3 * camWidth], 0, 255, camHeight, 0);
               ofVertex(n * 2, y);
            }
            ofEndShape();
            break;

         case 1: // plot the partials
            for (int n = 0; n < camHeight; ++n)
            {
               float partial = ofMap(pixels.getData()[col * 3 + channel + n * 3 * camWidth], 0, 255, 0.0f, 1.0f);
               int h = waveplot.getHeight() * partial;
               int y = waveplot.getHeight() - h;
               ofDrawLine(n * 2, y, n * 2, camHeight);
            }
            break;
         }
         waveplot.end();
      }
      if (initSound)
      {
         keyboard.keyPressed('a');
         keyboard.keyPressed('y');
      }
#endif
   }

   //--------------------------------------------------------------
   void draw()
   {
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

      // mTitles.drawCenter(ofGetWidth() / 2, ofGetWidth() / 2, ofGetHeight() - 100, subColor);
   }

   void drawVid(ofVideoPlayer &vid, ddVideos flag)
   {
      ofParameterGroup params;
      string prefix = "";
      switch (flag)
      {
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

   void drawExploControls(ofParameterGroup &params, int x, int y, int w)
   {
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
      for (auto shot : exploShots)
      {
         int shotX = ofMap(shot, 0, exploFrames, x, nX);
         ofDrawLine(shotX, nY - 4, shotX, nY + 4);
      }
      if (explo.getCurrentFrame() > 0)
         exploPlayhead = ofMap(explo.getCurrentFrame(), 0, exploFrames, x, nX);
      ofSetLineWidth(4.0f);
      ofDrawLine(exploPlayhead, nY - 6, exploPlayhead, nY + 6);
   }

   //--------------------------------------------------------------
   void keyPressed(int key)
   {
      if (key == 'f')
         ofToggleFullscreen();
      if (key == 's')
         gui->nextStyle();

#ifdef USE_ENGINE_PDSP
#ifndef USE_MIDI_KEYBOARD
      // sends key messages to ofxPDSPComputerKeyboard
      keyboard.keyPressed(key);
#endif
#endif
   }

   //--------------------------------------------------------------
   void keyReleased(int key)
   {
#ifdef USE_ENGINE_PDSP
#ifndef USE_MIDI_KEYBOARD
      // sends key messages to ofxPDSPComputerKeyboard
      keyboard.keyReleased(key);
#endif
#endif
   }

   //--------------------------------------------------------------
   void mouseMoved(int x, int y)
   {
      // int width = ofGetWidth();
      // float pct = (float)x / (float)width;
      // exploSpeed = (2 * pct - 1) * gui->exploParams.getInt("speedLimit");
      // explo.setSpeed(exploSpeed);
   }

   //--------------------------------------------------------------
   void mouseDragged(int x, int y, int button)
   {
   }

   //--------------------------------------------------------------
   void mousePressed(int x, int y, int button)
   {
   }

   //--------------------------------------------------------------
   void mouseReleased(int x, int y, int button)
   {
   }

   //--------------------------------------------------------------
   void mouseEntered(int x, int y)
   {
   }

   //--------------------------------------------------------------
   void mouseExited(int x, int y)
   {
   }

   //--------------------------------------------------------------
   void windowResized(int w, int h)
   {
   }

   //--------------------------------------------------------------
   void gotMessage(ofMessage msg)
   {
   }

   //--------------------------------------------------------------
   void dragEvent(ofDragInfo dragInfo)
   {
   }
};
