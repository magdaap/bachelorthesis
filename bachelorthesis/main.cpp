//
//  main.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 05.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "Utils.hpp"
#include "circularDisplay.hpp"
#include "digitDisplay.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

const char *src_window = "Select ROI";
const char *main_window = "Image Recognition";

Point point;
void setPoint(int event, int x, int y, int flags, void *userdata) {

    if (event == (EVENT_LBUTTONDOWN)) {
        point.x = x;
        point.y = y;
    }
};

Mat selectAreaOfInterest(Mat img) {
    Mat aoi;
    Point leftup, rightdown;
    char l;
    img.copyTo(aoi);
    while (true) {
        imshow(src_window, img);
        setMouseCallback(src_window, setPoint, NULL);
        l = waitKey(0);
        if (l == '1') {
            leftup = point;
            std::cout << "leftup: " << leftup << std::endl;
        } else if (l == '2') {
            rightdown = point;
            std::cout << "rightdown: " << rightdown << std::endl;

            rectangle(img, leftup, rightdown, Scalar(255));
        } else if (l == 'q') {
            aoi = aoi(Rect(leftup, rightdown));
            destroyWindow(src_window);

            break;
        }
    }

    return aoi;
};

int main(int argc, const char *argv[]) {
    try {

        if (argc < 2) {
            std::cout << "No image data given \n" << std::endl;
            return -1;
        }
        cv::Mat image1, image2, src, roi;
        std::vector<CircularDisplay> cds;
        DigitDisplay dd;

        char k;
        const char *url = argv[3];
        auto config = Utils::readConfig(url);
        for (auto &scaleVariant : config->getScales()) {
            if (scaleVariant.type() == typeid(Config::CircularScale)) {
                auto scale = boost::get<Config::CircularScale>(scaleVariant);
                CircularDisplay cd = CircularDisplay(
                    scale.radius, cv::Point(scale.middleX, scale.middleY),
                    scale.min, scale.max, Rect(Point(scale.roiLeftX, scale.roiLeftY),Point(scale.roiRightX, scale.roiRightY)));
                cds.push_back(cd);

            } else if (scaleVariant.type() == typeid(Config::DigitScale)) {
                // auto scale = boost::get<Config::DigitScale>(scaleVariant);
            }
        }

        VideoCapture vid(
            "/Users/magdalenaprobstl/Desktop/test_video1_kurz.mp4");
        if (!vid.isOpened()) {
            std::cout << "Cannot open video!\n";
            return -1;
        }
        image1 = imread(argv[1], CV_LOAD_IMAGE_COLOR);
        image2 = imread(argv[2], CV_LOAD_IMAGE_COLOR);

        if (config->is_manual() == true) {
            while (true) {
                vid.read(src);
                imshow(main_window, src);
                k = waitKey(0);
                if (k == 'c') {
                    roi = src(Rect(Point(668, 101), Point(1115, 322)));
                    for (int i = 0; i < cds.size(); i++){
                        cds[i].analyse(src);
                    }
                } else if (k == 'd') {
                    roi = selectAreaOfInterest(src);
                    dd.analyse(roi);
                }

                else if (k == 'q') {
                    break;
                }
            }
        } else {
            while (true) {
                vid.read(src);
                for (int i = 0; i < cds.size(); i++){
                    cds[i].analyse(src);
                }
            }
        }

        //  dd.analyse(image2);

        if (!image1.data) {
            std::cout << "No image data1 \n" << std::endl;
        }
        if (!image2.data) {
            std::cout << "No image data2 \n" << std::endl;
        }

        //  cd.analyse(image1);

        return 0;

    } catch (Exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
