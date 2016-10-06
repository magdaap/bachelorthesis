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

int main(int argc, const char *argv[]) {
    try {

        if (argc < 2) {
            std::cout << "No image data given \n" << std::endl;
            return -1;
        }
        cv::Mat image1, image2;
        image1 = imread(argv[1], CV_LOAD_IMAGE_COLOR);
        image2 = imread(argv[2], CV_LOAD_IMAGE_COLOR);

        DigitDisplay dd;
        dd.analyse(image2);

        if (!image1.data) {
            std::cout << "No image data1 \n" << std::endl;
        }
        if (!image2.data) {
            std::cout << "No image data2 \n" << std::endl;
        }

        CircularDisplay cd;
        cd.analyse(image1);

        return 0;

    } catch (Exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
