#pragma once
#include "ofMain.h"
#include <ofxXmlSettings.h>
#include "imageMedia.h"
#include "videoMedia.h"

class mediaMetadata
{

public:
	virtual ~mediaMetadata() {}
	void loadAndCreateImageMetaData(imageMedia &img, string fileName);
	void loadAndCreateVideoMetaData(videoMedia &vid, string fileName);

private:
	void generateImageMetaData(imageMedia &img, string xmlPath, ofxXmlSettings &XML);
	void generateVideoMetaData(videoMedia &vid, string xmlPath, ofxXmlSettings &XML);

	// Lumiance and color
	void calculateLuminanceAndColorAvg(imageMedia &img);
	void calculateLuminanceAndColorVar(imageMedia &img);
	// Edge histograms
	void calculateEdgeHistogram(imageMedia &img);
	float calculateEdgeHistogramAvg(ofImage img);
	float calculateEdgeHistogramVar(ofImage img, float average);
	// Texture histograms
	void calculateTextureHistogram(imageMedia &img);
	float calculateTextureHistogramAvg(ofImage img);
	float calculateTextureHistogramVar(ofImage img, float average);
	// num faces
	void calculateNumFaces(imageMedia &img);

	// keypoits/descriptors
	void detectAndCompute(imageMedia &img);

	// Cuts
	void calculateCuts(videoMedia &vid);
	float calculateDiff(ofImage &prevFrame, ofImage &currFrame, vector<int> &hist1, vector<int> &his2);
	void calculateHistogram(const ofPixels &frame, vector<int> &hist);
};
