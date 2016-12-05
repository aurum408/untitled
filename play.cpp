#include <iostream>
#include <cv.hpp>
#include <highgui.h>


using namespace cv;
void mix(){
    //std::cout << "Hello, World!" << std::endl;

    Mat img1 = imread("/Users//anastasia//Downloads//sun.jpg");
    Mat img2 = imread("/Users/anastasia/Downloads/spb.jpg");

    int cols = img1.cols;
    int raws = img1.rows;

    Mat spb(cols, raws, CV_8UC3);
    resize(img2, spb, Size(cols, raws));

    Mat mixed(raws,cols, CV_8UC3);

    for(int i = 0; i< raws; i++){
        uchar *u1 = spb.ptr<uchar>(i);
        uchar *u2 = img1.ptr<uchar>(i);
        uchar *u3 = mixed.ptr<uchar>(i);

        for(int j = 0; j< cols; j++){
            u3[j*3] = (u1[j*3] + u2[j*3])/2;
            u3[j*3 +1] = (u1[j*3 +1] + u2[j*3 +1])/2;
            u3[j*3 +2] = (u1[j*3 +2] + u2[j*3] +2)/2;
        }

    }

     imshow("img11", mixed);
     waitKey();


}

void calc_hist(Mat imr, int*hist ){
    memset(hist, 0, 256 * sizeof(int));
    for(int i = 0; i< 256;i++){

    }

}


void mix1(){

    Mat img1 = imread("/Users//anastasia//Downloads//sun.jpg", CV_8UC1);
    Mat img2 = imread("/Users/anastasia/Downloads/spb.jpg", CV_8UC1);

    int cols = img1.cols;
    int raws = img1.rows;

    Mat spb(cols, raws, CV_8UC1);
    resize(img2, spb, Size(cols, raws));

    Mat mixed(raws,cols, CV_8UC1);

    for(int i = 0; i< raws; i++){
        uchar *u1 = spb.ptr<uchar>(i);
        uchar *u2 = img1.ptr<uchar>(i);
        uchar *u3 = mixed.ptr<uchar>(i);

        for(int j = 0; j< cols; j++){
            u3[j] = (u1[j] + u2[j])/2;

        }

    }

    imshow("img11", mixed);
    waitKey();
}




