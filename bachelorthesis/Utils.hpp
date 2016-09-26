//
//  Utils.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 25.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
class Utils {
    Utils(){};
public: static void showImages(std::vector<Mat> srcs, std::string title);
    void getVideo();
};

#endif /* Utils_hpp */
