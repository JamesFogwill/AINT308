#include <Detection.h>

Detection::Detection(){

};

Mat Detection::generateCanny(Mat Frame){

    // converts to gray and blurs the image to remove some background edges
    cvtColor(Frame, grayFrame, COLOR_BGR2GRAY);
    GaussianBlur(grayFrame,gaussianFrame, Size(5,5),-1,-1); // Try more gaussian blur to remove background edges.
    // creates 5x5 kernal for grouping of pixels in morphological operations
    Mat kernal = getStructuringElement(MORPH_RECT, Size(5,5));
    // detects the edges in frame and stores in cannyFrame within the thresholds
    Canny(Frame,cannyFrame,100, 150);
    // applied dilate and erode to a object, removes small edges and connects gaps in larger edges
    morphologyEx(cannyFrame, closedFrame,MORPH_CLOSE,kernal);
    //return the cannny Mat
    //imshow("CannyFrame", closedFrame);
    return closedFrame;
};

// function to take the points of which you want your mask to cover and creates it
Mat Detection::generateMask(Mat cannyFrame, vector<vector<Point>> vectorPolygons){

    // creates the mask and fills with zeros
    Mat Mask = Mat::zeros(cannyFrame.size(),cannyFrame.type());
    //creates a white block colour to go over the mask
    Scalar ignoreMaskColour = Scalar(255,255,255);
    // fills the roi with block white
    fillPoly(Mask,vectorPolygons,ignoreMaskColour);
    // uses binary to compare the whites of the canny edges with the whites of the mask
    //this allows only the (1,1) pixels to be seen on the mask Mat
    bitwise_and(Mask,cannyFrame, maskedFrame);
    // returns the canny within the ROI
    //imshow("Polygon Mask", Mask);
    imshow("Masked Canny Frame", maskedFrame);
    return maskedFrame;

};



