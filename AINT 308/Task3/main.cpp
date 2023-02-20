//James Rogers Jan 2021 (c) Plymouth University
#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{

    //Path to image file
    string Path = "../Task3/PCB Images/";

    //loop through component images
    for(int n=0; n<10; ++n){

        //read PCB and component images
        Mat PCB = imread(Path+"PCB.png");
        Mat Component = imread(Path+"Component"+to_string(n)+".png");
        Mat OutputImage; // is this meant to be a 1x1 matrix for the height and width of the area the template is being found

        //================Your code goes here=====================


        matchTemplate(PCB, Component,OutputImage, TM_SQDIFF_NORMED);

        imshow("OutputImg", OutputImage);
        //cout<<OutputImage<<endl;
        double max_Val, min_Val;
        Point min_Loc, max_Loc;

        minMaxLoc(OutputImage, &min_Val, &max_Val, &min_Loc, &max_Loc);



















        //display the results untill x is pressed
        while(waitKey(10)!='x'){
            imshow("Target", Component);
            imshow("PCB", PCB);
        }

    }

}
