#include "userInteractions.h"

void userInteractions::setup(gallery *gallery)
{
  galleryRef = gallery;

  camWidth = 640;
  camHeight = 480;

  vector<ofVideoDevice> devices = vidGrabber.listDevices();

  vidGrabber.setDeviceID(0);
  vidGrabber.setDesiredFrameRate(30);
  vidGrabber.setup(camWidth, camHeight);
  ofSetVerticalSync(true);

  cameraStartupFrames = 0;
}

void userInteractions::handleKeyboard(int key)
{
    switch (key)
    {
    case 32: // space bar
        handlePlayOrPause();
        break;
    case OF_KEY_LEFT:
        handlePrevMedia();
        break;
    case OF_KEY_RIGHT:
        handleNextMedia();
        break;
    case 'c':
        handleShowOrHideCam();
    }
}

void userInteractions::handlePlayOrPause()
{
  auto selected = galleryRef->getSelectedMedia();
  if (selected)
  {
    // Check if selected media is a video
    auto video = dynamic_pointer_cast<videoMedia>(selected);
    if (video)
    {
      video->playOrPauseVideo();
    }
  }
}

void userInteractions::handleNextMedia()
{
  auto selected = galleryRef->getSelectedMedia();
  if (selected)
  {
    galleryRef->setSelected(1);
  }
}

void userInteractions::handlePrevMedia()
{
  auto selected = galleryRef->getSelectedMedia();
  if (selected)
  {
    galleryRef->setSelected(-1);
  }
}

void userInteractions::handleShowOrHideCam()
{
  auto selected = galleryRef->getSelectedMedia();
  if (selected)
  {
    
    if (vidGrabber.isInitialized())
    {
      vidGrabber.close(); 
    }
    else
    {
      vidGrabber.initGrabber(camWidth, camHeight); 
    }
  }
}

void userInteractions::update()
{

  auto selected = galleryRef->getSelectedMedia();
  if (!selected)
  {
    vidGrabber.close();
  }

  float windowW = ofGetWidth();
  float windowH = ofGetHeight();

  // detection area
  // right
  detectionAreaPrev.set(
      camWidth - 150, 
      100,              
      120,                        
      120                         
  );
  // left
  detectionAreaNext.set(
      100, 
      100, 
      120,           
      120            
  );
  ofBackground(100, 100, 100);
  vidGrabber.update();

  if (counter < 30) {
      counter++;
      currFrame.setFromPixels(vidGrabber.getPixels());
      currFrame.setImageType(OF_IMAGE_GRAYSCALE);
      prevFrame = currFrame;
      return;
  }

  if (vidGrabber.isInitialized())
  {
    if (vidGrabber.isFrameNew())
    {
      if (cameraStartupFrames < STARTUP_DELAY)
      {
        cameraStartupFrames++;
        
        prevFrame.setFromPixels(vidGrabber.getPixels());
        prevFrame.setImageType(OF_IMAGE_GRAYSCALE);
        return; 
      }

      if (!prevFrame.isAllocated())
      {
        prevFrame.setFromPixels(vidGrabber.getPixels());
        prevFrame.setImageType(OF_IMAGE_GRAYSCALE);
      }
      else
      {
        currFrame.setFromPixels(vidGrabber.getPixels());
        currFrame.setImageType(OF_IMAGE_GRAYSCALE);

        handleNextGesture();
        handlePrevGesture();
        prevFrame = currFrame;
      }
    }
  }
}

void userInteractions::draw()
{
  if (vidGrabber.isInitialized())
  {
    ofDisableDepthTest();
    ofSetHexColor(0xffffff);

    ofPushMatrix();

    float windowW = ofGetWidth();
    float windowH = ofGetHeight();

    ofTranslate(windowW-100, windowH - camHeight - 50); // Move to right edge of where video will be drawn
    
    ofScale(-1, 1); 
    vidGrabber.draw(0, 0);

    // rectangles
    ofColor color = ofColor(255, 0, 0);
    ofNoFill();
    ofSetColor(color);
    ofSetLineWidth(6);
    ofDrawRectangle(detectionAreaPrev.x, detectionAreaPrev.y, detectionAreaPrev.width, detectionAreaPrev.height);
    color = ofColor(0, 255, 0);
    ofSetColor(color);
    ofDrawRectangle(detectionAreaNext.x, detectionAreaNext.y, detectionAreaNext.width, detectionAreaNext.height);
    ofFill();

    ofSetColor(ofColor::white);
    ofPopMatrix();
    ofEnableDepthTest();
  }
}

void userInteractions::handleNextGesture()
{
  float totalDiff = 0;
  int detectedPixels = 0;

  int xStart = detectionAreaNext.x;
  int yStart = detectionAreaNext.y;
  int xEnd = xStart + detectionAreaNext.width;
  int yEnd = yStart + detectionAreaNext.height;

  for (int y = yStart; y < yEnd; y++)
  {
    for (int x = xStart; x < xEnd; x++)
    {
      int i = y * currFrame.getWidth() + x;
      totalDiff += abs(currFrame.getPixels()[i] - prevFrame.getPixels()[i]);
      detectedPixels++;
    }
  }

  float avgDiff = totalDiff / detectedPixels;
  if (avgDiff > 25.0f)
  {
    handleNextMedia();
    counter = 0;
  }
}

void userInteractions::handlePrevGesture()
{
  float totalDiff = 0;
  int detectedPixels = 0;

  int xStart = detectionAreaPrev.x;
  int yStart = detectionAreaPrev.y;
  int xEnd = xStart + detectionAreaPrev.width;
  int yEnd = yStart + detectionAreaPrev.height;

  for (int y = yStart; y < yEnd; y++)
  {
    for (int x = xStart; x < xEnd; x++)
    {
      int i = y * currFrame.getWidth() + x;
      totalDiff += abs(currFrame.getPixels()[i] - prevFrame.getPixels()[i]);
      detectedPixels++;
    }
  }

  float avgDiff = totalDiff / detectedPixels;
  if (avgDiff > 25.0f)
  {
    handlePrevMedia();
    counter=0;
  }
}