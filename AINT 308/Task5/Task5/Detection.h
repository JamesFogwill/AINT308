#ifndef DETECTION_H
#define DETECTION_H
#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
// class for functions used in detection of the road lines
class Detection{

    private:

    Mat grayFrame, gaussianFrame, cannyFrame, closedFrame;
    Mat maskedFrame;

    protected:

    public:

    Detection();

    Mat generateCanny(Mat);

    Mat generateMask(Mat, vector<vector<Point>>);





};
#endif // DETECTION_H
