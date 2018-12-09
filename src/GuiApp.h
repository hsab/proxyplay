#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxImGui.h"
#include "ofxKinect.h"
#include "ofxPreset.h"
#include "ofxXmlSettings.h"

class GuiApp : public ofBaseApp {
  public:
   ofxImGui::Gui gui;

   int gOffset = 20;
   bool mouseOverGui;

   ofParameter<int> monkX;
   ofParameter<int> monkY;
   ofParameter<int> monkW;
   ofParameter<int> monkLO;
   ofParameter<int> monkRO;
   ofParameter<int> monkTO;
   ofParameter<int> monkBO;
   ofParameterGroup monkParams;

   ofParameter<int> nikesX;
   ofParameter<int> nikesY;
   ofParameter<int> nikesW;
   ofParameter<int> nikesLO;
   ofParameter<int> nikesRO;
   ofParameter<int> nikesTO;
   ofParameter<int> nikesBO;
   ofParameterGroup nikesParams;

   ofParameter<int> exploX;
   ofParameter<int> exploY;
   ofParameter<int> exploW;
   ofParameter<int> exploLO;
   ofParameter<int> exploRO;
   ofParameter<int> exploTO;
   ofParameter<int> exploBO;
   ofParameter<int> horizOffset;
   ofParameter<int> vertOffset;
   ofParameter<float> topTextOffset;
   ofParameter<float> bottomTextOffset;
   ofParameter<float> midTextOffset;
   ofParameter<int> speedLimit;
   ofParameterGroup exploParams;

   ofParameter<int> minX;
   ofParameter<int> maxX;
   ofParameter<int> minY;
   ofParameter<int> maxY;
   ofParameterGroup offsetParams;

   ofxXmlSettings settings;
   ofDirectory dir;
   vector<string> styles;
   int styleIndex = -1;

   ofxKinect kinect;
   ofImage colorImg;
   ofImage grayImage;       // grayscale depth image
   ofImage grayThreshNear;  // the near thresholded image
   ofImage grayThreshFar;   // the far thresholded image
   ofImage grayPreprocImage;
   ofxCv::ContourFinder contourFinder;

   ofParameter<int> angle;
   ofParameter<float> minArea, maxArea, threshold;
   ofParameter<float> leftLimit, rightLimit;
   ofParameter<float> nearThreshold, farThreshold;
   ofParameter<float> triggerThreshold;
   ofParameterGroup kinectParams;
   float viewerPosition = -1.0f;
   float newViewerPosition;
   float limitedViewerPosition;
   float limitedViewWidth;
   bool updateInApp = false;

   void setup() {
      ofSetEscapeQuitsApp(false);

      gui.setup();
      setupParams();
      setupKinect();

      loadStyles();

      if (settings.loadFile("init.xml")) {
         ofDeserialize(settings, monkParams);
         ofDeserialize(settings, nikesParams);
         ofDeserialize(settings, exploParams);
         ofDeserialize(settings, offsetParams);
      }

      // ofBackground(0);
      ofSetVerticalSync(false);
   }

   void loadStyles() {
      styles.clear();
      int nFiles = dir.listDir(ofToDataPath("settings", true));
      if (nFiles) {
         for (int i = 0; i < dir.size(); i++) {
            string filePath = dir.getPath(i);
            styles.push_back(filePath);
         }
      }
   }

   void setupKinect() {
      kinect.setRegistration(true);

      kinect.init(false, false);  // disable video image (faster fps)
      kinect.open();              // open a kinect using it's unique serial #

      // print the intrinsic IR sensor values
      if (kinect.isConnected()) {
         ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
         ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
         ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
         ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
      }

      kinect.setCameraTiltAngle(angle);

      colorImg.allocate(kinect.width, kinect.height, OF_IMAGE_COLOR);
      grayImage.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
      grayThreshNear.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
      grayThreshFar.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
      grayPreprocImage.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
   }

