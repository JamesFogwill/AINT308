#include <Detection.h>

Detection::Detection(){

};

Mat Detection::generateCanny(Mat Frame){

    // converts to gray and blurs the image to remove some background edges
    cvtColor(Frame, grayFrame, COLOR_BGR2GRAY);
    GaussianBlur(grayFrame,gaussianFrame, Size(5,5),0); // Try more gaussian blur to remove background edges.
    Mat kernal = getStructuringElement(MORPH_RECT, Size(5,5));
    Canny(Frame,cannyFrame,175, 240);
    // applied dilate and erode to a object, removes small edges and connects gaps in larger edges
    morphologyEx(cannyFrame, closedFrame,MORPH_CLOSE,kernal);

    return closedFrame;
}
;
Mat Detection::generateMask(Mat cannyFrame, vector<vector<Point>> vectorPolygons){

    // creates the mask and fills with zeros
    Mat Mask = Mat::zeros(cannyFrame.size(),cannyFrame.type());     // maybe i can erode if i put the erode mat type in here and erode before this?
    //creates an area of block colour to go over the mask
    Scalar ignoreMaskColour = Scalar(255,255,255);
    // fills the roi with block white
    fillPoly(Mask,vectorPolygons,ignoreMaskColour);
    // shows the canny image only within the mask
    bitwise_and(Mask,cannyFrame, maskedFrame);
    imshow("mask", Mask);

    return maskedFrame;

};



