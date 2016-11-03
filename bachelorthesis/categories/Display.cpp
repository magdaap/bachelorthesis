//
//  Display.cpp
//  bachelorthesis
//

#include "Display.hpp"

#include <iostream>

const char *src_window = "Select ROI";

// Improve thread safety
thread_local cv::Point point;

void setPoint(int event, int x, int y, int flags, void *userdata) {
    if (event == (cv::EVENT_LBUTTONDOWN)) {
        point.x = x;
        point.y = y;
    }
}

void Display::selectROI(cv::Mat img) {
    cv::Mat roi_img;
    cv::Point leftup, rightdown;
    char l;
    img.copyTo(roi_img);
    while (true) {
        cv::imshow(src_window, roi_img);
        cv::setMouseCallback(src_window, setPoint, NULL);
        l = cv::waitKey(0);
        if (l == '1') {
            leftup = point;
            std::cout << "leftup: " << leftup << std::endl;
        } else if (l == '2') {
            rightdown = point;
            std::cout << "rightddown: " << rightdown << std::endl;

            rectangle(roi_img, leftup, rightdown, cv::Scalar(0));
        } else if (l == 'q') {
            cv::destroyWindow(src_window);
            roi = cv::Rect(leftup, rightdown);
            roiImg = img(roi);
            imshow("finish", roiImg);
            cv::waitKey(0);
            cv::destroyWindow("finish");

            break;
        }
    }
};

cv::Rect Display::regionOfInterestRect() { return roi; }

cv::Mat Display::regionOfInterest() { return roiImg; }

void Display::setROI(cv::Rect new_roi) { roi = new_roi; };