   void setupParams() {
      monkParams.clear();
      monkParams.setName("monkParams");
      monkParams.add(monkX.set("monkX", 0, -1920, 1920));
      monkParams.add(monkY.set("monkY", 0, -1920, 1920));
      monkParams.add(monkW.set("monkW", 540, 100, 1080));
      monkParams.add(monkLO.set("monkLO", 0, -540, 540));
      monkParams.add(monkRO.set("monkRO", 0, -540, 540));
      monkParams.add(monkTO.set("monkTO", 0, -540, 540));
      monkParams.add(monkBO.set("monkBO", 0, -540, 540));

      nikesParams.clear();
      nikesParams.setName("nikesParams");
      nikesParams.add(nikesX.set("nikesX", 0, -1920, 1920));
      nikesParams.add(nikesY.set("nikesY", 0, -1920, 1920));
      nikesParams.add(nikesW.set("nikesW", 540, 100, 1080));
      nikesParams.add(nikesLO.set("nikesLO", 0, -540, 540));
      nikesParams.add(nikesRO.set("nikesRO", 0, -540, 540));
      nikesParams.add(nikesTO.set("nikesTO", 0, -540, 540));
      nikesParams.add(nikesBO.set("nikesBO", 0, -540, 540));

      exploParams.clear();
      exploParams.setName("exploParams");
      exploParams.add(exploX.set("exploX", 0, -1920, 1920));
      exploParams.add(exploY.set("exploY", 0, -1920, 1920));
      exploParams.add(exploW.set("exploW", 540, 100, 1080));
      exploParams.add(exploLO.set("exploLO", 0, -540, 540));
      exploParams.add(exploRO.set("exploRO", 0, -540, 540));
      exploParams.add(exploTO.set("exploTO", 0, -540, 540));
      exploParams.add(exploBO.set("exploBO", 0, -540, 540));
      exploParams.add(horizOffset.set("horizOffset", 0, -50, 50));
      exploParams.add(vertOffset.set("vertOffset", 0, -50, 50));
      exploParams.add(topTextOffset.set("topTextOffset", 0, -50, 50));
      exploParams.add(bottomTextOffset.set("bottomTextOffset", 0, -50, 50));
      exploParams.add(midTextOffset.set("midTextOffset", 0, -50, 50));
      exploParams.add(speedLimit.set("speedLimit", 10, 1, 20));

      offsetParams.clear();
      offsetParams.setName("offsetParams");
      offsetParams.add(minX.set("minX", 0, -1920, 1920));
      offsetParams.add(maxX.set("maxX", 1920, -1920, 1920));
      offsetParams.add(minY.set("minY", 0, -1920, 1920));
      offsetParams.add(maxY.set("maxY", 1080, -1920, 1920));

      kinectParams.clear();
      kinectParams.setName("kinectParams");
      kinectParams.add(minArea.set("minArea", 50, 1, 5000));
      kinectParams.add(maxArea.set("maxArea", 5000, 1, 5000));
      kinectParams.add(threshold.set("thresholdArea", 0, 0, 255));
      kinectParams.add(nearThreshold.set("nearThreshold", 255, 0, 255));
      kinectParams.add(farThreshold.set("farThreshold", 160, 0, 255));
      kinectParams.add(triggerThreshold.set("triggerThreshold", 5, 0, 255));
      kinectParams.add(leftLimit.set("leftLimit", 80, 0, 1920));
      kinectParams.add(rightLimit.set("rightLimit", 160, 0, 1920));
      kinectParams.add(angle.set("angle", 0, -30, 30));
   }

