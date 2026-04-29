#pragma once
#include "ofMain.h"
#include "gallery.h"

class userInteractions
{
public:
  void setup(gallery *gallery);
  void update();
  void draw();
  void handleKeyboard(int key);

private:
  gallery *galleryRef = nullptr;
  ofVideoGrabber vidGrabber;

  int counter = 0;
  int camWidth;
  int camHeight;
  ofImage prevFrame;
  ofImage currFrame;

  ofRectangle detectionAreaPrev;
  ofRectangle detectionAreaNext;
  int cameraStartupFrames;
  const int STARTUP_DELAY = 5;

  void handlePlayOrPause();
  void handlePrevMedia();
  void handleNextMedia();
  void handleShowOrHideCam();
  void handleNextGesture();
  void handlePrevGesture();
};