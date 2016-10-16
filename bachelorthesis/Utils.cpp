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

namespace pt = boost::property_tree;

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

    auto config = std::make_shared<Config>(
        root.get<bool>("some_bool"), root.get<std::string>("some_string"),
        root.get<int>("some_int"), root.get<bool>("manual"));

    // Loop over all scales-Children (since it's an array)
    for (auto &n : root.get_child("scales")) {
        // All shared properties
        auto &scale = n.second;
        auto type = scale.get<std::string>("type");

        auto range = scale.get_child("range");
        auto min = range.get<int>("min");
        auto max = range.get<int>("max");
        auto roiLeftX = range.get<int>("roiLeftX");
        auto roiLeftY = range.get<int>("roiLeftY");
        auto roiRightX = range.get<int>("roiRightX");
        auto roiRightY = range.get<int>("roiRightY");
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

void Utils::getVideo() {
    /*   Mat test;
     VideoCapture camera(0);
     char k;
     bool t = false;
     CvFont font;
     cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4,0,1,8);
     while(true){
     camera.read(test);
     if (t)
     {
     std::string text("Circle");
     IplImage testm(test);
     cvPutText(&testm, text.c_str(), cvPoint(100, 100), &font,
     cvScalar(255,255,255));

     }
     imshow("test", test);
     k = waitKey(10);

     if (k == 'q')
     {
     break;
     }
     else if (k == 'c')
     {
     t = !t;
     }


     };*/
}
