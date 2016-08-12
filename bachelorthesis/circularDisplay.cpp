//
//  circularDisplay.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 05.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "circularDisplay.hpp"
#include <opencv2/opencv.hpp>


using namespace cv;

void CircularDisplay::showImages(Mat src, Mat evol){
    
    namedWindow("Src", WINDOW_AUTOSIZE );
    imshow("Src", src);
    namedWindow("Evolved", WINDOW_AUTOSIZE );
    imshow("Evolved", evol);
    waitKey(0);
};

Mat CircularDisplay::getLine(Mat img){
    Mat after;
 //   HoughLines(img, after, 30, 80, 1);
    HoughLinesP(img, after, 10, 50, 1);
    return after;

};
