#pragma once
#include "ofxGui.h"
#include "gallery.h"
class mainPanel
{
public:
	~mainPanel()
	{
		applyFilterButton.removeListener(this, &mainPanel::onApplyFilters);
		resetFilterButton.removeListener(this, &mainPanel::onResetFilters);
		suffleImages.removeListener(this, &mainPanel::onShuffle);
		temperatureBeh.removeListener(this, &mainPanel::onTemperatureBehaviour);
		cutsBeh.removeListener(this, &mainPanel::onCutsBehaviour);
		playOrPause.removeListener(this, &mainPanel::onPlayOrPauseBehaviour);
	}

	void setup(gallery *gallery);
	void update();
	void draw();

	void resetFilters();

	// Buttons
	void onApplyFilters();
	void onResetFilters();
	void onWarmSelected(bool& value);
	void onColdSelected(bool& value);
	void onShuffle();
	void onTemperatureBehaviour(bool &value);
	void onCutsBehaviour(bool &value);
	void onPlayOrPauseBehaviour(bool& value);

private:
	gallery *galleryRef = nullptr;

	ofxPanel gui;
	ofParameterGroup settings;

	ofParameterGroup imagesParameters;

	ofParameterGroup imagesFilters;
	ofParameter<bool> warm;
	ofParameter<bool> cold;
	ofParameter<float> luminance;
	ofParameter<float> minTexture;
	ofParameter<float> maxTexture;
	ofParameter<int> numFaces;

	ofParameterGroup imagesBehaviours;
	ofParameter<bool> temperatureBeh;

	ofxButton applyFilterButton;
	ofxButton resetFilterButton;
	ofxButton suffleImages;

	ofParameterGroup videosParameters;
	ofParameterGroup videosFilters;
	ofParameter<int> duration;
	ofParameter<int> numCuts;

	ofParameterGroup videosBehaviours;
	ofParameter<bool> cutsBeh;
	ofParameter<bool> playOrPause;
};
