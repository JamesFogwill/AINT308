#ifndef FUNCTIONS_H
#define FUNCTIONS_H

class calcData{

    protected:
    private:

        int Bf;
        int totalBf;
        int distance;

    public:

        int calcBf(int disparity, int distance);

        int calcDist(int disp, int meanBf);

        int meanBf(int* arr, int size);
};


#endif // FUNCTIONS_H
