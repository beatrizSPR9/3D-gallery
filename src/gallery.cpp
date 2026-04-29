#include "gallery.h"
#include "mediaMetadata.h"
#include "ofxCv.h"

using namespace cv;

void gallery::setupRoom()
{
    ofSetVerticalSync(true);
    ofEnableDepthTest();

    // Setup camera
    cam.setPosition(0, 0, 200);
    cam.lookAt(glm::vec3(0, 0, 0));
    cam.setNearClip(10.0);

    // Setup room dimensions
    roomWidth = 1000;
    roomHeight = 600;
    roomDepth = 1000;

    // Setup lighting
    pointLight.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
    pointLight.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    pointLight.setPosition(0, roomHeight, 200);
    pointLight.enable();

    // Setup wall textures
    ofImage brickImage;
    ofImage floorImage;
    ofImage ceilingImage;

    brickImage.load("WallGray.jpg");
    floorImage.load("Floor2.jpg");
    ceilingImage.load("Ceiling.jpg");
    brickTexture = brickImage.getTexture();
    floorTexture = floorImage.getTexture();
    ceilingTexture = ceilingImage.getTexture();

    wallPlane.set(roomWidth, roomHeight);
    wallPlane.mapTexCoordsFromTexture(brickTexture);

    floorPlane.set(roomWidth, roomDepth);
    floorPlane.mapTexCoordsFromTexture(floorTexture);

    ceilingPlane.set(roomWidth, roomDepth);
    ceilingPlane.mapTexCoordsFromTexture(ceilingTexture);

    // Setup main panel (filters and behaviours)
    infoPanel.setup();
}

void gallery::setupImages(ofDirectory dir)
{
    if (dir.size())
    {
        images.assign(dir.size(), imageMedia()); // All images
        imagesToShow.assign(12, imageMedia());   // Images on wall
    }

    for (int i = 0; i < dir.size(); i++)
    {
        imageMedia image;

        // 1. Load raw image
        ofImage img;
        string imagePath = dir.getPath(i);
        img.load(imagePath);
        image.setImage(img);
        image.setIndex(i);

        // 2. Load or create metadata
        string xmlPath = "xmlFiles/imageXML/" + ofFilePath::getBaseName(imagePath) + ".xml";
        mediaMetadata metadata;
        metadata.loadAndCreateImageMetaData(image, xmlPath);
        images[i] = image;
        if (i < 12)
            imagesToShow[i] = image;
    }
}

void gallery::setupVideos(ofDirectory dir)
{
    if (dir.size())
    {
        videos.assign(dir.size(), videoMedia());
        videosToShow.assign(6, videoMedia()); // Videos on wall
    }

    for (int i = 0; i < dir.size(); i++)
    {
        videoMedia video;

        // 1. Load video
        ofVideoPlayer v;
        string videoPath = dir.getPath(i);
        v.load(videoPath);
        v.setLoopState(OF_LOOP_NORMAL);
        v.update();
        video.setVideo(v);
        video.setIndex(i);

        // 2. Load metadata
        string xmlPath = "xmlFiles/videoXML/" + ofFilePath::getBaseName(videoPath) + ".xml";
        mediaMetadata metadata;
        metadata.loadAndCreateVideoMetaData(video, xmlPath);
        videos[i] = video;
        if (i < 6)
            videosToShow[i] = video;
    }
}

void gallery::update()
{
    float deltaTime = ofGetLastFrameTime();
    for (int i = 0; i < imagesToShow.size(); i++)
    {
        if (imagesToShow[i].getMotionSpeed() > 0.0f)
        {
            imagesToShow[i].updateMotion(deltaTime);
        }
    }

    for (int i = 0; i < videosToShow.size(); i++)
    {
        if (videosToShow[i].getMotionSpeed() > 0.0f)
        {
            videosToShow[i].updateMotion(deltaTime);
        }
    }

    if (infoPanel.getShowSimilar()) {
        filterSimilarImages();
    }

}

void gallery::calculateImageSize()
{
    float windowW = roomWidth;
    float windowH = roomHeight;

    // Start positions
    int spacing = 50;
    int ncols = 4;
    int nrows = 3;

    int xSize = (windowW - (ncols + 1) * spacing) / ncols;
    int ySize = (windowH - (nrows + 1) * spacing) / nrows;

    float size = std::min(xSize, ySize);

    float startX = (windowW - (ncols * size + (ncols - 1) * spacing)) / 2;
    float startY = (windowH - (nrows * size + (nrows - 1) * spacing)) / 2;

    float x = -windowW / 2 + startX;
    float y = windowH / 2 - startY;

    for (int i = 0; i < imagesToShow.size(); i++)
    {
        if (imagesToShow[i].getMotionSpeed() == 0.0f)
        {
            imagesToShow[i].setPosition(x + size / 2, y - size / 2, -roomDepth / 2 + 2);
            imagesToShow[i].setSize(size);

            x += size + spacing;
            if ((i + 1) % ncols == 0)
            {
                x = -windowW / 2 + startX;
                y -= size + spacing;
            }
        }
    }
}

