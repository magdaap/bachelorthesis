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
DigitDisplay::DigitDisplay() : shownAmount(){};
DigitDisplay::DigitDisplay(Rect roi, int max) : roi(roi), shownAmount(), max(max){};

const char *tessdata = "/Users/magdalenaprobstl/Documents/Bachelorarbeit/code/"
                       "bachelorthesis/bachelorthesis/resources/";

void DigitDisplay::analyse(Mat img) {
    Mat src, res;
    src = img(roi);

    res = preprocessImage(src);
    getText(res);
    // imshow("getElements", elements);
}

Mat DigitDisplay::preprocessImage(Mat img) {
    Mat dest, src_gray;
    cvtColor(img, src_gray, CV_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));

    threshold(src_gray, dest, 100, 255, THRESH_BINARY);
    bitwise_not(dest, dest);

    Mat element = getStructuringElement(MORPH_RECT, Size(2, 5), Point(-1, -1));
    Mat element1 =
        getStructuringElement(MORPH_ELLIPSE, Size(2, 2), Point(-1, -1));

    /// Apply the erosion operation
    dilate(dest, dest, element);
    erode(dest, dest, element1);
    return dest;
};

void DigitDisplay::getText(Mat img) {

    tesseract::TessBaseAPI tess;
    tess.Init(tessdata, "letsgodigital");
    tess.SetImage((uchar *)img.data, img.size().width, img.size().height,
                  img.channels(), (int)img.step1());
    tess.Recognize(0);
    const char *out = tess.GetUTF8Text();
    shownAmount = atof(out);
    while(shownAmount> max){
        shownAmount = shownAmount/10;
    }
    std::cout << shownAmount << std::endl;
};

double DigitDisplay::getAmount() { return shownAmount; }

bool DigitDisplay::roi_isset() {
    return (roi != Rect(Point(0, 0), Point(0, 0)));
};

void DigitDisplay::set_roi(Rect roi) { this->roi = roi; };
