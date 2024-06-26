//James Rogers Nov 2020 (c) Plymouth University
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <Functions.h>

using namespace cv;
using namespace std;

calcData calculate;

int main(int argc, char** argv)
{
    //Calibration file paths (you need to make these)
    string intrinsic_filename = "C:/GitHub/AINT308/AINT 308/StereoCalibration/intrinsics.xml";
    string extrinsic_filename = "C:/GitHub/AINT308/AINT 308/StereoCalibration/extrinsics.xml";

    //================================================Load Calibration Files===============================================
    //This code loads in the intrinsics.xml and extrinsics.xml calibration files, and creates: map11, map12, map21, map22.
    //These four matrices are used to distort the camera images to apply the lense correction.
    Rect roi1, roi2;
    Mat Q;
    Size img_size = {640,480};

    FileStorage fs(intrinsic_filename, FileStorage::READ);
    if(!fs.isOpened()){
        printf("Failed to open file %s\n", intrinsic_filename.c_str());
        return -1;
    }

    Mat M1, D1, M2, D2;
    fs["M1"] >> M1;
    fs["D1"] >> D1;
    fs["M2"] >> M2;
    fs["D2"] >> D2;

    fs.open(extrinsic_filename, FileStorage::READ);
    if(!fs.isOpened())
    {
        printf("Failed to open file %s\n", extrinsic_filename.c_str());
        return -1;
    }
    Mat R, T, R1, P1, R2, P2;
    fs["R"] >> R;
    fs["T"] >> T;

    stereoRectify( M1, D1, M2, D2, img_size, R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1, img_size, &roi1, &roi2 );

    Mat map11, map12, map21, map22;
    initUndistortRectifyMap(M1, D1, R1, P1, img_size, CV_16SC2, map11, map12);
    initUndistortRectifyMap(M2, D2, R2, P2, img_size, CV_16SC2, map21, map22);

    //===============================================Stereo SGBM Settings==================================================
    //This sets up the block matcher, which is used to create the disparity map. The various settings can be changed to
    //obtain different results. Note that some settings will crash the program.

    int SADWindowSize=5;            //must be an odd number >=3
    int numberOfDisparities=256;    //must be divisable by 16

    Ptr<StereoSGBM> sgbm = StereoSGBM::create(0,16,3);
    sgbm->setBlockSize(SADWindowSize);
    sgbm->setPreFilterCap(63);
    sgbm->setP1(8*3*SADWindowSize*SADWindowSize);
    sgbm->setP2(32*3*SADWindowSize*SADWindowSize);
    sgbm->setMinDisparity(0);
    sgbm->setNumDisparities(numberOfDisparities);
    sgbm->setUniquenessRatio(10);
    sgbm->setSpeckleWindowSize(100);
    sgbm->setSpeckleRange(32);
    sgbm->setDisp12MaxDiff(1);
    sgbm->setMode(StereoSGBM::MODE_SGBM);

    //==================================================Main Program Loop================================================

    //Open output file for angle data
    ofstream DataFileKnown;
    ofstream DataFileUnknown;
    DataFileKnown.open ("C:/GitHub/AINT308/AINT 308/Task4/Task4/DataK.csv");
    DataFileUnknown.open ("C:/GitHub/AINT308/AINT 308/Task4/Task4/DataUk.csv");

    // initialisation of matix to store the distance vs disparity
    // init of matrix to store the distances for distance map
    Mat dispVsDist = Mat::zeros(13,2, CV_32S);
    Mat distanceMap = Mat::zeros(480,640, CV_8U);

    // variable to store the mean BF constant and a index counter variable
    int meanBf;
    int idx = 0;
    //current image index for distance loop
    for(int distImgNum = 30; distImgNum <= 150 ; distImgNum+= 10){

        // will store the disparity for the known distance loop
        int disparityValue;
        // stores the bf constants in an array to make a mean constant.
        int constant[13];
        // stores the constant calculated distance
        int estimatedDistance;

        // loads the known distance targets to calculate the constant
        Mat distLeft =imread("C:/GitHub/AINT308/AINT 308/Task4/Task4/Distance Targets/left" +to_string(distImgNum)+"cm.jpg");
        Mat distRight =imread("C:/GitHub/AINT308/AINT 308/Task4/Task4/Distance Targets/right" +to_string(distImgNum)+"cm.jpg");
        cout<<endl<< "Loaded distance image: "<<distImgNum<<"cm"<<endl;

        //Distort image to correct for lens/positional distortion
        remap(distLeft, distLeft, map11, map12, INTER_LINEAR);
        remap(distRight, distRight, map21, map22, INTER_LINEAR);

        //Match left and right images to create disparity image
        Mat disp16bit, disp8bit;
        sgbm->compute(distLeft, distRight, disp16bit);                       //compute 16-bit greyscalse image with the stereo block matcher
        disp16bit.convertTo(disp8bit, CV_8U, 255/(numberOfDisparities*16.)); //Convert disparity map to an 8-bit greyscale image so it can be displayed (Only for imshow, do not use for disparity calculations)

        //takes disparity at pixel coordinates on the target
        disparityValue = disp16bit.at<ushort>(232,393);
        cout<<"The disparity at that distance is: "<<disparityValue<<endl;

        //calculates the constant for that image
        constant[idx] = calculate.calcBf(disparityValue,distImgNum);
        cout<<"The bf constant is: "<<constant[idx]<<endl;

        //calculates the distance using the constant
        estimatedDistance = calculate.calcDist(disparityValue, constant[idx]);
        cout<<"The estimated distance is: "<<estimatedDistance<<"cm"<<endl;

        // These plug the disparity and distance values into a matrix that is stored in the excel file later.
        dispVsDist.at<int>(idx,0) = disparityValue;
        dispVsDist.at<int>(idx,1) = distImgNum;

        // calculates the mean constant when the constant array is full
        if(distImgNum == 150){

            meanBf = calculate.meanBf(constant, 12);
            cout<<"The mean bf constant is: "<<meanBf<<endl<<"We will be using this constant for future calculations."<<endl;
        }
        //display images until x is pressed
        int key=0;
        while(waitKey(10)!='x')
        {
            imshow("left", distLeft);
            imshow("right", distRight);
            imshow("disparity", disp8bit);
        }

        idx++;
    }

    //offload the disp value to the csv file
    DataFileKnown<<dispVsDist<<endl;
    //close known data file
    DataFileKnown.close();

    while (1){

        // variables for the resolution of the images
        int imgWidth;
        int imgHeight;

        //loops through all the unknown images
        for(int ImageNum = 0; ImageNum <= 7; ImageNum++){

            int disparityValue;
            int estimatedDistance;

            //Load images from file (needs changing for known distance targets)
            Mat Left =imread("../Task4/Unknown Targets/left" +to_string(ImageNum)+".jpg");
            Mat Right=imread("../Task4/Unknown Targets/right"+to_string(ImageNum)+".jpg");
            cout<<"Loaded image: "<<ImageNum<<endl;

            //gets the resolution of the image in use
            imgWidth = Left.cols;
            imgHeight = Left.rows;

            //Distort image to correct for lens/positional distortion
            remap(Left, Left, map11, map12, INTER_LINEAR);
            remap(Right, Right, map21, map22, INTER_LINEAR);

            //Match left and right images to create disparity image
            Mat disp16bit, disp8bit;
            sgbm->compute(Left, Right, disp16bit);                               //compute 16-bit greyscalse image with the stereo block matcher
            disp16bit.convertTo(disp8bit, CV_8U, 255/(numberOfDisparities*16.)); //Convert disparity map to an 8-bit greyscale image so it can be displayed (Only for imshow, do not use for disparity calculations)

            // loop to create distance map, cycles through every pixel

            for(int w = 0; w < imgWidth; w++){
                for(int h = 0; h < imgHeight; h++){

                    // finds disparity and distance at that pixel
                    disparityValue = disp16bit.at<ushort>(h,w);

                    // finds the estimated distance using disparity and constant
                    estimatedDistance = calculate.calcDist(disparityValue,meanBf);

                    if(estimatedDistance <= 255){
                        // applies the distance to the current pixel value
                        distanceMap.at<uchar>(h,w) = estimatedDistance;
                    }
                    // if out of the 255cm range just put 255cm
                    else{
                        distanceMap.at<uchar>(h,w) = 255;
                    }
                }
            }

            int caseDistMap = distanceMap.at<uchar>(249,357);
            cout<<"The distance to the case is: "<<caseDistMap<<"cm"<<endl;

            //display images until x is pressed
            int key=0;
            while(waitKey(10)!='x')
            {
                imshow("left", Left);
                imshow("right", Right);
                imshow("disparity", disp8bit);
                imshow("Distance Map", distanceMap);
            }
        }
        break;
    }

    return 0;
}

