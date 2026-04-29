#include "media.h"

ofVec3f media::getPosition()
{
	return position;
}

float media::getSize()
{
	return size;
}

string media::getFileName()
{
	return fileName;
}

int media::getIndex()
{
	return index;
}

void media::setPosition(float x, float y, float z)
{
	position = ofVec3f(x, y, z);
}

void media::setSize(float s)
{
	size = s;
}
void media::setFileName(string name)
{
	fileName = name;
}

void media::setIndex(int i)
{
	index = i;
}

void media::setMotionSpeed(float s)
{
	speed = s;
}

void media::updateMotion(float deltaTime)
{
	angle += speed * deltaTime;
	float x = center.x + radius * cos(angle);
	float y = center.y + radius * sin(angle);
	float z = center.z;
	setPosition(x, y, z);
}

void media::setMotionCenter(ofVec3f c)
{
	center = c;
}

void media::setMotionRadius(float r)
{
	radius = r;
}

float media::getMotionSpeed()
{
	return speed;
}

float media::getRadius()
{
	return radius;
}

ofVec3f media::getCenter()
{
	return center;
}
