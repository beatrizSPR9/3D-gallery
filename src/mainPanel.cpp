#include "mainPanel.h"

void mainPanel::setup(gallery* gallery)
{
    galleryRef = gallery;

    // Images filters
    imagesFilters.setName("Images Filters");
    imagesFilters.add(warm.set("Warm", false));
    imagesFilters.add(cold.set("Cold", false));
    imagesFilters.add(luminance.set("Luminance", 0, 0, 255));
    imagesFilters.add(minTexture.set("Minimum Texture", 0.0f, 0.0f, 1.0f));
    imagesFilters.add(maxTexture.set("Maximum Texture", 1.0f, 0.0f, 1.0f));
    imagesFilters.add(numFaces.set("Number of faces", 0, 0, 50));

    warm.addListener(this, &mainPanel::onWarmSelected);
    cold.addListener(this, &mainPanel::onColdSelected);

    // Images behaviours
    imagesBehaviours.setName("Behaviours");
    imagesBehaviours.add(temperatureBeh.set("By temperature", false));
    temperatureBeh.addListener(this, &mainPanel::onTemperatureBehaviour);

    imagesParameters.setName("Images");
    imagesParameters.add(imagesFilters);
    imagesParameters.add(imagesBehaviours);


    // Video filters
    videosFilters.setName("Videos Filters");
    videosFilters.add(duration.set("Duration", 0, 0, 50));
    videosFilters.add(numCuts.set("Number of cuts", 0, 0, 50));

    // Video behaviours
    videosBehaviours.setName("Behaviours");
    videosBehaviours.add(cutsBeh.set("By number of cuts", false));
    cutsBeh.addListener(this, &mainPanel::onCutsBehaviour);
    videosBehaviours.add(playOrPause.set("Play all videos", false));
    playOrPause.addListener(this, &mainPanel::onPlayOrPauseBehaviour);

    videosParameters.setName("Videos");
    videosParameters.add(videosFilters);
    videosParameters.add(videosBehaviours);


    settings.setName("Settings");
    settings.add(imagesParameters);
    settings.add(videosParameters);

    // Setup panel with parameters first
    gui.setDefaultWidth(300);
    gui.setup(settings);

    // Setup buttons
    applyFilterButton.setup("Apply Filters");
    applyFilterButton.setSize(300, applyFilterButton.getHeight());
    applyFilterButton.addListener(this, &mainPanel::onApplyFilters);

    resetFilterButton.setup("Reset Settings");
    resetFilterButton.setSize(300, resetFilterButton.getHeight());
    resetFilterButton.addListener(this, &mainPanel::onResetFilters);

    suffleImages.setup("Shuffle");
    suffleImages.setSize(300, suffleImages.getHeight());
    suffleImages.addListener(this, &mainPanel::onShuffle);

    // Add buttons to panel
    gui.add(&applyFilterButton);
    gui.add(&resetFilterButton);
    gui.add(&suffleImages);

    gui.setHeaderBackgroundColor(ofColor(67, 98, 95));

    gui.minimize();
}

void mainPanel::update() {
    if (galleryRef->getSelectedMedia())
        resetFilters();

    if (minTexture > maxTexture) {
        minTexture = maxTexture; 
    }

    float panelWidth = ofGetWidth() * 0.3; 
    if (panelWidth > 300) panelWidth = 300;
    gui.setDefaultWidth(panelWidth);      
    gui.setSize(panelWidth, gui.getHeight()); 

    // Optional: reposition
    gui.setPosition(10, 10);
}

void mainPanel::onCutsBehaviour(bool& value)
{
    if (galleryRef) {
        galleryRef->handleCutsBehaviour(value);
    }
}

void mainPanel::onPlayOrPauseBehaviour(bool& value)
{
    if (galleryRef) {
        galleryRef->handlePlayOrPauseVideos(value);
    }
}

void mainPanel::onTemperatureBehaviour(bool& value)
{
    if (galleryRef) {
        galleryRef->handleTemperatureBehaviour(value);
    }
}

void mainPanel::onApplyFilters()
{
    if (galleryRef) {
        if (warm) {
            galleryRef->filterByColor("warm");
            resetFilters();
            warm = true;
        }
        else if (cold) {
            galleryRef->filterByColor("cold");
            resetFilters();
            cold = true;
        }
        else if (luminance > 0.0f) {
            galleryRef->filterByLuminance(luminance);
            float aux = luminance;
            resetFilters();
            luminance = aux;
        }
        else if (minTexture > 0.0f || maxTexture < 1.0f) {
            galleryRef->filterByTexture(minTexture, maxTexture);
            float aux1 = minTexture;
            float aux2 = maxTexture;
            resetFilters();
            minTexture = aux1;
            maxTexture = aux2;
        }
        else if (numFaces > 0) {
            galleryRef->filterByNumFaces(numFaces);
            int aux = numFaces;
            resetFilters();
            numFaces = aux;
        }
        else if (duration > 0) {
            galleryRef->filterByDuration(duration);
            float aux = duration;
            resetFilters();
            duration = aux;
        }
        else if (numCuts > 0) {
            galleryRef->filterByNumCuts(numCuts);
            int aux = numCuts;
            resetFilters();
            numCuts = aux;
        }
        else
            galleryRef->removeFilters();
    }
    temperatureBeh = false;
}

void mainPanel::onResetFilters()
{
    if (galleryRef) {
        resetFilters();
        playOrPause = false;
        galleryRef->removeFilters();
    }
}

void mainPanel::onWarmSelected(bool& value) {
    if (value) {
        cold = false;
    }
}

void mainPanel::onColdSelected(bool& value) {
    if (value) {
        warm = false;
    }
}

void mainPanel::onShuffle()
{
    if (galleryRef) {
        resetFilters();
        galleryRef->shuffleImages();
    }
}

void mainPanel::draw()
{
	gui.draw();

}

void mainPanel::resetFilters()
{
    warm = false;
    cold = false;
    luminance = 0.0f;
    minTexture = 0.0f;
    maxTexture = 1.0f;
    numFaces = 0;
    duration = 0;
    numCuts = 0;
    temperatureBeh = false;
    cutsBeh = false;
}

