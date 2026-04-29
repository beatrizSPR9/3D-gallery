#include "mediaMetadata.h"
#include "imageMedia.h"
#include "ofxCv.h"
#include "ofxCvHaarFinder.h"

using namespace ofxCv;
using namespace cv;

void mediaMetadata::loadAndCreateImageMetaData(imageMedia &img, string xmlPath)
{
    ofxXmlSettings XML;
    if (XML.loadFile(xmlPath))
    {
        XML.pushTag("imageData");
        img.setFileName(XML.getValue("fileName", ""));
        img.setLuminanceAvg(XML.getValue("luminanceAvg", 0.0f));
        img.setLuminanceVar(XML.getValue("luminanceVar", 0.0f));
        img.setColorAvg(XML.getValue("colorRAvg", 0.0f), XML.getValue("colorGAvg", 0.0f), XML.getValue("colorBAvg", 0.0f));
        img.setColorVar(XML.getValue("colorRVar", 0.0f), XML.getValue("colorGVar", 0.0f), XML.getValue("colorBVar", 0.0f));
        img.setNumFaces(XML.getValue("numFaces", 0.0f));

        vector<string> tokens = ofSplitString(XML.getValue("edgeHistAvg", ""), " ");
        for (int i = 0; i < tokens.size(); i++)
        {
            img.setEdgeHistAvg(ofToFloat(tokens[i]), i);
        }

        tokens.clear();
        tokens = ofSplitString(XML.getValue("edgeHistVar", ""), " ");
        for (int i = 0; i < tokens.size(); i++)
        {
            img.setEdgeHistVar(ofToFloat(tokens[i]), i);
        }

        tokens.clear();
        tokens = ofSplitString(XML.getValue("textureHistAvg", ""), " ");
        for (int i = 0; i < tokens.size(); i++)
        {
            img.setTextureHistAvg(ofToFloat(tokens[i]), i);
        }

        tokens.clear();
        tokens = ofSplitString(XML.getValue("textureHistVar", ""), " ");
        for (int i = 0; i < tokens.size(); i++)
        {
            img.setTextureHistVar(ofToFloat(tokens[i]), i);
        }

        XML.popTag();

        detectAndCompute(img);
    }
    else
    {
        generateImageMetaData(img, xmlPath, XML);
    }
}

void mediaMetadata::generateImageMetaData(imageMedia &img, string xmlPath, ofxXmlSettings &XML)
{
    // Set file name
    img.setFileName(ofFilePath::getBaseName(xmlPath));

    // Calculate average luminance and color
    calculateLuminanceAndColorAvg(img);

    // Calculate luminance and color variance
    calculateLuminanceAndColorVar(img);

    // Calculate edge histogram (average and variance)
    calculateEdgeHistogram(img);

    // Calculate texture histogram (average and variance)
    calculateTextureHistogram(img);

    // Calculate number of faces
    calculateNumFaces(img);

    // Calculate keypoints and descriptores
    detectAndCompute(img);

    // Create metadata file
    XML.addTag("imageData");
    XML.pushTag("imageData");
    XML.addValue("fileName", img.getFileName());
    XML.addValue("luminanceAvg", img.getLuminanceAvg());
    XML.addValue("luminanceVar", img.getLuminanceVar());
    XML.addValue("colorRAvg", img.getColorAvg().x);
    XML.addValue("colorGAvg", img.getColorAvg().y);
    XML.addValue("colorBAvg", img.getColorAvg().z);
    XML.addValue("colorRVar", img.getColorVar().x);
    XML.addValue("colorGVar", img.getColorVar().y);
    XML.addValue("colorBVar", img.getColorVar().z);
    XML.addValue("numFaces", img.getNumFaces());
    XML.addValue("edgeHistAvg", ofToString(img.getEdgeHistAvg(0)) + " " + ofToString(img.getEdgeHistAvg(1)) + " " + ofToString(img.getEdgeHistAvg(2)) + " " +
                                    ofToString(img.getEdgeHistAvg(3)) + " " + ofToString(img.getEdgeHistAvg(4)));
    XML.addValue("edgeHistVar", ofToString(img.getEdgeHistVar(0)) + " " + ofToString(img.getEdgeHistVar(1)) + " " + ofToString(img.getEdgeHistVar(2)) + " " +
                                    ofToString(img.getEdgeHistVar(3)) + " " + ofToString(img.getEdgeHistVar(4)));

    string textureHistAvg = "";
    for (int i = 0; i < 24; i++)
    {
        textureHistAvg += ofToString(img.getTextureHistAvg(i)) + " ";
    }
    XML.addValue("textureHistAvg", textureHistAvg);

    string textureHistVar = "";
    for (int i = 0; i < 24; i++)
    {
        textureHistVar += ofToString(img.getTextureHistVar(i)) + " ";
    }
    XML.addValue("textureHistVar", textureHistVar);

    XML.popTag();
    XML.saveFile(xmlPath);
}

