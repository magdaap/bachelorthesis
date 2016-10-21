//
//  digitDisplay.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 29.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "digitDisplay.hpp"
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

using namespace cv;
DigitDisplay::DigitDisplay(){};
DigitDisplay::DigitDisplay(Rect roi) : roi(roi){};

Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

void DigitDisplay::analyse(Mat img) {
    Mat src;
    img = img + Scalar(-10, -10, -10);
    src = img(roi);

    /// Convert image to gray and blur it
    cvtColor(src, src_gray, CV_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));

    /// Create Window

    // imshow("Source", src);
    Mat elements = getElements(src_gray);
    // imshow("getElements", elements);

    //    waitKey(0);
}

Mat DigitDisplay::getElements(Mat img) {

    Mat dest, edges, test;
    std::vector<Vec4i> lines;

    // GaussianBlur(img, img, Size(13, 13), 0, 0);

    threshold(img, edges, 100, 255, THRESH_BINARY);
    bitwise_not(edges, edges);

    Mat element = getStructuringElement(MORPH_RECT, Size(2, 5), Point(-1, -1));
    Mat element1 =
        getStructuringElement(MORPH_ELLIPSE, Size(2, 2), Point(-1, -1));

    /// Apply the erosion operation
    dilate(edges, edges, element);
    erode(edges, edges, element1);
    imshow("Erosion Demo", edges);
    // find the contours

    tesseract::TessBaseAPI tess;
    tess.Init("/usr/local/Cellar/tesseract/3.04.01_2/share/tessdata",
              "letsgodigital");
    tess.SetImage((uchar *)edges.data, edges.size().width, edges.size().height,
                  edges.channels(), (int)edges.step1());
    tess.Recognize(0);
    const char *out = tess.GetUTF8Text();
    std::cout << out << std::endl;
    /*   std::vector<std::vector<Point>> contours;
       findContours(edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

       // CV_FILLED fills the connected components found
       drawContours(edges, contours, -1, Scalar(255), CV_FILLED);
     */
    return edges;
};
