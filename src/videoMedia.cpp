#include "videoMedia.h"

void *videoMedia::getMedia()
{
    return &video;
}
float videoMedia::getDuration()
{
    return duration;
}
float videoMedia::getNumCuts()
{
    return numCuts;
}

void videoMedia::setVideo(ofVideoPlayer v)
{
    video = v;
}
void videoMedia::setDuration(float dur)
{
    duration = dur;
}
void videoMedia::setNumCuts(float ncuts)
{
    numCuts = ncuts;
}

void videoMedia::drawSelectedMedia(mediaInfoPanel *panel)
{
    ofDisableDepthTest();
    
    float windowW = ofGetWidth();
    float windowH = ofGetHeight();

    float maxW = windowW * 0.6;
    float maxH = windowH * 0.6;

    float imageW = video.getWidth();
    float imageH = video.getHeight();

    float scaleX = maxW / imageW;
    float scaleY = maxH / imageH;
    float scale = std::min(scaleX, scaleY);

    float drawW = imageW * scale;
    float drawH = imageH * scale;

    float x = (windowW - drawW) / 2.0f;
    float y = (windowH - drawH) / 2.0f;

    ofSetColor(255, 255, 255, 255);

    // Draw selected video
    video.draw(x, y, drawW, drawH);

    // Draw border
    ofColor color = ofColor(255, 0, 0);
    ofColor borderColor(color);
    ofNoFill();
    ofSetColor(borderColor);
    ofSetLineWidth(6);
    ofDrawRectangle(x + 0.5, y + 0.5, drawW - 1, drawH - 1);
    ofFill(); 
    ofSetColor(ofColor::white);

    if (panel != nullptr)
    {
        float guiX = x;
        float guiY = y + drawH + 20;       
        panel->setGuiPosition(guiX, guiY); 
        panel->draw();
    }

    ofEnableDepthTest();
}

ofTexture videoMedia::getTexture()
{
    return video.getTexture();
}

void videoMedia::draw()
{
    ofPlanePrimitive plane;
    plane.set(size, size);
    plane.mapTexCoordsFromTexture(getTexture());

    ofColor color = ofColor(255, 0, 0);
    drawBorder(position, ofVec2f(size, size), color);

    ofPushMatrix();
    ofTranslate(position.x, position.y, position.z);
    ofRotateYDeg(180);

    getTexture().bind();
    ofSetColor(255);
    plane.draw();
    getTexture().unbind();

    ofPopMatrix();

    // ofVideoPlayer *video = static_cast<ofVideoPlayer *>(getMedia());
    video.update();

    // video.draw(position.x, position.y, size, size);
}

void videoMedia::drawBorder(ofVec3f position, ofVec2f size, ofColor color)
{
    ofPlanePrimitive planeBorder;
    planeBorder.set(size.x * 1.05f, size.y * 1.05f);

    ofPushMatrix();
    ofTranslate(position.x, position.y, position.z + 1);
    ofSetColor(color); 
    planeBorder.draw();
    ofPopMatrix();
}

void videoMedia::playOrPauseVideo()
{
    if (video.isPlaying())
    {
        video.setPaused(true);
    }
    else
    {
        video.play();
    }
}

void videoMedia::playVideo()
{
    video.play();
}

void videoMedia::pauseVideo()
{
    video.setPaused(true);
}