void mediaMetadata::calculateLuminanceAndColorAvg(imageMedia &img)
{

    ofImage *image = static_cast<ofImage *>(img.getMedia());
    ofPixels pixels = image->getPixels();

    float luminanceAvg = 0.0f;
    float rAvg = 0.0f;
    float gAvg = 0.0f;
    float bAvg = 0.0f;

    for (int x = 0; x < pixels.getHeight(); x++)
    {
        for (int y = 0; y < pixels.getWidth(); y++)
        {
            ofColor color = pixels.getColor(y, x);

            float luminance = 0.2125f * color.r + 0.7154f * color.g + 0.0721f * color.b;
            luminanceAvg += luminance;
            rAvg += color.r;
            gAvg += color.g;
            bAvg += color.b;
        }
    }

    int numPixels = pixels.getWidth() * pixels.getHeight();

    luminanceAvg /= numPixels;
    rAvg /= numPixels;
    gAvg /= numPixels;
    bAvg /= numPixels;

    img.setLuminanceAvg(luminanceAvg);
    img.setColorAvg(rAvg, gAvg, bAvg);
}

void mediaMetadata::calculateLuminanceAndColorVar(imageMedia &img)
{

    ofImage *image = static_cast<ofImage *>(img.getMedia());
    ofPixels pixels = image->getPixels();

    float luminanceVar = 0.0f;
    float rVar = 0.0f;
    float gVar = 0.0f;
    float bVar = 0.0f;

    float luminanceAvg = img.getLuminanceAvg();
    float rAvg = img.getColorAvg().x;
    float gAvg = img.getColorAvg().y;
    float bAvg = img.getColorAvg().z;

    for (int x = 0; x < pixels.getHeight(); x++)
    {
        for (int y = 0; y < pixels.getWidth(); y++)
        {
            ofColor color = pixels.getColor(y, x);
            float luminance = 0.2125f * color.r + 0.7154f * color.g + 0.0721f * color.b;

            float diffLum = luminance - luminanceAvg;
            luminanceVar += diffLum * diffLum;

            float diffR = color.r - rAvg;
            float diffG = color.g - gAvg;
            float diffB = color.b - bAvg;
            rVar += diffR * diffR;
            gVar += diffG * diffG;
            bVar += diffB * diffB;
        }
    }

    int numPixels = pixels.getWidth() * pixels.getHeight();
    luminanceVar /= numPixels;
    rVar /= numPixels;
    gVar /= numPixels;
    bVar /= numPixels;

    img.setLuminanceVar(luminanceVar);
    img.setColorVar(rVar, gVar, bVar);
}

