#pragma once

#include "GuiApp.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
   enum ddVideos : short {
      DD_MONK = 0x01,
      DD_NIKES = 0x02,
      DD_EXPLO = 0x03
   };

  public:
   void setup();
   void update();
   void draw();

   void keyPressed(int key);
   void keyReleased(int key);
   void mouseMoved(int x, int y);
   void mouseDragged(int x, int y, int button);
   void mousePressed(int x, int y, int button);
   void mouseReleased(int x, int y, int button);
   void mouseEntered(int x, int y);
   void mouseExited(int x, int y);
   void windowResized(int w, int h);
   void dragEvent(ofDragInfo dragInfo);
   void gotMessage(ofMessage msg);

   void drawVid(ofVideoPlayer& vid, ddVideos flag);

   shared_ptr<GuiApp> gui;
   ofVideoPlayer monk;
   ofVideoPlayer nikes;
   ofVideoPlayer explo;

   int centerX;
   int centerY;
};
