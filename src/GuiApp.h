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

   ofParameterGroup exploParams;

   ofxXmlSettings settings;

   void setup() {
      gui.setup();
      setupParams();

      if (settings.loadFile("init.xml")) {
         ofDeserialize(settings, monkParams);
         ofDeserialize(settings, nikesParams);
      }

      ofBackground(0);
      ofSetVerticalSync(false);
   }

   void setupParams() {
      monkParams.clear();
      monkParams.setName("monkParams");
      monkParams.add(monkX.set("monkX", 0, 0, 1920));
      monkParams.add(monkY.set("monkY", 0, 0, 1920));
      monkParams.add(monkW.set("monkW", 540, 100, 1080));
      monkParams.add(monkLO.set("monkLO", 0, -540, 540));
      monkParams.add(monkRO.set("monkRO", 0, -540, 540));
      monkParams.add(monkTO.set("monkTO", 0, -540, 540));
      monkParams.add(monkBO.set("monkBO", 0, -540, 540));

      nikesParams.clear();
      nikesParams.setName("nikesParams");
      nikesParams.add(nikesX.set("nikesX", 0, 0, 1920));
      nikesParams.add(nikesY.set("nikesY", 0, 0, 1920));
      nikesParams.add(nikesW.set("nikesW", 540, 100, 1080));
      nikesParams.add(nikesLO.set("nikesLO", 0, -540, 540));
      nikesParams.add(nikesRO.set("nikesRO", 0, -540, 540));
      nikesParams.add(nikesTO.set("nikesTO", 0, -540, 540));
      nikesParams.add(nikesBO.set("nikesBO", 0, -540, 540));
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
         //  ImGui::SetNextWindowPos(ImVec2(gOffset, gOffset), ImGuiCond_Always);
         //  ImGui::SetNextWindowSize(ImVec2(ofGetWidth() - gOffset * 2, ofGetHeight() - gOffset * 2), ImGuiCond_Always);
         //  ImGui::Begin("Dissonance distance", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
         if (ofxPreset::Gui::BeginWindowFlags("Dissonance distance", guiSettings, false, NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("\n");

            ImGui::Columns(3, "buttons", false);
            if (ImGui::Button("SAVE", ImVec2(ImGui::GetColumnWidth(), 24.0f))) {
               saveSettings();
            }
            ImGui::NextColumn();
            if (ImGui::Button("LOAD", ImVec2(ImGui::GetColumnWidth(), 24.0f))) {
               loadSettings();
            }
            ImGui::NextColumn();
            if (ImGui::Button("RESET", ImVec2(ImGui::GetColumnWidth(), 24.0f))) {
               setupParams();
            }

            ImGui::Columns(1);
            ImGui::Text("\n");

            ofxPreset::Gui::AddGroup(monkParams, guiSettings);
            ofxPreset::Gui::AddGroup(nikesParams, guiSettings);
            // ofxPreset::Gui::AddGroup(parameters.colors, mainSettings);
         }
         //  ImGui::End();
         ofxPreset::Gui::EndWindow(guiSettings);
      }

      gui.end();

      return guiSettings.mouseOverGui;
   }

   void saveSettings() {
      settings.clear();
      ofSerialize(settings, monkParams);
      ofSerialize(settings, nikesParams);

      ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + ".xml", "Save Settings");
      if (saveFileResult.bSuccess) {
         settings.saveFile(saveFileResult.filePath);
      }
   }

   void loadSettings() {
      ofFileDialogResult openFileResult = ofSystemLoadDialog("Load Settings", false, ofToDataPath("settings", true));
      if (openFileResult.bSuccess) {
         settings.loadFile(openFileResult.getPath());
         ofDeserialize(settings, monkParams);
         ofDeserialize(settings, nikesParams);
      }
   }
};
