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

void Display::selectROI(const cv::Mat &img) {
    cv::Mat roi_img;
    cv::Point leftup, rightdown;
    char l;
    img.copyTo(roi_img);
    while (true) {
        cv::imshow(src_window, img);
        cv::setMouseCallback(src_window, setPoint, NULL);
        l = cv::waitKey(0);
        if (l == '1') {
            leftup = point;
            std::cout << "leftup: " << leftup << std::endl;
        } else if (l == '2') {
            rightdown = point;
            std::cout << "rightdown: " << rightdown << std::endl;

            rectangle(img, leftup, rightdown, cv::Scalar(255));
        } else if (l == 'q') {
            cv::destroyWindow(src_window);
            roi = cv::Rect(leftup, rightdown);
            roi_img = roi_img(roi);

            break;
        }
    }
};

const cv::Rect &Display::regionOfInterestRect() { return roi; }

const cv::Mat &Display::regionOfInterest() { return roi_img; }

void Display::set_roi(cv::Rect new_roi) { roi = new_roi; };
