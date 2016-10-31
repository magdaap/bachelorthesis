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
        cv::Mat src, bg;

        std::vector<CircularDisplay> cds;
        std::vector<DigitDisplay> dds;
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
                    scale.min, scale.max, config->is_manual());
                cd.set_roi(Rect(Point(scale.roiLeftX, scale.roiLeftY),
                                Point(scale.roiRightX, scale.roiRightY)));
                cds.push_back(cd);

            } else if (scaleVariant.type() == typeid(Config::DigitScale)) {
                auto scale = boost::get<Config::DigitScale>(scaleVariant);

                DigitDisplay dd = DigitDisplay(scale.max);
                dd.set_roi(Rect(Point(scale.roiLeftX, scale.roiLeftY),
                                Point(scale.roiRightX, scale.roiRightY)));
                dds.push_back(dd);
                // auto scale = boost::get<Config::DigitScale>(scaleVariant);
            }
        }

        auto mog = createBackgroundSubtractorMOG2();
        VideoCapture vid(srcUrl);
        if (!vid.isOpened()) {
            if (config->is_video()) {

                std::cout << "Cannot open video!\n";
                return -1;
            }
        }

        std::ofstream analogResultFile;
        std::ofstream digitalResultFile;
        analogResultFile.open(
            "/Users/magdalenaprobstl/Desktop/analog_results.txt");
        digitalResultFile.open(
            "/Users/magdalenaprobstl/Desktop/digital_results.txt");

        while (true) {
            if (config->is_video()) {
                vid.read(src);
            } else {
                src = imread(srcUrl);
                cvtColor(src, src, CV_GRAY2RGB);
            }

            for (int i = 0; i < cds.size(); i++) {
                if (!cds[i].roi_isset()) {
                    cds[i].selectRegionOfInterest(src);
                }
                mog->apply(src(cds[i].regionOfInterestRect()), bg);
                if(config->is_manual()){
                     imshow("frame", src(cds[i].regionOfInterestRect()));
                    waitKey(0);
                    imshow("frame", bg);
                    waitKey(0);
                }
                cds[i].analyse(bg);
                analogResultFile << cds[i].getAmount() << std::endl;
            }

            for (int i = 0; i < dds.size(); i++) {
                if (!dds[i].roi_isset()) {
                    dds[i].selectRegionOfInterest(src);
                }
                dds[i].analyse(src(dds[i].regionOfInterestRect()));
                digitalResultFile << dds[i].getAmount() << std::endl;
            }
            if (!config->is_video()) {
                break;
            }
        }
        analogResultFile.close();
        digitalResultFile.close();
        return 0;

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
