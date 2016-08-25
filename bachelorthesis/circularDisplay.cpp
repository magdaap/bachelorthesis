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


Mat CircularDisplay::getLine(Mat img, double rho, double theta){
    Mat dest,edges;
    std::vector<Vec4i> lines;

    Canny(img,edges,50,200,3, true);
    cvtColor(edges, dest, CV_GRAY2BGR);
    HoughLinesP(edges, lines, 1, CV_PI/180, 50, 50, 10);

    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];

        line( dest, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, 8, 0);

    }
    return dest;

};