void gallery::calculateVideoSize()
{
    float windowW = roomWidth;
    float windowH = roomHeight;

    // Start positions
    int spacing = 50;
    int ncols = 3;
    int nrows = 2;

    int xSize = (windowW - (ncols + 1) * spacing) / ncols;
    int ySize = (windowH - (nrows + 1) * spacing) / nrows;

    float size = std::min(xSize, ySize);

    float startX = (windowW - (ncols * size + (ncols - 1) * spacing)) / 2;
    float startY = (windowH - (nrows * size + (nrows - 1) * spacing)) / 2;

    float x = -windowW / 2 + startX;
    float y = windowH / 2 - startY;

    for (int i = 0; i < videosToShow.size(); i++)
    {
        if (videosToShow[i].getMotionSpeed() == 0.0f)
        {
            videosToShow[i].setPosition(x + size / 2, y - size / 2, roomDepth / 2 - 2);
            videosToShow[i].setSize(size);

            x += size + spacing;
            if ((i + 1) % ncols == 0)
            {
                x = -windowW / 2 + startX;
                y -= size + spacing;
            }
        }
    }
}

void gallery::drawSelectedMedia()
{
    if (!selectedMedia)
        return;

    infoPanel.update(selectedMedia);

    // Create an overlay
    ofSetColor(0, 0, 0, 150);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255);

    // Draw media
    selectedMedia->drawSelectedMedia(&infoPanel);
}

void gallery::handleMousePressed(int x, int y, int button)
{

    if (selectedMedia == NULL)
    {
        // Direction camera is facing
        glm::vec3 lookDir = glm::normalize(cam.getLookAtDir());

        // Normal of each wall
        glm::vec3 pictureWallNormal = glm::vec3(0, 0, -1);
        glm::vec3 videoWallNormal = glm::vec3(0, 0, 1);

        // Check which wall camera is facing based on dot product
        float facingPictures = glm::dot(lookDir, pictureWallNormal);
        float facingVideos = glm::dot(lookDir, videoWallNormal);
        float closestDist = std::numeric_limits<float>::max();

        if (facingPictures > 0.8f)
        {
            for (int i = 0; i < imagesToShow.size(); i++)
            {
                glm::vec3 screenPos = cam.worldToScreen(imagesToShow[i].getPosition());

                float worldSize = imagesToShow[i].getSize(); 
                glm::vec3 rightEdge = imagesToShow[i].getPosition() + glm::vec3(worldSize / 2.0, 0, 0);
                float screenSize = abs(cam.worldToScreen(rightEdge).x - screenPos.x); 

                float dx = x - screenPos.x;
                float dy = y - screenPos.y;
                float dist = sqrt(dx * dx + dy * dy);

                if (dist < screenSize)
                {
                    if (dist < closestDist)
                    {
                        closestDist = dist;
                        selectedMediaIndex = i;
                        selectedMedia = make_shared<imageMedia>(imagesToShow[i]);
                    }
                }
            }
        }
        else if (facingVideos > 0.8f)
        {
            for (int i = 0; i < videosToShow.size(); i++)
            {
                glm::vec3 screenPos = cam.worldToScreen(videosToShow[i].getPosition());

                
                float worldSize = videosToShow[i].getSize();
                glm::vec3 rightEdge = videosToShow[i].getPosition() + glm::vec3(worldSize / 2.0, 0, 0);
                float screenSize = abs(cam.worldToScreen(rightEdge).x - screenPos.x); 

                float dx = x - screenPos.x;
                float dy = y - screenPos.y;
                float dist = sqrt(dx * dx + dy * dy);

                if (dist < screenSize)
                {
                    if (dist < closestDist)
                    {
                        closestDist = dist;
                        selectedMediaIndex = i;
                        selectedMedia = make_shared<videoMedia>(videosToShow[i]);
                    }
                }
            }
        }
    }
    else
    {
        selectedMediaIndex = -1;
        selectedMedia = NULL;
    }
}

