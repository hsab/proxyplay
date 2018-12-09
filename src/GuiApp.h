#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxImGui.h"
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
   ofParameterGroup exploParams;

   ofParameter<int> minX;
   ofParameter<int> maxX;
   ofParameter<int> minY;
   ofParameter<int> maxY;
   ofParameterGroup offsetParams;

   ofxXmlSettings settings;
   ofDirectory dir;
   vector<string> styles;
   int styleIndex = 0;

   void setup() {
      ofSetEscapeQuitsApp(false);

      gui.setup();
      setupParams();

      loadStyles();

      if (settings.loadFile("init.xml")) {
         ofDeserialize(settings, monkParams);
         ofDeserialize(settings, nikesParams);
         ofDeserialize(settings, exploParams);
         ofDeserialize(settings, offsetParams);
      }

      ofBackground(0);
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

      offsetParams.clear();
      offsetParams.setName("offsetParams");
      offsetParams.add(minX.set("minX", 0, -1920, 1920));
      offsetParams.add(maxX.set("maxX", 1920, -1920, 1920));
      offsetParams.add(minY.set("minY", 0, -1920, 1920));
      offsetParams.add(maxY.set("maxY", 1080, -1920, 1920));
   }

   void update() {
   }

   void draw() {
      mouseOverGui = imGui();
   }

   bool imGui() {
      auto guiSettings = ofxPreset::Gui::Settings();
      guiSettings.windowPos = ofVec2f(gOffset, gOffset);
      guiSettings.windowSize = ofVec2f(ofGetWidth() - gOffset * 2, ofGetHeight() - gOffset * 2);

      gui.begin();
      gui.draw();
      {
         if (ofxPreset::Gui::BeginWindowFlags("Dissonance distance", guiSettings, false, NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("\n");

            ImGui::Columns(3, "buttons", false);
            if (ImGui::Button("SAVE", ImVec2(ImGui::GetColumnWidth(), 24.0f)))
               saveSettings();

            ImGui::NextColumn();
            if (ImGui::Button("LOAD", ImVec2(ImGui::GetColumnWidth(), 24.0f)))
               loadSettings();

            ImGui::NextColumn();
            if (ImGui::Button("RESET", ImVec2(ImGui::GetColumnWidth(), 24.0f)))
               setupParams();

            ImGui::Text("\n");

            ImGui::NextColumn();
            if (ImGui::Button("DEFAULT", ImVec2(ImGui::GetColumnWidth(), 24.0f)))
               loadSettings("init.xml");

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
         }
         ofxPreset::Gui::EndWindow(guiSettings);
      }

      gui.end();

      return guiSettings.mouseOverGui;
   }

   void saveSettings() {
      settings.clear();
      ofSerialize(settings, monkParams);
      ofSerialize(settings, nikesParams);
      ofSerialize(settings, exploParams);
      ofSerialize(settings, offsetParams);

      ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + ".xml", "Save Settings");
      if (saveFileResult.bSuccess) {
         settings.saveFile(saveFileResult.filePath);
         loadStyles();
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
      }
   }

   void nextStyle() {
      styleIndex++;
      if (styleIndex > (styles.size() - 1))
         styleIndex = 0;
      loadSettings(styles[styleIndex]);
   }
};
