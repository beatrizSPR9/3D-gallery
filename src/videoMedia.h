#pragma once
#include "media.h"
#include "mediaInfoPanel.h"

class videoMedia : public media
{
public:
	// Gets
	float getDuration();
	float getNumCuts();

	// Sets
	void setVideo(ofVideoPlayer vid);
	void setDuration(float dur);
	void setNumCuts(float ncuts);
	void playOrPauseVideo();
	void playVideo();
	void pauseVideo();

	// Abstract methods
	void *getMedia() override;
	void drawSelectedMedia(mediaInfoPanel *panel) override;
	ofTexture getTexture() override;
	void draw() override;
	void drawBorder(ofVec3f position, ofVec2f size, ofColor color) override;

private:
	ofVideoPlayer video;
	float duration;
	float numCuts;
};
