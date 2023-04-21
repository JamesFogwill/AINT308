#include <Functions.h>

int calcData::calcBf(int disparity, int distance){
    //calculates constant and a mean for the constant
    Bf = disparity * distance;
    return Bf;
}

int calcData::meanBf(int* array, int size){
    int sum = 0;

    for (int i = 0; i < size; i++) {
            sum += array[i];
        }
    int meanBf = sum/size;
    return meanBf;
}


int calcData::calcDist(int disp, int meanBf){
    if(disp != 0){

        distance = meanBf/disp;
        return distance;
    }
    else
        return 0;
}
