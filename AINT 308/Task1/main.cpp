//James Rogers Jan 2022 (c) Plymouth University
#include <iostream>

#include<opencv2/opencv.hpp>
#include<opencv2/opencv_modules.hpp>

using namespace std;
using namespace cv;

int highestValue(Vec3b pixelValue)
{

    if(pixelValue[0]> pixelValue[1] + pixelValue[2])
    {
        return 1; // blue
    }
    else if(pixelValue[1]> pixelValue[0] + pixelValue[2])
    {
        return 2; // green
    }
    else if(pixelValue[2]> pixelValue[1] + pixelValue[0])
    {
        return 3; //red
    }
    else
    {
        return 0; // nothing
    }
}


int main(){

    int imgWidth = 640;
    int imgHight = 480;
    int red = 0;
    int blue = 0;
    int green = 0;
    int hue = 0;

    //Path of image folder
    string PathToFolder = "../Task1/Car Images/";

    //Loop through the 30 car images
    for(int n=0; n<30; ++n){

        //Each image is named 0.png, 1.png, 2.png, etc. So generate the image file path based on n and the folder path
        string PathToImage = PathToFolder+to_string(n)+".png";

        cout<<PathToImage<<endl;

        //Load car image at the file paths location
        Mat Car=imread(PathToImage);

        //Your code goes here. The example code below shows you how to read the red, green, and blue colour values of the
        //pixel at position (0,0). Modify this section to check not just one pixel, but all of them in the 640x480 image
        //(using for-loops), and using the RGB values classifiy if a given pixel looks red, green, blue, or other.

        for (int pixelWidth = 0; pixelWidth <= imgWidth; pixelWidth++){

            for (int pixelHight = 0; pixelHight < imgHight; ++pixelHight) {

                Vec3b pixelValue = Car.at<Vec3b>(pixelHight,pixelWidth);

                if(highestValue(pixelValue)== 1)
                {
                    blue++;
                }
                else if(highestValue(pixelValue) == 2)
                {
                    green++;
                }
                else if(highestValue(pixelValue) == 3)
                {
                    red++;
                }
                else {

                    hue++;
                }

            }
        }
        cout<<"The total blue pixels in the image are "<<blue<<endl;
        cout<<"The total green pixels in the image are "<<green<<endl;
        cout<<"The total red pixels in the image are "<<red<<endl;
        cout<<"The total hued pixels that were too hard to depict are "<<hue<<endl;

        if(red > green && red > blue){
            cout<<"This car is Red"<<endl;
        }
        else if(green > blue && green > red){
            cout<<"This car is Green"<<endl;
        }
        else if(blue > red && blue > green)
        {
            cout<<"This car is Blue"<<endl;
        }

        red = 0;
        blue = 0;
        green = 0;
        hue = 0;

        // have variables that tell me if it classifies into any colour?

        //==============example code, feel free to delete=============
        //int x=0;
        //int y=0;
        //Vec3b PixelValue = Car.at<Vec3b>(y,x);

        // so PixelValue is an array that contains the r, g and b values for a point.
        //cout<<"The blue value at (0,0) is " <<(int)PixelValue[0]<<endl;
        //cout<<"The green value at (0,0) is "<<(int)PixelValue[1]<<endl;
        //cout<<"The red value at (0,0) is "  <<(int)PixelValue[2]<<endl;
        //============================================================

        //display the car image untill x is pressed
        while(waitKey(10)!='x'){
            imshow("Car", Car);
        }

    }

}



















