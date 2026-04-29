#pragma once
#include "ofMain.h"
#include "media.h"
#include "ofxGui.h"
#include "mediaInfoPanel.h"
#include "ofxCv.h"

class imageMedia : public media
{

public:
	// Gets
	ofVec3f getColorAvg();
	ofVec3f getColorVar();
	float getLuminanceAvg();
	float getLuminanceVar();
	float getEdgeHistAvg(int index);
	float getEdgeHistVar(int index);
	float getTextureHistAvg(int index);
	float getTextureHistVar(int index);
	float getNumFaces();
	cv::Mat getDescriptors();

	// Sets
	void setImage(const ofImage &image);
	void setColorAvg(float R, float G, float B);
	void setColorVar(float R, float G, float B);
	void setLuminanceAvg(float luminanceAvg);
	void setLuminanceVar(float luminanceVar);
	void setEdgeHistAvg(float value, int index);
	void setEdgeHistVar(float value, int index);
	void setTextureHistAvg(float value, int index);
	void setTextureHistVar(float value, int index);
	void setNumFaces(float nFaces);
	void setDescriptors(cv::Mat &descriptor);

	// Abstract methods
	void *getMedia() override;
	void drawSelectedMedia(mediaInfoPanel *panel) override;
	void draw() override;
	ofTexture getTexture() override;
	void drawBorder(ofVec3f position, ofVec2f size, ofColor color) override;

private:
	ofImage image;
	float rAvg, gAvg, bAvg;
	float rVar, gVar, bVar;
	float luminanceAvg;
	float luminanceVar;
	vector<float> edgeHistAvg = vector<float>(5, 0.0f);
	vector<float> edgeHistVar = vector<float>(5, 0.0f);
	vector<float> textureHistAvg = vector<float>(24, 0.0f);
	vector<float> textureHistVar = vector<float>(24, 0.0f);
	float numFaces;
	cv::Mat desc;
};
