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
DigitDisplay::DigitDisplay(){};

Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/** @function thresh_callback */
void thresh_callback(int, void *) {
    Mat threshold_output;
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;

    /// Detect edges using Threshold
    threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
    imshow("Threshhold Output", threshold_output);
    /// Find contours
    findContours(threshold_output, contours, hierarchy, CV_RETR_TREE,
                 CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    /// Approximate contours to polygons + get bounding rects and circles
    std::vector<std::vector<Point>> contours_poly(contours.size());
    std::vector<Rect> boundRect(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
        boundRect[i] = boundingRect(Mat(contours_poly[i]));
    }

    /// Draw polygonal contour + bonding rects + circles
    Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
                              rng.uniform(0, 255));
        drawContours(drawing, contours_poly, i, color, 1, 8,
                     std::vector<Vec4i>(), 0, Point());
        rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8,
                  0);
    }

    /// Show in a window
    imshow("Contours", drawing);
}

void DigitDisplay::analyse(Mat img) {

    /// Convert image to gray and blur it
    cvtColor(img, src_gray, CV_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));

    /// Create Window

    imshow("Source", img);
    Mat elements = getElements(img);
    imshow("getElements", elements);

    createTrackbar(" Threshold:", "Source", &thresh, max_thresh,
                   thresh_callback);
    thresh_callback(0, 0);

    waitKey(0);
}

Mat DigitDisplay::getElements(Mat img) {

    Mat dest, edges, test;
    std::vector<Vec4i> lines;
    GaussianBlur(img, test, Size(9, 9), 4, 4);
    Canny(test, edges, 50, 100, 3, true);
    threshold(edges, edges, 100, 255, THRESH_BINARY);

    /*  cvtColor(edges, dest, CV_GRAY2BGR);
     HoughLinesP(img, lines, 1, CV_PI/180, 50, 20, 5);

     for( size_t i = 0; i < lines.size(); i++ )
     {
     Vec4i l = lines[i];

     line( dest, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 2, 8,
     0);

     }*/

    // find the contours
    std::vector<std::vector<Point>> contours;
    findContours(edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    // you could also reuse img1 here
    Mat mask = Mat::zeros(edges.rows, edges.cols, CV_8UC1);

    // CV_FILLED fills the connected components found
    drawContours(mask, contours, -1, Scalar(255), CV_FILLED);

    return mask;
};
