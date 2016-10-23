//
//  Utils.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 25.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "Utils.hpp"
#include "Config.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <opencv2/opencv.hpp>

namespace pt = boost::property_tree;

const char *src_window = "Select ROI";

void Utils::showImages(std::vector<cv::Mat> imgs, std::string title) {

    for (size_t i = 0; i < imgs.size(); i++) {
        //     std::cout << i << std::endl;
        imshow(std::to_string(i) + title, imgs[i]);
    }
};

// Shorter, boost namespaces are fucking long

std::shared_ptr<Config::Config> Utils::readConfig(const char *url) {

    // Root of the whole json file
    pt::ptree root;

    // Read the json file into root, throws on invalid syntax
    pt::json_parser::read_json(url, root);
    auto config = std::make_shared<Config>(root.get<bool>("video"),
                                           root.get<bool>("manual"));

    // Loop over all scales-Children (since it's an array)
    for (auto &n : root.get_child("scales")) {
        // All shared properties
        auto &scale = n.second;
        auto type = scale.get<std::string>("type");

        auto range = scale.get_child("range");
        auto min = range.get<int>("min");
        auto max = range.get<int>("max");
        auto roiLeftX = scale.get<int>("roiLeftX");
        auto roiLeftY = scale.get<int>("roiLeftY");
        auto roiRightX = scale.get<int>("roiRightX");
        auto roiRightY = scale.get<int>("roiRightY");
        if (type == "analog") {
            // Here we have analog
            config->addScale(Config::CircularScale(
                min, max, roiLeftX, roiLeftY, roiRightX, roiRightY,
                scale.get<int>("middleX"), scale.get<int>("middleY"),
                scale.get<int>("radius")));
        } else if (type == "digital") {
            // Here we have digital
            config->addScale(Config::DigitScale(min, max, roiLeftX, roiLeftY,
                                                roiRightX, roiRightY,
                                                // Digigal-only variable
                                                scale.get<int>("digitCount")));
        } else {
            // Throw on unsupported scale type
            throw std::runtime_error("Unsupported scale type: " + type);
        }
    }

    return config;
};
cv::Point point;
void setPoint(int event, int x, int y, int flags, void *userdata) {

    if (event == (cv::EVENT_LBUTTONDOWN)) {
        point.x = x;
        point.y = y;
    }
};

cv::Mat Utils::selectAreaOfInterest(cv::Mat img) {
    cv::Mat aoi;
    cv::Point leftup, rightdown;
    char l;
    img.copyTo(aoi);
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
            aoi = aoi(cv::Rect(leftup, rightdown));
            cv::destroyWindow(src_window);

            break;
        }
    }

    return aoi;
};