void mediaMetadata::calculateEdgeHistogram(imageMedia &img)
{

    vector<Mat> masks;
    masks.assign(5, Mat());

    // Define masks
    Mat vertical = (Mat_<float>(2, 2) << 1, -1,
                    1, -1);
    masks[0] = vertical;

    Mat horizontal = (Mat_<float>(2, 2) << 1, 1,
                      -1, -1);
    masks[1] = horizontal;

    Mat diagonal45 = (Mat_<float>(2, 2) << sqrt(2), 0,
                      0, -sqrt(2));
    masks[2] = diagonal45;

    Mat diagonal135 = (Mat_<float>(2, 2) << 0, sqrt(2),
                       -sqrt(2), 0);
    masks[3] = diagonal135;

    Mat nonDir = (Mat_<float>(2, 2) << 2, -2,
                  -2, 2);
    masks[4] = nonDir;

    // Convert ofImage to Mat
    ofImage *image = static_cast<ofImage *>(img.getMedia());
    Mat mat = toCv(*image);

    // Convert to grayscale
    Mat gray;
    cvtColor(mat, gray, COLOR_RGB2GRAY);

    for (int i = 0; i < masks.size(); i++)
    {

        // Apply filter2D
        Mat edges;
        filter2D(gray, edges, CV_8U, masks[i]);

        // Convert back to ofImage
        ofImage result;
        toOf(edges, result);
        result.update();

        // Save result
        string filePath = "edges/" + to_string(i) + "edges.png";
        result.save(filePath);

        float avg = calculateEdgeHistogramAvg(result);
        img.setEdgeHistAvg(avg, i);
        img.setEdgeHistVar(calculateEdgeHistogramVar(result, avg), i);
    }
}

float mediaMetadata::calculateEdgeHistogramAvg(ofImage img)
{
    ofPixels pixels = img.getPixels();

    float total = pixels.getHeight() * pixels.getWidth();

    float numPixelsBorderAvg = 0.0;

    for (int x = 0; x < pixels.getHeight(); x++)
    {
        for (int y = 0; y < pixels.getWidth(); y++)
        {
            ofColor color = pixels.getColor(y, x);

            float val = (color != ofColor(0, 0, 0)) ? 1.0f : 0.0f;
            numPixelsBorderAvg += val;
        }
    }
    return numPixelsBorderAvg /= total;
}

float mediaMetadata::calculateEdgeHistogramVar(ofImage img, float average)
{

    ofPixels pixels = img.getPixels();

    float total = pixels.getHeight() * pixels.getWidth();

    float numPixelsBorderVar = 0.0;

    for (int x = 0; x < pixels.getHeight(); x++)
    {
        for (int y = 0; y < pixels.getWidth(); y++)
        {
            ofColor color = pixels.getColor(y, x);

            float val = (color != ofColor(0, 0, 0)) ? 1.0f : 0.0f;

            float diffLum = val - average;
            numPixelsBorderVar += diffLum * diffLum;
        }
    }
    return numPixelsBorderVar /= total;
}

void mediaMetadata::calculateTextureHistogram(imageMedia &img)
{
    // Convert ofImage to Mat
    ofImage *image = static_cast<ofImage *>(img.getMedia());
    Mat mat = toCv(*image);

    // Convert to grayscale
    Mat gray;
    cvtColor(mat, gray, COLOR_RGB2GRAY);

    // Parameters
    int kernelSize = 21;
    double sigma = 5.0;
    double gamma = 0.5;
    double psi = 0;

    // Frequencies
    std::vector<double> lambdas = {4.0, 8.0, 12.0, 16.0};

    // 6 orientations: 0, 30, 60, 90, 120, 150 degrees
    for (int i = 0; i < 6; i++)
    {

        double theta = i * CV_PI / 6; // Convert degrees to radians

        for (int l = 0; l < lambdas.size(); l++)
        {

            // Create Gabor kernel
            Mat kernel = getGaborKernel(cv::Size(kernelSize, kernelSize),
                                        sigma, theta, lambdas[l], gamma, psi, CV_32F);

            // Apply filter
            Mat filtered;
            filter2D(gray, filtered, CV_32F, kernel);

            normalize(filtered, filtered, 0, 255, NORM_MINMAX, CV_8U);

            // Convert back to ofImage
            ofImage result;
            toOf(filtered, result);
            result.update();

            // Save result
            string filePath = "textures/" + to_string(i) + "_" + to_string(l) + "textures.png";
            result.save(filePath);

            string aux = img.getFileName();

            float avg = calculateTextureHistogramAvg(result);
            int index = i * lambdas.size() + l;
            img.setTextureHistAvg(avg, index);
            img.setTextureHistVar(calculateTextureHistogramVar(result, avg), index);
        }
    }
}