   void update() {
      kinect.update();
      updateInApp = true;
      if (newViewerPosition >= leftLimit && newViewerPosition <= kinect.width - rightLimit) {
         if (viewerPosition < 0) {
            viewerPosition = newViewerPosition;
            limitedViewerPosition = viewerPosition;
            limitedViewerPosition = ofMap(limitedViewerPosition, 0, kinect.width, 0, kinect.width - rightLimit - leftLimit);
            cout << limitedViewerPosition << endl;
         }
         if (abs(newViewerPosition - viewerPosition) > triggerThreshold) {
            viewerPosition = newViewerPosition;
            limitedViewerPosition = viewerPosition;
            cout << "viewerPosition: " << viewerPosition << endl;
            cout << " data: " << kinect.width << " | " << leftLimit << " | " << rightLimit << endl;
            limitedViewerPosition -= leftLimit;
            limitedViewWidth = kinect.width - rightLimit - leftLimit;
            cout << "new: " << limitedViewerPosition << " | " << limitedViewWidth << endl
                 << endl;

            updateInApp = true;
            // limitedViewerPosition = ofMap(limitedViewerPosition, 0, kinect.width, 0, kinect.width - rightLimit - leftLimit);
            // cout << "IN UPDT limitedViewerPosition: " << limitedViewerPosition << endl;
            // cout << "IN UPDT upper limit: " << kinect.width - rightLimit - leftLimit << endl;
         }
      } else {
         limitedViewWidth = kinect.width - rightLimit - leftLimit;
         if (newViewerPosition <= leftLimit) {
            limitedViewerPosition = 0;
         }

         if (newViewerPosition >= rightLimit) {
            limitedViewerPosition = limitedViewWidth;
         }
      }

      if (kinect.isFrameNew()) {
         grayImage.setFromPixels(kinect.getDepthPixels());
         grayImage.update();
         ofxCv::threshold(grayImage, grayThreshNear, nearThreshold, true);
         ofxCv::threshold(grayImage, grayThreshFar, farThreshold);
         cv::Mat grayThreshNearMat = ofxCv::toCv(grayThreshNear);
         cv::Mat grayThreshFarMat = ofxCv::toCv(grayThreshFar);
         cv::Mat grayImageMat = ofxCv::toCv(grayImage);
         ofxCv::bitwise_and(grayThreshNearMat, grayThreshFarMat, grayImageMat);
         grayPreprocImage = grayImage;

         ofxCv::dilate(grayImage);
         ofxCv::dilate(grayImage);
         //erode(grayImage);

         // Mark image as changed
         grayImage.update();

         // Find contours
         contourFinder.setMinAreaRadius(minArea);
         contourFinder.setMaxAreaRadius(maxArea);
         contourFinder.setThreshold(threshold);
         contourFinder.findContours(grayImage);
         contourFinder.setFindHoles(false);
      }
   }

   void draw() {
      mouseOverGui = imGui();
      int leftMargin = 400;
      ofSetHexColor(0xffffff);
      kinect.drawDepth(leftMargin, gOffset, kinect.width, kinect.height);
      ofSetHexColor(0xff0000);

      ofDrawLine(leftMargin + leftLimit, gOffset, leftMargin + leftLimit, kinect.height + gOffset);
      ofDrawLine(leftMargin + kinect.width - rightLimit, gOffset, leftMargin + kinect.width - rightLimit, kinect.height + gOffset);

      grayImage.draw(leftMargin, kinect.height + gOffset * 2, kinect.width, kinect.height);

      ofPushMatrix();
      {
         ofTranslate(leftMargin, kinect.height + gOffset, 0);  // center the points a bit
                                                               // ofScale(0.625, 0.625, 1);  // 640x480 -> 400x300 = 0.625
         ofSetHexColor(0x00ff00);
         cv::Rect rc;
         int area = 0;
         int cx, cy, cw, ch;
         if (contourFinder.size() > 0) {
            for (int i = 0; i < contourFinder.size(); i++) {
               rc = contourFinder.getBoundingRect(i);
               if (rc.area() > area) {
                  area = rc.area();
                  cx = rc.x;       // cx = (rc.x > cx) ? rc.x : cx;
                  cy = rc.y;       // cy = (rc.y > cy) ? rc.y : cy;
                  cw = rc.width;   // cw = (rc.width < cw) ? rc.width : cw;
                  ch = rc.height;  // ch = (rc.height < ch) ? rc.height : ch;
               }
            }
            newViewerPosition = cx + cw / 2;
         }
         ofNoFill();
         ofRect(cx, cy, cw, ch);
         ofFill();
         ofDrawCircle(cx + cw / 2, cy + ch / 2, 10);
         // contourFinder.draw();
      }
      ofPopMatrix();

      ofLoadIdentityMatrix();
      ofSetHexColor(0xff0000);
      ofDrawLine(leftMargin + leftLimit, kinect.height + gOffset * 2, leftMargin + leftLimit, (kinect.height + gOffset) * 2);
      ofDrawLine(leftMargin + kinect.width - rightLimit, kinect.height + gOffset * 2, leftMargin + kinect.width - rightLimit, (kinect.height + gOffset) * 2);
   }