void gallery::drawRoom()
{
    ofBackground(20);
    cam.begin();

    // Ceiling
    ceilingTexture.bind();
    ofPushMatrix();
    ofTranslate(0, roomHeight / 2, 0);
    ofRotateXDeg(90);
    ceilingPlane.draw();
    ofPopMatrix();
    ceilingTexture.unbind();

    // Floor
    floorTexture.bind();
    ofPushMatrix();
    ofTranslate(0, -roomHeight / 2, 0);
    ofRotateXDeg(-90);
    floorPlane.draw();
    ofPopMatrix();
    floorTexture.unbind();

    // Back wall
    brickTexture.bind();
    ofPushMatrix();
    ofTranslate(0, 0, -roomDepth / 2);
    wallPlane.draw();
    ofPopMatrix();

    // Front wall
    ofPushMatrix();
    ofTranslate(0, 0, roomDepth / 2);
    ofRotateYDeg(180);
    wallPlane.draw();
    ofPopMatrix();
    brickTexture.unbind();

    // Left wall
    brickTexture.bind();
    ofPushMatrix();
    ofTranslate(-roomWidth / 2, 0, 0);
    ofRotateYDeg(90);
    wallPlane.draw();
    ofPopMatrix();

    // Right wall
    ofPushMatrix();
    ofTranslate(roomWidth / 2, 0, 0);
    ofRotateYDeg(-90);
    wallPlane.draw();
    ofPopMatrix();
    brickTexture.unbind();

    // Draw images on the wall
    calculateImageSize();
    drawImages();

    // Draw videos on the wall
    calculateVideoSize();
    drawVideos();

    cam.end();
}

void gallery::drawImages()
{
    for (int i = 0; i < imagesToShow.size(); i++)
    {
        imagesToShow[i].draw();
        if (i == 11)
            break;
    }
}

void gallery::drawVideos()
{
    for (int i = 0; i < videosToShow.size(); i++)
    {
        videosToShow[i].draw();
    }
}

void gallery::filterByColor(string type)
{
    imagesToShow.clear();
    for (int i = 0; i < images.size(); i++)
    {
        ofColor colorAvg = ofColor(images[i].getColorAvg().x, images[i].getColorAvg().y, images[i].getColorAvg().z);
        float hue = colorAvg.getHue();
        if (type == "cold" && hue >= 60.0f && hue <= 240.0f)
        {
            imageMedia img = images[i];
            img.setIndex(i);
            imagesToShow.push_back(img);
        }
        if (type == "warm" && (hue < 60.0f || hue > 240.0f))
        {
            imageMedia img = images[i];
            img.setIndex(i);
            imagesToShow.push_back(img);
        }
    }
}

void gallery::filterByLuminance(float luminance)
{
    imagesToShow.clear();
    for (int i = 0; i < images.size(); i++)
    {
        float lum = images[i].getLuminanceAvg();
        if (lum >= luminance)
        {
            imageMedia img = images[i];
            img.setIndex(i);
            imagesToShow.push_back(img);
        }
    }
}

void gallery::filterByNumFaces(int numFaces)
{
    imagesToShow.clear();
    for (int i = 0; i < images.size(); i++)
    {
        float faces = images[i].getNumFaces();
        if (faces >= numFaces)
        {
            imageMedia img = images[i];
            img.setIndex(i);
            imagesToShow.push_back(img);
        }
    }
}

void gallery::filterByDuration(int duration)
{
    videosToShow.clear();
    for (int i = 0; i < videos.size(); i++)
    {
        float dur = videos[i].getDuration();
        if (dur >= duration)
        {
            videoMedia vid = videos[i];
            vid.setIndex(i);
            videosToShow.push_back(vid);
        }
    }
}

void gallery::filterByNumCuts(int numCuts)
{
    videosToShow.clear();
    for (int i = 0; i < videos.size(); i++)
    {
        float cuts = videos[i].getNumCuts();
        if (cuts >= numCuts)
        {
            videoMedia vid = videos[i];
            vid.setIndex(i);
            videosToShow.push_back(vid);
        }
    }
}

void gallery::filterByTexture(float minTexture, float maxTexture)
{
    imagesToShow.clear();
    for (int i = 0; i < images.size(); i++)
    {
        float avg = 0.0f;
        for(int j= 0; j < 24 ; j++)
            avg += images[i].getTextureHistAvg(j);

        avg /= 24;

        if (minTexture <= avg && avg <= maxTexture)
        {
            imageMedia img = images[i];
            img.setIndex(i);
            imagesToShow.push_back(img);
        }
    }
}

void gallery::filterSimilarImages()
{
    if (selectedMedia == NULL) return;

    auto selectedImage = dynamic_pointer_cast<imageMedia>(selectedMedia);

    if (selectedImage) {
        imageMedia& refImage = *selectedImage; 

        vector<imageMedia> sortedImages;

        for (auto& img : images) {
                sortedImages.push_back(img);
        }


        sort(sortedImages.begin(), sortedImages.end(),
            [this, &refImage](imageMedia& a, imageMedia& b) {
                return getSimilarity(refImage, a) > getSimilarity(refImage, b);
            });

        imagesToShow.clear();
        for (int i = 0; i < sortedImages.size(); i++) {
            imagesToShow.push_back(sortedImages[i]);
        }

        selectedMedia = NULL;
        infoPanel.setShowingSimilar(false);
    }
}

