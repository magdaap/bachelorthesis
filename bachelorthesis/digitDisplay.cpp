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

const char *tessdata = "/Users/magdalenaprobstl/Documents/Bachelorarbeit/code/"
                       "bachelorthesis/bachelorthesis/resources/";

thread_local std::unique_ptr<tesseract::TessBaseAPI> tess;

DigitDisplay::DigitDisplay() : shownAmount(){};

DigitDisplay::DigitDisplay(int max) : max(max), shownAmount() {
    if (!tess) {
        tess.reset(new tesseract::TessBaseAPI);
        tess->Init(tessdata, "letsgodigital");
    }
};

void DigitDisplay::analyse(Mat img) {
    Mat res;

    res = preprocessImage(img);
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

    tess->SetImage((uchar *)img.data, img.size().width, img.size().height,
                   img.channels(), (int)img.step1());
    tess->Recognize(0);
    const char *out = tess->GetUTF8Text();
    shownAmount = atof(out);
    while (shownAmount > max) {
        shownAmount = shownAmount / 10;
    }
    std::cout << shownAmount << std::endl;
};

double DigitDisplay::getAmount() { return shownAmount; }

bool DigitDisplay::roi_isset() {
    return (regionOfInterestRect() != Rect(Point(0, 0), Point(0, 0)));
};

void DigitDisplay::selectRegionOfInterest(const Mat &img) { selectROI(img); };
