//
//  Utils.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 25.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include "Config.hpp"
#include <opencv2/opencv.hpp>
#include <stdio.h>

class Utils {
    Utils(){};

  public:
    static void showImages(std::vector<cv::Mat> srcs, std::string title);
    void getVideo();
    static std::shared_ptr<Config::Config> readConfig(const char *url);
};

#endif /* Utils_hpp */
