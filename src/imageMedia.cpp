#include "imageMedia.h"
#include "ofxGui.h"

void *imageMedia::getMedia()
{
    return &image;
}

ofVec3f imageMedia::getColorAvg()
{
    return ofVec3f(rAvg, gAvg, bAvg);
}
ofVec3f imageMedia::getColorVar()
{
    return ofVec3f(rVar, gVar, bVar);
}
float imageMedia::getLuminanceAvg()
{
    return luminanceAvg;
}
float imageMedia::getLuminanceVar()
{
    return luminanceVar;
}
float imageMedia::getEdgeHistAvg(int index)
{
    return edgeHistAvg[index];
}
float imageMedia::getEdgeHistVar(int index)
{
    return edgeHistVar[index];
}
float imageMedia::getTextureHistAvg(int index)
{
    return textureHistAvg[index];
}
float imageMedia::getTextureHistVar(int index)
{
    return textureHistVar[index];
}

float imageMedia::getNumFaces()
{
    return numFaces;
}

cv::Mat imageMedia::getDescriptors()
{
    return desc;
}

void imageMedia::setImage(const ofImage &img)
{
    image = img;
}
void imageMedia::setColorAvg(float R, float G, float B)
{
    rAvg = R;
    gAvg = G;
    bAvg = B;
}
void imageMedia::setColorVar(float R, float G, float B)
{
    rVar = R;
    gVar = G;
    bVar = B;
}
void imageMedia::setLuminanceAvg(float l)
{
    luminanceAvg = l;
}
void imageMedia::setLuminanceVar(float l)
{
    luminanceVar = l;
}
void imageMedia::setEdgeHistAvg(float value, int index)
{
    edgeHistAvg[index] = value;
}
void imageMedia::setEdgeHistVar(float value, int index)
{
    edgeHistVar[index] = value;
}
void imageMedia::setTextureHistAvg(float value, int index)
{
    textureHistAvg[index] = value;
}
void imageMedia::setTextureHistVar(float value, int index)
{
    textureHistVar[index] = value;
}
void imageMedia::setNumFaces(float nFaces)
{
    numFaces = nFaces;
}

void imageMedia::setDescriptors(cv::Mat& descriptor)
{
    desc = descriptor;
}

void imageMedia::drawSelectedMedia(mediaInfoPanel *panel)
{
    ofDisableDepthTest();
    // Calculate image size and position to draw
    float windowW = ofGetWidth();
    float windowH = ofGetHeight();

    float maxW = windowW * 0.6;
    float maxH = windowH * 0.6;
    float xOffset = windowW * 0.20f;

    float imageW = image.getWidth();
    float imageH = image.getHeight();

    float scaleX = maxW / imageW;
    float scaleY = maxH / imageH;
    float scale = std::min(scaleX, scaleY);

    float drawW = imageW * scale;
    float drawH = imageH * scale;

    float x = ((windowW - drawW) / 2.0f) - xOffset;
    float y = (windowH - drawH) / 2.0f;

    ofSetColor(255, 255, 255, 255);

    // Draw selected image
    image.draw(x, y, drawW, drawH);

    // Draw border
    ofColor color = ofColor(getColorAvg().x, getColorAvg().y, getColorAvg().z);
    ofColor borderColor(color);
    ofNoFill();
    ofSetColor(borderColor);
    ofSetLineWidth(6);
    ofDrawRectangle(x + 0.5, y + 0.5, drawW - 1, drawH - 1);
    ofFill(); 
    ofSetColor(ofColor::white);

    if (panel != nullptr)
    {
        float guiX = x + drawW + 20;
        float guiY = y;                    
        panel->setGuiPosition(guiX, guiY);
        panel->draw();
    }
    ofEnableDepthTest();
}

void imageMedia::draw()
{
    ofPlanePrimitive plane;
    plane.set(size, size);
    plane.mapTexCoordsFromTexture(getTexture());

    ofColor color = ofColor(getColorAvg().x, getColorAvg().y, getColorAvg().z);
    drawBorder(position, ofVec2f(size, size), color);

    ofPushMatrix();
    ofTranslate(position.x, position.y, position.z);

    getTexture().bind();
    ofSetColor(255);
    plane.draw();
    getTexture().unbind();

    ofPopMatrix();
}

ofTexture imageMedia::getTexture()
{
    return image.getTexture();
}

void imageMedia::drawBorder(ofVec3f position, ofVec2f size, ofColor color)
{
    ofPlanePrimitive planeBorder;
    planeBorder.set(size.x * 1.10f, size.y * 1.10f);

    ofPushMatrix();
    ofTranslate(position.x, position.y, position.z - 1);
    ofSetColor(color); 
    planeBorder.draw();
    ofPopMatrix();
}