   bool imGui() {
      ofSetHexColor(0xffffff);

      auto guiSettings = ofxPreset::Gui::Settings();
      guiSettings.windowPos = ofVec2f(gOffset, gOffset);
      guiSettings.windowSize = ofVec2f(400 - gOffset * 2, 900 - gOffset * 2);

      gui.begin();
      gui.draw();
      {
         if (ofxPreset::Gui::BeginWindowFlags("Dissonance distance", guiSettings, false, NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("\n");

            ImGui::Columns(4, "buttons", false);
            if (ImGui::Button("SAVE", ImVec2(ImGui::GetColumnWidth(), 24.0f))) {
               if (styleIndex >= 0)
                  saveSettings(styles[styleIndex]);
            }

            ImGui::NextColumn();
            if (ImGui::Button("SAVE AS", ImVec2(ImGui::GetColumnWidth(), 24.0f)))
               saveSettings();

            ImGui::NextColumn();
            if (ImGui::Button("LOAD", ImVec2(ImGui::GetColumnWidth(), 24.0f)))
               loadSettings();

            ImGui::NextColumn();
            if (ImGui::Button("RESET", ImVec2(ImGui::GetColumnWidth(), 24.0f)))
               setupParams();

            ImGui::Text("\n");

            ImGui::NextColumn();
            if (ImGui::Button("DEFAULT", ImVec2(ImGui::GetColumnWidth(), 24.0f))) {
               loadSettings("init.xml");
               styleIndex = -1;
            }

            for (int i = 0; i < styles.size(); i++) {
               ImGui::NextColumn();
               string styleButton = ofFilePath::getBaseName(styles[i]);
               if (ImGui::Button(styleButton.c_str(), ImVec2(ImGui::GetColumnWidth(), 24.0f))) {
                  loadSettings(styles[i]);
                  styleIndex = i;
               }
            }

            ImGui::Columns(1);
            ImGui::Text("\n");

            ofxPreset::Gui::AddGroup(offsetParams, guiSettings);
            ofxPreset::Gui::AddGroup(exploParams, guiSettings);
            ofxPreset::Gui::AddGroup(monkParams, guiSettings);
            ofxPreset::Gui::AddGroup(nikesParams, guiSettings);
            ofxPreset::Gui::AddGroup(kinectParams, guiSettings);
         }
         ofxPreset::Gui::EndWindow(guiSettings);
      }

      gui.end();

      return guiSettings.mouseOverGui;
   }

   void saveSettings(string path = "") {
      settings.clear();
      ofSerialize(settings, monkParams);
      ofSerialize(settings, nikesParams);
      ofSerialize(settings, exploParams);
      ofSerialize(settings, offsetParams);
      ofSerialize(settings, kinectParams);

      if (path == "") {
         ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + ".xml", "Save Settings");
         if (saveFileResult.bSuccess) {
            settings.saveFile(saveFileResult.filePath);
            loadStyles();
         }
      } else {
         settings.saveFile(path);
      }
   }
   void loadSettings(string path = "") {
      string filePath = path;
      bool success = true;
      if (filePath == "") {
         ofFileDialogResult openFileResult = ofSystemLoadDialog("Load Settings", false, ofToDataPath("", true));
         if (openFileResult.bSuccess) {
            success = openFileResult.bSuccess;
            filePath = openFileResult.getPath();
         }
      }
      if (success) {
         settings.loadFile(filePath);
         ofDeserialize(settings, monkParams);
         ofDeserialize(settings, nikesParams);
         ofDeserialize(settings, exploParams);
         ofDeserialize(settings, offsetParams);
         ofDeserialize(settings, kinectParams);
      }
   }

   void nextStyle() {
      styleIndex++;
      if (styleIndex > (styles.size() - 1))
         styleIndex = 0;
      loadSettings(styles[styleIndex]);
   }
};
