#include "mediaInfoPanel.h"
#include "imageMedia.h"
#include "videoMedia.h"
#include "media.h"

void mediaInfoPanel::setup()
{
  videoGui.setDefaultBackgroundColor(ofColor(50, 50, 50, 200)); // Dark semi-transparent
  videoGui.setDefaultFillColor(ofColor(80, 80, 80, 255));       // Darker fill
  videoGui.setDefaultTextColor(ofColor(255, 255, 255, 255));    // White text
  videoGui.setDefaultBorderColor(ofColor(100, 100, 100, 255));  // Gray border
  videoGui.setDefaultWidth(400);                                // Make it wider
  videoGui.setDefaultHeight(25);
  videoGui.setup("Video Info");

  videoGui.add(fileName.set("File name", ""));
  videoGui.add(duration.set("Duration", ""));
  videoGui.add(numCuts.set("Number of cuts", ""));

  imageGui.setDefaultBackgroundColor(ofColor(50, 50, 50, 200)); // Dark semi-transparent
  imageGui.setDefaultFillColor(ofColor(80, 80, 80, 255));       // Darker fill
  imageGui.setDefaultTextColor(ofColor(255, 255, 255, 255));    // White text
  imageGui.setDefaultBorderColor(ofColor(100, 100, 100, 255));  // Gray border
  imageGui.setDefaultWidth(750);                                // Make it wider
  imageGui.setDefaultHeight(30);                                // Height per element
  imageGui.setup("Image Info");

  imageGui.add(fileName.set("File name", ""));
  imageGui.add(luminanceAvg.set("Luminance average", ""));
  imageGui.add(luminanceVar.set("Luminance variance", ""));
  imageGui.add(colorAvg.set("Color average (R,G,B)", ""));
  imageGui.add(colorVar.set("Color variance (R,G,B)", ""));
  imageGui.add(numFaces.set("Number of faces", ""));
  imageGui.add(edgeHistAvg.set("Edge histogram average", ""));
  imageGui.add(edgeHistVar.set("Edge histogram variance", ""));
  imageGui.setDefaultHeight(80);
  imageGui.add(textureHistAvg.set("Texture averages", ""));
  imageGui.setDefaultHeight(80);
  imageGui.add(textureHistVar.set("Texture variances", ""));
  imageGui.setDefaultHeight(30);

  // Add similar button
  similarImages.setup("Search for similiar images");
  similarImages.setSize(30, similarImages.getHeight());
  similarImages.addListener(this, &mediaInfoPanel::onSearchSimilar);

  imageGui.add(&similarImages);
}

void mediaInfoPanel::update(std::shared_ptr<media> m)
{
  selectedMedia = m;

  if (!m)
  {
    fileName = "";
    duration = "";
    numCuts = "";
    luminanceAvg = "";
    luminanceVar = "";
    colorAvg = "";
    colorVar = "";
    numFaces = "";
    edgeHistAvg = "";
    edgeHistVar = "";
    textureHistAvg = "";
    textureHistVar = "";
    return;
  }
  
  auto video = dynamic_pointer_cast<videoMedia>(selectedMedia);
  if (video)
  {
    fileName = video->getFileName();
    duration = ofToString(video->getDuration());
    numCuts = ofToString(video->getNumCuts());
  }

  auto image = dynamic_pointer_cast<imageMedia>(selectedMedia);
  if (image)
  {
    fileName = image->getFileName();
    luminanceAvg = ofToString(image->getLuminanceAvg());
    luminanceVar = ofToString(image->getLuminanceVar());
    colorAvg = ofToString(image->getColorAvg().x) + ", " + ofToString(image->getColorAvg().y) + ", " + ofToString(image->getColorAvg().z);
    colorVar = ofToString(image->getColorVar().x) + ", " + ofToString(image->getColorVar().y) + ", " + ofToString(image->getColorVar().z);
    numFaces = ofToString(image->getNumFaces());
    edgeHistAvg = ofToString(image->getEdgeHistAvg(0)) + ", " + ofToString(image->getEdgeHistAvg(1)) + ", " + ofToString(image->getEdgeHistAvg(2)) + ", " + ofToString(image->getEdgeHistAvg(3)) + ", " + ofToString(image->getEdgeHistAvg(4));
    edgeHistVar = ofToString(image->getEdgeHistVar(0)) + ", " + ofToString(image->getEdgeHistVar(1)) + ", " + ofToString(image->getEdgeHistVar(2)) + ", " + ofToString(image->getEdgeHistVar(3)) + ", " + ofToString(image->getEdgeHistVar(4));
    textureHistAvg = "";
    for (int i = 0; i < 24; i++)
    {
      if (i != 0 && i % 8 == 0)
        textureHistAvg += "\n";
      textureHistAvg += ofToString(image->getTextureHistAvg(i), 4) + " ";
    }
    // textureHistAvg = "";
    textureHistVar = "";
    for (int i = 0; i < 24; i++)
    {
      if (i != 0 && i % 8 == 0)
        textureHistVar += "\n";
      textureHistVar += ofToString(image->getTextureHistVar(i), 4) + " ";
    }
    // textureHistVar = "";
  }
}

void mediaInfoPanel::draw()
{


  if (!selectedMedia)
    return;

  auto video = dynamic_pointer_cast<videoMedia>(selectedMedia);
  if (video)
  {
    videoGui.draw();
  }

  auto image = dynamic_pointer_cast<imageMedia>(selectedMedia);
  if (image)
  {
    imageGui.draw();
  }
}

void mediaInfoPanel::setGuiPosition(float x, float y)
{
  imageGui.setPosition(x, y);
  videoGui.setPosition(x, y);
}

void mediaInfoPanel::onSearchSimilar()
{
    showSimilar = true;
}

bool mediaInfoPanel::getShowSimilar()
{
    return showSimilar;
}

void mediaInfoPanel::setShowingSimilar(bool state)
{
    showSimilar = state;
}
