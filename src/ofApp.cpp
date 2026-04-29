#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// Setup room
	gallery.setupRoom();

	// Setup images
	ofDirectory dirImages;
	dirImages.listDir("images");
	dirImages.allowExt("jpg");
	gallery.setupImages(dirImages);

	// Setup videos
	ofDirectory dirVideos;
	dirVideos.listDir("movies");
	dirVideos.allowExt("mp4");
	gallery.setupVideos(dirVideos);

	// Setup user interactions
	userInteractions.setup(&gallery);
	ofBackground(ofColor::white);

	// Setup main panel
	mainPanel.setup(&gallery);
}

//--------------------------------------------------------------
void ofApp::update()
{
	gallery.update();
	userInteractions.update();
	mainPanel.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	gallery.drawRoom();
	gallery.drawSelectedMedia();
	userInteractions.draw();

	auto selected = gallery.getSelectedMedia();
	if (!selected){
		ofDisableDepthTest();
		mainPanel.draw();
		ofEnableDepthTest();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	userInteractions.handleKeyboard(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	gallery.handleMousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
