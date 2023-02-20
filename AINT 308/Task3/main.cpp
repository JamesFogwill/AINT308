//James Rogers Jan 2021 (c) Plymouth University
#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{

    //Path to image file
    string Path = "../Task3/PCB Images/";
    double maximumError = 0.009;

    //loop through component images
    for(int n=0; n<10; ++n){

        //read PCB and component images
        Mat PCB = imread(Path+"PCB.png");
        Mat Component = imread(Path+"Component"+to_string(n)+".png");
        Mat greyScaleOutputImage; // mat for thegreyscale output that shows where most similar match is using match template

        //================Your code goes here=====================


        matchTemplate(PCB, Component,greyScaleOutputImage, TM_SQDIFF_NORMED); //  match component with PCB and create greyscale image of highest similarity. Darkest spots have highest similarity.

        imshow("OutputImg", greyScaleOutputImage); //  output the greyscale image

        double max_Val, min_Val; // create variable to contain the min and maximum error values of the most similar pixel.
        Point min_Loc, max_Loc; // create points for the position of min and maximum error values of most similar and disimilar pixels.

        minMaxLoc(greyScaleOutputImage, &min_Val, &max_Val, &min_Loc, &max_Loc); //  locate the most and least similar pixels, assign the error values.

        if (min_Val < maximumError) // Checking to see if the component is there.
        {
            rectangle(PCB, min_Loc, Point(min_Loc.x + Component.cols, min_Loc.y + Component.rows), Scalar(0,0,255),2); //  draws rectangle from point of most imilarity and adds the width and hight of component image, then makes rectangle red with line thickness 2.
            //imshow("Best Match", PCB); //  displays the PCB with rec
        }
        else { //  if component is not visable
            cout<<min_Val<<endl; //  outputs the error value
            cout<<"maxmimum error value was reached, component likely missing at position: "<< min_Loc <<endl; //  outputs the position of missing component
        }

        //cout<<min_Val<<endl;




        //display the results untill x is pressed
        while(waitKey(10)!='x'){
            imshow("Target", Component);
            imshow("PCB", PCB);
        }

    }

}
