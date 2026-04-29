#pragma once
#include "ofMain.h"
#include "media.h"
#include "imageMedia.h"
#include "videoMedia.h"
#include "mediaInfoPanel.h"

class gallery
{

public:
	// Setup room
	void setupRoom();

	// Setup images/videos
	void setupImages(ofDirectory dir);
	void setupVideos(ofDirectory dir);

	// Update
	void update();

	// Draw
	void drawRoom();
	void drawImages();
	void drawVideos();
	void drawSelectedMedia();

	// Calculate sizes
	void calculateImageSize();
	void calculateVideoSize();

	// Handle user interactions
	void handleMousePressed(int x, int y, int button);

	// Handle Filters
	void filterByColor(string type);
	void filterByLuminance(float luminance);
	void filterByNumFaces(int numFaces);
	void filterByDuration(int duration);
	void filterByNumCuts(int numCuts);
	void filterByTexture(float minTexture, float maxTexture);
	void filterSimilarImages();
	void removeFilters();
	void shuffleImages();

	// Handle Behaviours
	void handleTemperatureBehaviour(bool state);
	void handleCutsBehaviour(bool state);
	void handlePlayOrPauseVideos(bool state);

	shared_ptr<media> getSelectedMedia() const;
	void setSelected(int i);


private:

	int getSimilarity(imageMedia &refence, imageMedia &image);

	vector<imageMedia> images;
	vector<imageMedia> imagesToShow;

	vector<videoMedia> videos;
	vector<videoMedia> videosToShow;

	shared_ptr<media> selectedMedia;
	int selectedMediaIndex;

	mediaInfoPanel infoPanel;

	ofEasyCam cam;
	float roomWidth;
	float roomHeight;
	float roomDepth;

	// Wall textures
	ofTexture brickTexture;
	ofTexture floorTexture;
	ofTexture ceilingTexture;

	ofLight pointLight;

	ofPlanePrimitive wallPlane;
	ofPlanePrimitive floorPlane;
	ofPlanePrimitive ceilingPlane;
};
