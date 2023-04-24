//James Rogers Mar 2022 (c) Plymouth University
#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//a drawing function that can draw a line based on rho and theta values.
//useful for drawing lines from the hough line detector.
void lineRT(Mat &Src, Vec2f L, Scalar color, int thickness){
    Point pt1, pt2;
    double a = cos(static_cast<double>(L[1]));
    double b = sin(static_cast<double>(L[1]));
    double x0 = a*static_cast<double>(L[0]);
    double y0 = b*static_cast<double>(L[0]);
    pt1.x = cvRound(x0 + 10000*(-b));
    pt1.y = cvRound(y0 + 10000*(a));
    pt2.x = cvRound(x0 - 10000*(-b));
    pt2.y = cvRound(y0 - 10000*(a));
    line(Src, pt1, pt2, color, thickness, LINE_AA);
}

int main()
{

    //Open video file
    VideoCapture CarVideo("../Task5/DashCam.mp4");
    if(!CarVideo.isOpened()){
        cout<<"Error opening video"<<endl;
        return -1;
    }

    float rhoRes = 1;
    float thetaRes = M_PI/180;

    int Threshold = 150;

    // creates polygon where we will be detecting the lines
    //top left, top right, bottom right,bottom left
    //this is one polygon
    vector<Point>points = {Point(589,404),Point(747,404),Point(1090,710),Point(200,710)};
    // this is a vector of polygons, only necessary for me because the fillPoly function takes this type as input
    vector<vector<Point>>vectorPolygons = {points};

    //main program loop
    while(true){

        //open the next frame from the video file, or exit the loop if its the end
        Mat Frame, grayFrame, gaussianFrame, cannyFrame, erodedFrame,dilatedFrame, houghFrame;

        vector<Vec2f>lines;

        CarVideo.read(Frame);
        if(Frame.empty()){
            break;
        }

        //==========================Your code goes here==========================

        cvtColor(Frame, grayFrame, COLOR_BGR2GRAY);
        GaussianBlur(grayFrame,gaussianFrame, Size(1,1),0,0); // Try more gaussian blur to remove background edges.
        Mat kernal = getStructuringElement(MORPH_RECT, Size(3,3));
        Canny(Frame,cannyFrame,175, 240);

        // erode removes small canny edges which are noise, dilate increases the size of edges to fill in gaps
        // erode and dilate code crash the bitwise and hough parts of my code

        // creates the mask and fills with zeros
        Mat Mask = Mat::zeros(cannyFrame.size(),cannyFrame.type());
        //creates an area of block colour to go over the mask
        Scalar ignoreMaskColour = Scalar(255,255,255);
        // fills the roi with block white
        fillPoly(Mask,vectorPolygons,ignoreMaskColour);
        // shows the canny image only within the mask
        Mat maskedImage;
        bitwise_and(Mask,cannyFrame,maskedImage);

        imshow("masked Image", maskedImage);

        // how can there be pink line on the left side when that side is not int he mask?
        HoughLines(maskedImage,lines,rhoRes,thetaRes,Threshold,0,0);

        for(int i = 0; i <= lines.size(); i++){

            // takes the magnitude of the angle left and right of the y axis and checks if the line has less than a 30 degree angle from the y
            if(abs(lines[i][1] *180/M_PI) -90 <= 30 || abs(lines[i][1] *180/M_PI)-270 <= 30){
                //cout<<lines[i][1]<<endl;
                // draws the lines
                lineRT(Frame,lines[i],Scalar(255,0,255),2);
            }

        }

        //display frame
        imshow("Video", Frame);
        waitKey(10);
    }
}



