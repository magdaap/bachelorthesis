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

#include "bgsegm.hpp"
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

        std::vector<CircularDisplay> cds;
        std::vector<DigitDisplay> dds;
        const char *srcUrl = argv[1];

        const char *url = argv[2];

        std::shared_ptr<Config> config;
        try {
            config = readConfig(url);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
        for (auto &scaleVariant : config->getScales()) {
            if (scaleVariant.type() == typeid(Config::CircularScale)) {
                auto scale = boost::get<Config::CircularScale>(scaleVariant);
                CircularDisplay cd = CircularDisplay(
                    scale.radius, cv::Point(scale.middleX, scale.middleY),
                    scale.min, scale.max, scale.base, config->is_manual(),
                    scale.calculationType);
                cd.setROI(Rect(Point(scale.roiLeftX, scale.roiLeftY),
                               Point(scale.roiRightX, scale.roiRightY)));
                cds.push_back(cd);

            } else if (scaleVariant.type() == typeid(Config::DigitScale)) {
                auto scale = boost::get<Config::DigitScale>(scaleVariant);

                DigitDisplay dd = DigitDisplay(scale.max, config->is_manual());
                dd.setROI(Rect(Point(scale.roiLeftX, scale.roiLeftY),
                               Point(scale.roiRightX, scale.roiRightY)));
                dds.push_back(dd);
            }
        }
        Ptr<BackgroundSubtractor> mog;
        mog = bgsegm::createBackgroundSubtractorMOG();
        VideoCapture vid(srcUrl);
        if (config->is_video()) {

            if (!vid.isOpened()) {

                std::cout << "Cannot open video!\n";
                return -1;
            }
        }
        cv::Mat bg;
        cv::Mat src;

        std::ofstream resultFile;
        resultFile.open("resources/results.txt");
        VideoWriter outputVideo("resources/result.avi", CV_FOURCC('m', 'p', '4', 'v'),
                         vid.get(CV_CAP_PROP_FPS),
                         Size(vid.get(CV_CAP_PROP_FRAME_WIDTH),
                              vid.get(CV_CAP_PROP_FRAME_HEIGHT)));

        bool isfirst = true;
        while (true) {

            if (config->is_video()) {
                vid.read(src);
            } else {
                src = imread(srcUrl);
                try {
                    cvtColor(src, src, CV_BGR2GRAY);
                } catch (std::exception &e) {
                }
            }

            if ((src.rows <= 0) || (src.cols <= 0)) {
                std::cout << "Video is finished." << std::endl;
                break;
            }
            Mat temp;
            src.copyTo(temp);

            for (int i = 0; i < cds.size(); i++) {
                if (!cds[i].roiIsset()) {
                    cds[i].selectRegionOfInterest(src);
                }
                mog->apply(src(cds[i].regionOfInterestRect()), bg);
                if (config->is_manual()) {
                    imshow("frame", src(cds[i].regionOfInterestRect()));
                    waitKey(0);
                }
                if (!config->is_video()) {
                    cds[i].analyze(src(cds[i].regionOfInterestRect()));
                } else if (isfirst) {
                    cds[i].analyze(src(cds[i].regionOfInterestRect()));
                    isfirst = false;
                } else {
                    cds[i].analyze(bg);
                }
                if (cds[i].getAmount() != 0) {
                    resultFile << cds[i].getAmount() << "   ";

                    std::string t = std::to_string(cds[i].getAmount());

                    putText(temp, t, Point(cds[i].regionOfInterestRect().tl().x,cds[i].regionOfInterestRect().tl().y+20) ,
                            FONT_HERSHEY_SIMPLEX, 1, Scalar(0), 3);
                  
                }
            }

            for (int i = 0; i < dds.size(); i++) {
                if (!dds[i].roiIsset()) {
                    dds[i].selectRegionOfInterest(src);
                }
                dds[i].analyze(src(dds[i].regionOfInterestRect()));
                resultFile << dds[i].getAmount() << "   ";
                std::string t = std::to_string(dds[i].getAmount());

                putText(temp, t, Point(dds[i].regionOfInterestRect().tl().x,dds[i].regionOfInterestRect().tl().y+20),
                        FONT_HERSHEY_SIMPLEX, 1, Scalar(0), 3);
            }
            if (config->is_video()) {
                if (!outputVideo.isOpened()){
                    std::cout << "AHHHHHH" <<std::endl;
                }
                outputVideo.write(temp);
            }
            resultFile << std::endl;
            if (!config->is_video()) {
               
                imwrite("resources/result.jpg", temp);
                
                break;
            }
        }
        resultFile.close();

        return 0;

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