float mediaMetadata::calculateTextureHistogramAvg(ofImage img)
{

    ofPixels pixels = img.getPixels();
    int numPixels = pixels.getWidth() * pixels.getHeight();

    vector<int> histogram(256, 0);
    int maxCount = 0;
    int dominantGray = 0;

    for (int i = 0; i < numPixels; i++)
    {

        unsigned char brightness = pixels[i];
        histogram[brightness]++;

        if (histogram[brightness] > maxCount)
        {
            maxCount = histogram[brightness];
            dominantGray = brightness;
        }
    }

    // Find num of pixels different from dominant gray
    float sum = 0.0f;

    for (int i = 0; i < numPixels; i++)
    {

        unsigned char brightness = pixels[i];
        float val = (brightness != dominantGray) ? 1.0f : 0.0f;
        sum += val;
    }

    return sum / numPixels;
}

float mediaMetadata::calculateTextureHistogramVar(ofImage img, float average)
{
    ofPixels pixels = img.getPixels();
    int numPixels = pixels.getWidth() * pixels.getHeight();

    // Find dominant gray
    vector<int> histogram(256, 0);
    int maxCount = 0;
    int dominantGray = 0;

    for (int i = 0; i < numPixels; i++)
    {

        unsigned char brightness = pixels[i];
        histogram[brightness]++;

        if (histogram[brightness] > maxCount)
        {
            maxCount = histogram[brightness];
            dominantGray = brightness;
        }
    }

    // Find variance
    float variance = 0.0f;

    for (int i = 0; i < numPixels; i++)
    {

        unsigned char brightness = pixels[i];
        float val = (brightness != dominantGray) ? 1.0f : 0.0f;
        float diff = val - average;
        variance += diff * diff;
    }

    return variance / numPixels;
}

void mediaMetadata::calculateNumFaces(imageMedia &img)
{
    ofImage *image = static_cast<ofImage *>(img.getMedia());

    ofxCvHaarFinder finder;
    finder.setup("haarcascade_frontalface_default.xml");
    finder.findHaarObjects(*image);

    img.setNumFaces(finder.blobs.size());
}

void mediaMetadata::detectAndCompute(imageMedia &img)
{
    ofImage grayScaleImage = *static_cast<ofImage *>(img.getMedia());
    grayScaleImage.setImageType(OF_IMAGE_GRAYSCALE);
    Mat mat = toCv(grayScaleImage);
    cv::Ptr<ORB> orb = ORB::create();
    vector<KeyPoint> kpts;
    Mat desc;
    orb->detectAndCompute(mat, Mat(), kpts, desc);

    cv::Mat keypointsImg;
    cv::drawKeypoints(mat, kpts, keypointsImg, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);
    ofImage result;
    toOf(keypointsImg, result);
    result.update();

    // Save result
    string filePath = "keypoints/" + img.getFileName() + ".png";
    result.save(filePath);

    img.setDescriptors(desc);
}

//-------------------------------------------------------------- VIDEOS -----------------------------------------------------------------
void mediaMetadata::loadAndCreateVideoMetaData(videoMedia &vid, string xmlPath)
{
    ofxXmlSettings XML;
    if (XML.loadFile(xmlPath))
    {
        XML.pushTag("videoData");
        vid.setFileName(XML.getValue("fileName", ""));
        vid.setDuration(XML.getValue("duration", 0.0f));
        vid.setNumCuts(XML.getValue("numCuts", 0.0f));
        XML.popTag();
    }
    else
    {
        generateVideoMetaData(vid, xmlPath, XML);
    }
}

