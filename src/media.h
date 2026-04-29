#pragma once
#include "ofMain.h"

class mediaInfoPanel;

// Represents a media object (can be an image or video)
class media
{
public:
	virtual ~media() {}

	// Gets
	ofVec3f getPosition();
	float getSize();
	string getFileName();
	int getIndex();

	// Sets
	void setPosition(float x, float y, float z);
	void setSize(float size);
	void setFileName(string fileName);
	void setIndex(int i);

	// Abstract methods
	virtual void *getMedia() = 0;
	virtual void draw() = 0;
	virtual ofTexture getTexture() = 0;
	virtual void drawSelectedMedia(mediaInfoPanel *panel) = 0;
	virtual void drawBorder(ofVec3f position, ofVec2f size, ofColor color) = 0;

	// Behaviours
	void setMotionSpeed(float s);
	void updateMotion(float deltaTime);
	void setMotionCenter(ofVec3f c);
	void setMotionRadius(float r);
	float getMotionSpeed();
	float getRadius();
	ofVec3f getCenter();

protected:
	ofVec3f position;
	float size;
	string fileName;
	int index;

	// Behaviours
	float angle = 0.0f;
	float speed = 0.0f;
	float radius = 50.0f;
	ofVec3f center;
};
