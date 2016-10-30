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
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

const char *main_window = "Image Recognition";

int main(int argc, const char *argv[]) {
    try {

        if (argc < 2) {
            std::cout << "No image data given \n" << std::endl;
            return -1;
        }
        cv::Mat src, roi, bg;

        bool doesPicExist;

        std::vector<CircularDisplay> cds;
        std::vector<DigitDisplay> dds;
        char k;
        const char *srcUrl = argv[1];

        const char *url = argv[2];

        std::shared_ptr<Config> config;
        try {
            config = Utils::readConfig(url);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
        for (auto &scaleVariant : config->getScales()) {
            if (scaleVariant.type() == typeid(Config::CircularScale)) {
                auto scale = boost::get<Config::CircularScale>(scaleVariant);
                CircularDisplay cd = CircularDisplay(
                    scale.radius, cv::Point(scale.middleX, scale.middleY),
                    scale.min, scale.max,
                    Rect(Point(scale.roiLeftX, scale.roiLeftY),
                         Point(scale.roiRightX, scale.roiRightY)), config->is_manual());
                cds.push_back(cd);

            } else if (scaleVariant.type() == typeid(Config::DigitScale)) {
                auto scale = boost::get<Config::DigitScale>(scaleVariant);

                DigitDisplay dd =
                    DigitDisplay(Rect(Point(scale.roiLeftX, scale.roiLeftY),
                                      Point(scale.roiRightX, scale.roiRightY)), scale.max);
                dds.push_back(dd);
                // auto scale = boost::get<Config::DigitScale>(scaleVariant);
            }
        }

        auto mog = createBackgroundSubtractorMOG2();
        if (config->is_video()) {
            VideoCapture vid(srcUrl);
            if (!vid.isOpened()) {
                std::cout << "Cannot open video!\n";
                return -1;
            }
            std::ofstream analogResultFile;
            std::ofstream digitalResultsFile;
            analogResultFile.open(
                "/Users/magdalenaprobstl/Desktop/analog_results.txt");
            digitalResultsFile.open(
                "/Users/magdalenaprobstl/Desktop/digital_results.txt");

            if (config->is_manual()) {
                while (true) {

                    doesPicExist = vid.read(src);
                      imshow(main_window, src);
                  cvtColor(src, src, CV_GRAY2RGB);

                    k = waitKey(0);
                    if (k == 'c') {
                        roi = Utils::selectAreaOfInterest(src);
                        for (int i = 0; i < cds.size(); i++) {
                            mog->apply(roi, bg);
                            //     imshow("frame", bg);
                            cds[i].analyseManual(bg);
                            analogResultFile << cds[i].getAmount() << std::endl;
                        }
                        analogResultFile.close();
                    } else if (k == 'd') {
                        roi = Utils::selectAreaOfInterest(src);
                        for (int i = 0; i < dds.size(); i++) {
                            dds[i].analyse(roi);
                        }
                    } else if (k == 'q') {
                        break;
                    }
                }
            } else {
                while (true) {
                    doesPicExist = vid.read(src);

                    if (doesPicExist) {
                        for (int i = 0; i < cds.size(); i++) {
                            //  imshow("src", src);
                            mog->apply(src, bg);
                            cds[i].analyse(bg);

                            analogResultFile << cds[i].getAmount() << std::endl;
                        }

                        for (int i = 0; i < dds.size(); i++) {
                            dds[i].analyse(src);
                            digitalResultsFile << dds[i].getAmount()
                                               << std::endl;
                        }
                        if (src.empty()) {
                            analogResultFile.close();
                            digitalResultsFile.close();

                            //  break;
                        }
                    }
                }
            }
        } else {
            src = imread(srcUrl);
            roi = Utils::selectAreaOfInterest(src);
            k = waitKey(0);
            if (k == 'c') {
                CircularDisplay cd;
                cd.config(roi);
                cd.analyse(roi);
            } else if (k == 'd') {
                DigitDisplay dd;
                dd.analyse(roi);
            }
        }
        return 0;

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
