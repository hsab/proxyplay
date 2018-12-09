#include "GuiApp.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"
#include "ofMain.h"

//========================================================================
int main() {
   ofGLFWWindowSettings settings;

   settings.setSize(1920, 1080);
   settings.setPosition(ofVec2f(1300, 0));
   settings.resizable = true;
   shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

   settings.setSize(1300, 900);
   settings.setPosition(ofVec2f(0, 0));
   settings.resizable = false;
   shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);

   shared_ptr<ofApp> mainApp(new ofApp);
   shared_ptr<GuiApp> guiApp(new GuiApp);
   mainApp->gui = guiApp;

   ofRunApp(guiWindow, guiApp);
   ofRunApp(mainWindow, mainApp);
   ofRunMainLoop();
}