void gallery::removeFilters()
{
    imagesToShow.clear();
    for (int i = 0; i < 12; i++)
    {
        imageMedia img = images[i];
        img.setIndex(i);
        imagesToShow.push_back(img);
    }

    videosToShow.clear();
    for (int i = 0; i < 6; i++)
    {
        videoMedia vid = videos[i];
        vid.setIndex(i);
        videosToShow.push_back(vid);
    }
}

void gallery::shuffleImages()
{
    vector<imageMedia> shuffle = images;
    std::shuffle(shuffle.begin(), shuffle.end(), std::default_random_engine(ofGetElapsedTimeMillis()));

   
    imagesToShow.clear();
    for (int i = 0; i < 12; i++) {
        imagesToShow.push_back(shuffle[i]);
        imagesToShow[i].setIndex(i);
    }
}

void gallery::handleTemperatureBehaviour(bool state)
{
    if (state)
    {
        for (int i = 0; i < imagesToShow.size(); i++)
        {
            ofVec3f p = imagesToShow[i].getPosition();
            imagesToShow[i].setMotionCenter(p);
            imagesToShow[i].setMotionRadius(10);

            ofColor colorAvg = ofColor(imagesToShow[i].getColorAvg().x, imagesToShow[i].getColorAvg().y, imagesToShow[i].getColorAvg().z);
            float hue = colorAvg.getHue();
            if (hue >= 60.0f && hue <= 240.0f)
                imagesToShow[i].setMotionSpeed(1.0f);
            else if (hue < 60.0f || hue > 240.0f)
                imagesToShow[i].setMotionSpeed(4.0f);
        }
    }
    else
    {
        for (int i = 0; i < imagesToShow.size(); i++)
        {
            imagesToShow[i].setMotionSpeed(0.0f);
        }
    }
}

void gallery::handleCutsBehaviour(bool state)
{
    if (state)
    {
        for (int i = 0; i < videosToShow.size(); i++)
        {
            ofVec3f p = videosToShow[i].getPosition();
            videosToShow[i].setMotionCenter(p);
            videosToShow[i].setMotionRadius(10);

            float numCuts = videosToShow[i].getNumCuts();

            float maxCuts = 20.0f; 
            float minSpeed = 1.0f;
            float maxSpeed = 5.0f;

            float speed = ofMap(numCuts, 0.0f, maxCuts, minSpeed, maxSpeed, true);
            videosToShow[i].setMotionSpeed(speed);
        }
    }
    else
    {
        for (int i = 0; i < videosToShow.size(); i++)
        {
            videosToShow[i].setMotionSpeed(0.0f);
        }
    }
}

void gallery::handlePlayOrPauseVideos(bool state)
{
    if (state)
    {
        for (int i = 0; i < videosToShow.size(); i++)
        {
            videosToShow[i].playVideo();
        }
    }
    else
    {
        for (int i = 0; i < videosToShow.size(); i++)
        {
            videosToShow[i].pauseVideo();
        }
    }
}

shared_ptr<media> gallery::getSelectedMedia() const
{
    return selectedMedia;
}

void gallery::setSelected(int i)
{
    if (selectedMedia != NULL)
    {
        auto video = dynamic_pointer_cast<videoMedia>(selectedMedia);
        if (video)
        {
            selectedMediaIndex += i;

            if (selectedMediaIndex < 0)
                selectedMediaIndex = videosToShow.size() - 1;
            else if (selectedMediaIndex >= videosToShow.size())
                selectedMediaIndex = 0;

            selectedMedia = make_shared<videoMedia>(videosToShow[selectedMediaIndex]);
        }

        auto image = dynamic_pointer_cast<imageMedia>(selectedMedia);
        if (image)
        {
            selectedMediaIndex += i;

            if (selectedMediaIndex < 0)
                selectedMediaIndex = imagesToShow.size() - 1;
            else if (selectedMediaIndex >= imagesToShow.size())
                selectedMediaIndex = 0;

            selectedMedia = make_shared<imageMedia>(imagesToShow[selectedMediaIndex]);
        }
    }
}

int gallery::getSimilarity(imageMedia& reference, imageMedia& image)
{
    BFMatcher matcher(cv::NORM_HAMMING);
    vector<DMatch> matches;

    cv::Mat desc1 = reference.getDescriptors();
    cv::Mat desc2 = image.getDescriptors();

    if (desc1.empty() || desc2.empty()) {
        return 0;
    }

    matcher.match(desc1, desc2, matches);

    int goodMatches = 0;
    for (auto& match : matches) {
        if (match.distance < 70) goodMatches++;
    }

    return goodMatches;
}
