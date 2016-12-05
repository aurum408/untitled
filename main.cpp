#include <algorithm>
#include <cv.hpp>
#include <highgui.h>
#include <random>
#include <array>
#include <math.h>


//#include <tclDecls.h>
//#include "play.cpp"

using namespace cv;
using namespace std;


double kor(Mat img1, Mat img2, int dx, int dy){
    double c = 0;
    for(int i = 0; i < img1.rows; i++){
        for(int j = 0; j< img1.cols; j++){
            c += img1.at<uchar>(i,j) * img2.at<uchar>(i - dx, j- dy);
           // std:: cout << c;
        }
    }

    //for()
    return c / (img1.cols * img1.rows);
}


int move(Mat img11, Mat img12){
    //double best = -100;
    double var = 0;
    int bestX = 0;
    int bestY = 0;

    //double t = 0;
    for(int r = - img11.rows; r< img11.rows; r++){
        for(int c = - img11.cols; c < img11.cols; c++){
            double t = kor(img11,img12,r,c);
           // std::cout << t;
            if( t > var){
                 var = t;
                 bestX = r;
                 bestY = c;
            }
        }
    }





    std:: cout << bestX  <<"    "  << bestY;

    return 0;
}

int phasedft(){

    VideoCapture cap(0);
    if(!cap.isOpened()) return -1;
    char c = 0;
    Mat prev, dftp;
    bool bFirst = true;
    do {
        Mat frame, im, dfti;
        cap >> frame;
        resize(frame, im, Size(1280/3, 720/3));
        cvtColor(im, im, COLOR_BGR2GRAY);
        im.convertTo(dfti, CV_32F);
        dft(dfti, dfti, DFT_COMPLEX_OUTPUT);

        if(bFirst) {
            bFirst = false;
            prev = im.clone();
            dftp = dfti.clone();
            continue;
        }
        Mat res, cross;
        Mat planes[] = {Mat(dfti.rows,dfti.cols, CV_32F), Mat(dfti.rows,dfti.cols, CV_32F)};

        Mat planes1[] = {Mat(dftp.rows,dftp.cols, CV_32F), Mat(dftp.rows,dftp.cols, CV_32F)};

        split(dfti, planes);
        split(dftp, planes1);
        Mat mag1;
        Mat mag2;

        magnitude(planes[0], planes[1], mag1);
        magnitude(planes1[0], planes1[1], mag2);

        for (int l = 0; l < dfti.rows * dfti.cols; ++l) {


            if(mag1.data[l] == 0 ){
                planes[0].data[l] = 0;}

            else if(mag2.data[l] == 0 ){
                planes1[0].data[l] = 0;}

            else {

                planes[0].data[l] = planes[0].data[l] / mag1.data[l];
                planes[1].data[l] = planes[1].data[l] / mag1.data[l];

                planes1[0].data[l] = planes1[0].data[l] / mag2.data[l];
                planes1[0].data[l] = planes1[0].data[l] / mag2.data[l];

            }
        }


        merge(planes,  2, dfti);
        merge(planes1, 2, dftp);






        mulSpectrums(dfti, dftp, res, 0, true);
        idft(res, res, DFT_SCALE | DFT_REAL_OUTPUT );
        normalize(res, res, 0, 255, NORM_MINMAX);
        res.convertTo(cross, CV_8U);
        Mat cross1 = Mat(cross.rows, cross.cols, CV_8UC1);
        int bestx = 0, besty = 0, bestc = 0;
        for(int i = 0; i < cross.rows; i++) {
            for(int j = 0; j < cross.cols; j++) {
                int c = cross.at<uchar>(i,j);
                cross1.at<uchar>((i+cross.rows/2)%cross.rows,
                                 (j+cross.cols/2)%cross.cols) =
                        cross.at<uchar>(i,j);
                if(bestc < c) {
                    bestc = c;
                    besty = i < cross.rows/2 ? i : i - cross.rows;
                    bestx = j < cross.cols/2 ? j : j - cross.cols;
                }
            }
        }
        cout << "Shifts: " << bestx << ", " << besty << " c = " << bestc << endl;

        imshow("im1", prev);
        imshow("im2", im);
        imshow("out", cross1);
        c = waitKey(20);
        prev = im.clone();
        dftp = dfti.clone();
    } while(c != 27);

}


