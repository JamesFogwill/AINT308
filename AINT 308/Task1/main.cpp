//James Rogers Jan 2022 (c) Plymouth University
#include <iostream>

#include<opencv2/opencv.hpp>
#include<opencv2/opencv_modules.hpp>

using namespace std;
using namespace cv;

//Vec3b is a 3 colour channel class, one channel is red green blue respectively
int highestValue(Vec3b pixelValue) // Function returns a number to signify either red green or blue pixel dominance
{

    if(pixelValue[0]> pixelValue[1] + pixelValue[2]) //  if the value of blue in the pixel > value of all green and red in the Vec3b pixelValue
    {                                                // The purpose of plusing the two values is to make sure the pixel is dominant in one colour
        return 1; // return a 1 from this function which will signify blue
    }
    else if(pixelValue[1]> pixelValue[0] + pixelValue[2]) //  if the value of green in the pixel > value of all blue and red in the Vec3b pixelValue
    {
        return 2; // return a 2 from this function which will signify green
    }
    else if(pixelValue[2]> pixelValue[1] + pixelValue[0]) //  if the value of red in the pixel > value of all green and blue in the Vec3b pixelValue
    {
        return 3; // return a 3 from this function which will signify red
    }
    else
    {
        return 0; // nothing aka there is  no dominant colour. Example, this stops a slightly blue hued pixel counting as a full blue
    }
}


int main(){

    int imgWidth;
    int imgHight;
    int red = 0;  // set initial colour values to 0
    int blue = 0;
    int green = 0;
    int hue = 0;

    //Path of image folder
    string PathToFolder = "../Task1/Car Images/";

    //Loop through the 30 car images
    for(int n=0; n<37; ++n){

        //Each image is named 0.png, 1.png, 2.png, etc. So generate the image file path based on n and the folder path
        string PathToImage = PathToFolder+to_string(n)+".png";

        cout<<PathToImage<<endl;

        //Load car image at the file paths location
        Mat Car=imread(PathToImage);
        cout<<PathToImage<<endl;

        imgWidth = Car.cols;
        imgHight = Car.rows;
        cout<< "Width of this image is: "<< imgWidth <<endl;
        cout<< "Hight of this image is: "<< imgHight <<endl;

        //Your code goes here. The example code below shows you how to read the red, green, and blue colour values of the
        //pixel at position (0,0). Modify this section to check not just one pixel, but all of them in the 640x480 image
        //(using for-loops), and using the RGB values classifiy if a given pixel looks red, green, blue, or other.

        for (int pixelWidth = 0; pixelWidth < imgWidth; pixelWidth++){ //  loop that runs 640 times, for every column of the car image

            for (int pixelHight = 0; pixelHight < imgHight; ++pixelHight) { // loop that runs 480 time, for every row of the car image

                Vec3b pixelValue = Car.at<Vec3b>(pixelHight,pixelWidth); //  gets the Vec3b pixel value on the current car image at specified pixel hight and width

                if(highestValue(pixelValue)== 1) //  run my highest value function that takes the Vec3b value and returns the dominant colour
                {
                    blue++; // A 1 equates to Blue so add one blue pixel to the total blue pixels of the image
                }
                else if(highestValue(pixelValue) == 2)
                {
                    green++; // A 2 equates to green so add one green pixel to the total green pixels of the image
                }
                else if(highestValue(pixelValue) == 3)
                {
                    red++; // A 3 equates to red so add one red pixel to the total red pixels of the image
                }
                else {

                    hue++; //  This is to keep track of how many pixels have no dominant colour.
                }

            }
        }
        cout<<"The total blue pixels in the image are "<<blue<<endl;               // Here I am printing to the terminal the total number of coloured and hued pixels in the current car image
        cout<<"The total green pixels in the image are "<<green<<endl;
        cout<<"The total red pixels in the image are "<<red<<endl;
        cout<<"The total hued pixels that were too hard to depict are "<<hue<<endl;

        if(red > green && red > blue){ // if the image has more red pixels than green and blue pixels print the car is red
            cout<<"This car is Red"<<endl;
        }
        else if(green > blue && green > red){ //  same as above but for green
            cout<<"This car is Green"<<endl;
        }
        else if(blue > red && blue > green) // same as above but for blue
        {
            cout<<"This car is Blue"<<endl;
        }

        red = 0;   // resets my values for the next image to be loaded.
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
