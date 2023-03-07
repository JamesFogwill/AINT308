//James Rogers Jan 2022 (c) Plymouth University
#include<iostream>
#include <fstream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    int circleRadius = 20;
    int n = 1;
    VideoCapture InputStream;
    Point Pivot;
    Vec3b LowerBound;
    Vec3b UpperBound;

    if (n == 0){

        InputStream = VideoCapture("C:/AINT308Lib/Data/Task2 Video.mp4"); //Load in the video as an input stream
        Pivot = Point(592,52);
        LowerBound = Vec3b(100,100,100);
        UpperBound = Vec3b(140, 255,255);//Pivot position in the video
    }
    else
    {

        InputStream = VideoCapture("C:/GitHub/AINT308/AINT 308/Task2/MyData/RedPendulum.mp4");
        Pivot = Point(547,100);
        LowerBound = Vec3b(0,100,100);
        UpperBound = Vec3b(20,255,255);
    }

    //Open output file for angle data
    ofstream DataFile;
    DataFile.open ("../Task2/Data.csv");

    //loop through video frames
    while(true){

        //load next frame
        Mat Frame;
        InputStream.read(Frame);


        //if frame is empty then the video has ended, so break the loop
        if(Frame.empty()){
            break;
        }

        //video is very high resolution, reduce it to 720p to run faster
        resize(Frame,Frame,Size(1280,720));

        //======================================================Your code goes here====================================================
        //this code will run for each frame of the video. your task is to find the location of the swinging green target, and to find
        //its angle to the pivot. These angles will be saved to a .csv file where they can be plotted in Excel.

        Mat FrameHSV;
        cvtColor(Frame,  FrameHSV, COLOR_BGR2HSV_FULL);

        Mat FrameFiltered;

        //Vec3b LowerBound(HueLower, SatLower, ValLower);
        //Vec3b UpperBound(HueUpper, SatUpper, ValUpper);

        inRange(FrameHSV,LowerBound,UpperBound,FrameFiltered);

        imshow("HSV image", FrameFiltered);

        Moments m = moments(FrameFiltered, true);

        if(m.m00 > 0)
        {
            Point p(m.m10/m.m00, m.m01/m.m00);

            circle(Frame,p ,circleRadius, Scalar(255,100,100),2);

            Point lineTopX(p.x + 25,p.y);
            Point lineBotX(p.x -25,p.y);
            Point lineTopY(p.x,p.y + 25);
            Point lineBotY(p.x,p.y - 25);
            line(Frame,lineTopX, lineBotX, Scalar(255,100,100),2);// line a
            line(Frame,lineTopY, lineBotY, Scalar(255,100,100),2);// line b
            line(Frame, p, Pivot, Scalar(255,100,100), 2);// line c

            float lengthAdj = p.y - Pivot.y;
            float lengthOpp = p.x - Pivot.x;
            float angleOfPendulum = atan2(lengthOpp, lengthAdj);

            cout<<angleOfPendulum<<endl;

            DataFile<<angleOfPendulum<<endl;

        }

        //==============================================================================================================================

        //display the frame
        imshow("Video", Frame);
        waitKey(10);
    }

    DataFile.close(); //close output file
}
