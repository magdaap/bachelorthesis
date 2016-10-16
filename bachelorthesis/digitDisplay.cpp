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
DigitDisplay::DigitDisplay(Rect roi) : roi(roi){};

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
        // rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8,
        // 0);
    }

    /// Show in a window
    imshow("Contours", drawing);
}

void DigitDisplay::analyse(Mat img) {
    Mat src;
    src = img(roi);

    /// Convert image to gray and blur it
    cvtColor(src, src_gray, CV_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));

    /// Create Window

    imshow("Source", src);
    Mat elements = getElements(src_gray);
    imshow("getElements", elements);

    createTrackbar(" Threshold:", "Source", &thresh, max_thresh,
                   thresh_callback);
    thresh_callback(0, 0);

    waitKey(0);
}

Mat DigitDisplay::getElements(Mat img) {
    
    Mat dest, edges, test;
        std::vector<Vec4i> lines;
    
    threshold(img, edges, 100, 255, THRESH_BINARY);
    imshow("jkdfm", edges);
    waitKey();
    destroyWindow("jkdfm");
    Mat element = getStructuringElement( MORPH_RECT,
                                        Size(10,10 ),
                                        Point( -1, -1 ) );
    /// Apply the erosion operation
    dilate( edges, edges, element );
    imshow( "Erosion Demo", edges );
    // find the contours
    std::vector<std::vector<Point>> contours;
    findContours(edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    // CV_FILLED fills the connected components found
    drawContours(edges, contours, -1, Scalar(255), CV_FILLED);
    imshow("jkdfm", edges);
    waitKey();
    destroyWindow("jkdfm");
    return edges;
};
