//James Rogers Mar 2022 (c) Plymouth University
#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>
#include<Detection.h>

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
    VideoCapture myCarVideo("C:/GitHub/AINT308/AINT 308/Task5/Task5/MyDashCam.mp4");

    if(!CarVideo.isOpened()){
        cout<<"Error opening video"<<endl;
        return -1;
    }

    Mat Frame;
    //create detection object from the detection class I made
    Detection myDetection;
    // thresholds and resolutions for finding the hough lines
    float rhoRes = 1;
    float thetaRes = M_PI/180;
    int Threshold = 100;
    // creates the vectors to store the lines
    vector<Vec2f>lines;
    Vec2f averageLines[2];
    // this thresholds the lines so houghlines function only looks for lines within these angles.
    // the angle is relative to the x axis with 0 degrees beign verticle and 90 being horizontal.
    // thresholding so only lines that point in the direction of the road are drawn
    float minLineCheck = 35*(M_PI/180);
    float maxLineCheck = 145*(M_PI/180);

    // creates polygon where we will be detecting the lines
    //top left, top right, bottom right,bottom left
    //V1 is for the supplied video, V2 is for my video
    vector<Point>pointsV1 = {Point(589,404),Point(747,404),Point(1070,710),Point(200,710)};
    vector<Point>pointsV2 = {Point(570,370),Point(696,370), Point(1220,715),Point(174,715)};
    // this is a vector of polygons, only necessary for me because the fillPoly function takes this type as input
    vector<vector<Point>>vectorPolygons = {pointsV2};

    //main program loop
    while(true){

        //open the next frame from the video file, or exit the loop if its the end
        //CarVideo.read(Frame);
        myCarVideo.read(Frame);

        // if the video has ended leave the loop
        if(Frame.empty()){
            break;
        }

        // makes the frame smaller for increased fps
        resize(Frame,Frame,Size(1280,720));

        float totalRhoNE = 0;
        float totalRhoSE = 0;
        float totalThetaNE = 0;
        float totalThetaSE = 0;
        int counterNE = 0;
        int counterSE = 0;

        //==========================Your code goes here==========================
        // erode removes small canny edges which are noise, dilate increases the size of edges to fill in gaps

        // takes your frame, applies bluring and canny
        Mat cannyImage = myDetection.generateCanny(Frame);

        // takes the frame you want the mask on and the mask size, and applies it
        Mat maskedImage = myDetection.generateMask(cannyImage,vectorPolygons);

        // finds the lines that are aabove the threshold and within the min and max angle.
        HoughLines(maskedImage,lines,rhoRes,thetaRes,Threshold, 0, 0, minLineCheck, maxLineCheck);

        //cycles through the lines and takes the average rhos for each lane.
        for(int i = 0; i < lines.size(); i++){
            //takes total rho and theta of all NE pointing lines
            if(lines[i][1] < 60*(M_PI/180)){
                totalRhoNE += lines[i][0];
                totalThetaNE += lines[i][1];
                counterNE++;
            }
            // takes total rho and theta of all SE pointing lines
            else if(lines[i][1] > 120*(M_PI/180)){
                totalRhoSE += lines[i][0];
                totalThetaSE += lines[i][1];
                counterSE++;
            }
            else{
                cout<<"no lines to print"<<endl;
            }

        }
        // stops a division of 0 from happening which would crash the program
        if(counterNE != 0 && counterSE != 0){
            // calculates the mean rho and theta for both directions and stores them in average lines Vec2f
            averageLines[0][0] = totalRhoNE/counterNE;
            averageLines[0][1] = totalThetaNE/counterNE;
            averageLines[1][0] = totalRhoSE/counterSE;
            averageLines[1][1] = totalThetaSE/counterSE;
        }
        // draws the two average lines
        for(int i = 0; i <= 2; i ++){

            lineRT(Frame,averageLines[i],Scalar(0,0,255),2);

        }


        //display frame
        imshow("Video", Frame);
        waitKey(10);
    }
}

// notes
// take the average of the hough lines pointing NE and the average rho of them pointing NW then just print these lines.
// print the average rho between these two lines to get the midpoint of the road.

/*
if(lines[i][1] < 60){
    lineRT(Frame,lines[i],Scalar(255,0,255),2);
}
else if(lines[i][1] > 120){
    lineRT(Frame,lines[i],Scalar(255,0,255),2);
}
fillPoly(Frame,lines,Scalar(255,0,255));
*/

