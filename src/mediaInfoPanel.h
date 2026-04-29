#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "media.h"

class mediaInfoPanel
{
public:

	~mediaInfoPanel()
	{
		similarImages.removeListener(this, &mediaInfoPanel::onSearchSimilar);
	}

	void setup();
	void update(std::shared_ptr<media> m);
	void draw();
	void setGuiPosition(float x, float y);
	void onSearchSimilar();
	bool getShowSimilar();
	void setShowingSimilar(bool state);

private:

	bool showSimilar;
	std::shared_ptr<media> selectedMedia;

	ofxPanel videoGui;
	ofxPanel imageGui;
	ofParameter<std::string> fileName;
	ofParameter<std::string> duration;
	ofParameter<std::string> numCuts;

	ofParameter<std::string> luminanceAvg;
	ofParameter<std::string> luminanceVar;
	ofParameter<std::string> colorAvg;
	ofParameter<std::string> colorVar;
	ofParameter<std::string> numFaces;
	ofParameter<std::string> edgeHistAvg;
	ofParameter<std::string> edgeHistVar;
	ofParameter<std::string> textureHistAvg;
	ofParameter<std::string> textureHistVar;

	ofxButton similarImages;
};