int phaseFreqdft(){
    VideoCapture cap(0);
    if(!cap.isOpened()) return -1;
    char c = 0;
    Mat prev, dftp;
    bool bFirst = true;
    do {
        Mat frame, im, dfti;
        cap >> frame;
        resize(frame, im, Size(1280/3, 720/3));
        cvtColor(im, im, COLOR_BGR2GRAY);
        im.convertTo(dfti, CV_32F);
        dft(dfti, dfti, DFT_COMPLEX_OUTPUT);

        if(bFirst) {
            bFirst = false;
            prev = im.clone();
            dftp = dfti.clone();
            continue;
        }
        Mat res, cross;

        mulSpectrums(dfti, dftp, res, 0, true);
        idft(res, res, DFT_SCALE | DFT_REAL_OUTPUT );
        normalize(res, res, 0, 255, NORM_MINMAX);
        res.convertTo(cross, CV_8U);
        Mat cross1 = Mat(cross.rows, cross.cols, CV_8UC1);
        int bestx = 0, besty = 0, bestc = 0;
        for(int i = 0; i < cross.rows; i++) {
            for(int j = 0; j < cross.cols; j++) {
                int c = cross.at<uchar>(i,j);
                cross1.at<uchar>((i+cross.rows/2)%cross.rows,
                                 (j+cross.cols/2)%cross.cols) =
                        cross.at<uchar>(i,j);
                if(bestc < c) {
                    bestc = c;
                    besty = i < cross.rows/2 ? i : i - cross.rows;
                    bestx = j < cross.cols/2 ? j : j - cross.cols;
                }
            }
        }
        cout << "Shifts: " << bestx << ", " << besty << " c = " << bestc << endl;

        imshow("im1", prev);
        imshow("im2", im);
        imshow("out", cross1);
        c = waitKey(20);
        prev = im.clone();
        dftp = dfti.clone();
    } while(c != 27);
}


int main(int argc, const char * argv[])
{
    phasedft();
    phaseFreqdft();


    return 0;
}



//int main(){
//    Mat img1 = imread("/Users/anastasia/Desktop/ycbVZ6bNhmM.jpg", CV_8UC1);
//    resize(img1, img1,  Size(100, 100));
//    Mat img2 = imread("/Users/anastasia/Desktop/ycbVZ6bNhmM.jpg", CV_8UC1);
//   // std :: cout << kor(img1, img2, 60,0);
//   // move(img1, img2);
//
//    //std:: cout << kor(img1,img1,100,200);
//   // move(img1, img1);
//
//    VideoCapture cap(0);
//    if(!cap.isOpened()) return -1;
//    char c = 0;
//    Mat frame, im, prev;
//
//    bool bFirst = true;
//    do{
//        cap >> frame;
//        resize(frame,im,Size(480, 300));
//        cvtColor(im, im, COLOR_BGR2GRAY);
//        //resize(prev,prev,Size(480, 300));
//        //cvtColor(prev, prev, COLOR_BGR2GRAY);
//        //prev = frame.clone();
//
//
//        if(bFirst){
//            bFirst = false;
//            prev = im.clone();
//           // continue;
//
//
//        }
//
//        Mat i, i1;
//        i = im.clone();
//        i1 = prev.clone();
//       // move(i1, i);
//
//        imshow("img1", prev);
//        imshow("img2", im);
//
//        move(i1, i);
//
//
//        c = waitKey(30);
//
//        prev = im.clone();
//
//
//
//    }while ( c != 27);
//
//
//
//    return 0;
//
//
//}








