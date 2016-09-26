//
//  digitDisplay.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 29.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "digitDisplay.hpp"
#include <opencv2/opencv.hpp>


using namespace cv;

Mat DigitDisplay::getLines(Mat img){
    
    Mat dest,edges, test;
    std::vector<Vec4i> lines;
    GaussianBlur( img, test, Size(9,9), 4, 4);
    Canny(test,edges,50,100,3, true);
    
    /*  cvtColor(edges, dest, CV_GRAY2BGR);
     HoughLinesP(img, lines, 1, CV_PI/180, 50, 20, 5);
     
     for( size_t i = 0; i < lines.size(); i++ )
     {
     Vec4i l = lines[i];
     
     line( dest, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 2, 8, 0);
     
     }*/
    
    // find the contours
    std::vector< std::vector<Point> > contours;
    findContours(edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    
    // you could also reuse img1 here
    Mat mask = Mat::zeros(edges.rows, edges.cols, CV_8UC1);
    
    // CV_FILLED fills the connected components found
    drawContours(mask, contours, -1, Scalar(255), CV_FILLED);
    
    
    
    return mask;
};
