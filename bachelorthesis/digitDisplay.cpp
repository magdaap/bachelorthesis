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

const char *progressDigit = "Progress";

const char *tessdata = "/Users/magdalenaprobstl/Documents/Bachelorarbeit/code/"
                       "bachelorthesis/bachelorthesis/resources/";

thread_local std::unique_ptr<tesseract::TessBaseAPI> tess;

DigitDisplay::DigitDisplay() : shownAmount(){};

DigitDisplay::DigitDisplay(int max, bool manual)
    : max(max), shownAmount(), manual(manual) {
    if (!tess) {
        tess.reset(new tesseract::TessBaseAPI);
        tess->Init(tessdata, "letsgodigital");
    }
};

void DigitDisplay::analyze(Mat img) {
    Mat res;
    res = preprocessImage(img);
    getText(res);
}

Mat DigitDisplay::preprocessImage(Mat img) {
    Mat dest, src_gray;

    cvtColor(img, src_gray, CV_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));
    fastNlMeansDenoising(src_gray, src_gray);

    if (manual) {
        imshow(progressDigit, src_gray);
        waitKey(0);
        destroyWindow(progressDigit);
    }

    threshold(src_gray, dest, 100, 255, THRESH_BINARY);
    bitwise_not(dest, dest);
    return dest;
};

void DigitDisplay::getText(Mat img) {
    tess->SetVariable("tessedit_char_whitelist", "1234567890");

    tess->SetImage((uchar *)img.data, img.size().width, img.size().height,
                   img.channels(), (int)img.step1());
    tess->Recognize(0);
    const char *out = tess->GetUTF8Text();
    std::cout << out << std::endl;

    shownAmount = atof(out);
    while (shownAmount > max) {
        shownAmount = shownAmount / 10;
    }
    std::cout << shownAmount << std::endl;
};

double DigitDisplay::getAmount() { return shownAmount; }

bool DigitDisplay::roiIsset() {
    return (regionOfInterestRect() != Rect(Point(0, 0), Point(0, 0)));
};

void DigitDisplay::selectRegionOfInterest(Mat img) { selectROI(img); };