void mediaMetadata::generateVideoMetaData(videoMedia &vid, string xmlPath, ofxXmlSettings &XML)
{

    ofVideoPlayer *video = static_cast<ofVideoPlayer *>(vid.getMedia());

    vid.setFileName(ofFilePath::getBaseName(xmlPath));

    // Calculate number of cuts
    calculateCuts(vid);

    // Calculate duration
    vid.setDuration(video->getDuration());

    // Create metadata file
    XML.addTag("videoData");
    XML.pushTag("videoData");
    XML.addValue("fileName", vid.getFileName());
    XML.addValue("duration", vid.getDuration());
    XML.addValue("numCuts", vid.getNumCuts());
    XML.popTag();
    XML.saveFile(xmlPath);
}

void mediaMetadata::calculateCuts(videoMedia &vid)
{
    // Create a temporary ofVideoPlayer
    ofVideoPlayer tempVideo;

    ofVideoPlayer* video = static_cast<ofVideoPlayer*>(vid.getMedia());
    
    std::string videoPath = video->getMoviePath();  
    if (!tempVideo.load(videoPath)) {
        ofLogError() << "Failed to load video for cut detection: " << videoPath;
        return;
    }

    tempVideo.setLoopState(OF_LOOP_NONE);
    tempVideo.play(); 
    tempVideo.setPaused(true);  

    // Wait for the first frame to be loaded
    while (!tempVideo.isLoaded() || !tempVideo.getPixels().isAllocated()) {
        tempVideo.update();
        ofSleepMillis(10);
    }

    float numCuts = 0.0f;
    float cutThreshold = 0.3f;
    bool hasPrev = false;
    int stepFrames = 5;

    ofImage prevFrame, currentFrame;
    vector<int> histogram1(256, 0), histogram2(256, 0);

    int totalFrames = tempVideo.getTotalNumFrames();

    for (int i = stepFrames; i < totalFrames; i+=stepFrames) {
        tempVideo.setFrame(i);
        tempVideo.update();

        // Wait for the new frame to be ready
        int tries = 0;
        while (!tempVideo.isFrameNew() && tries++ < 10) {
            tempVideo.update();
            ofSleepMillis(5);
        }

        if (!tempVideo.getPixels().isAllocated()) continue;

        if (!hasPrev) {
            prevFrame.setFromPixels(tempVideo.getPixels());
            hasPrev = true;
        }
        else {
            currentFrame.setFromPixels(tempVideo.getPixels());
            float diff = calculateDiff(prevFrame, currentFrame, histogram1, histogram2);
            if (diff > cutThreshold) {
                numCuts++;
            }
            prevFrame.setFromPixels(currentFrame.getPixels());
        }
    }

    vid.setNumCuts(numCuts);

    // Cleanup (not strictly necessary as tempVideo will go out of scope)
    tempVideo.close();
}

float mediaMetadata::calculateDiff(ofImage &prevFrame, ofImage &currFrame, vector<int> &hist1, vector<int> &hist2)
{
    calculateHistogram(prevFrame.getPixels(), hist1);
    calculateHistogram(currFrame.getPixels(), hist2);

    float sum = 0.0f;
    for (int i = 0; i < 256; i++) {
        sum += abs(hist1[i] - hist2[i]);
    }

    float totalPixels = prevFrame.getPixels().getHeight() * prevFrame.getPixels().getWidth();

    return sum / totalPixels;
}

void mediaMetadata::calculateHistogram(const ofPixels &frame, vector<int> &hist)
{

    if (hist.size() != 256) {
        hist.resize(256, 0);
    }
    else {
        fill(hist.begin(), hist.end(), 0); 
    }

    // Convert to grayscale
    ofImage grayFrame;
    grayFrame.setFromPixels(frame);
    grayFrame.setImageType(OF_IMAGE_GRAYSCALE);

    const ofPixels& grayPixels = grayFrame.getPixels();
    int numPixels = grayPixels.size(); 

    for (int i = 0; i < numPixels; i++) {
        hist[grayPixels[i]]++;
    }
}
