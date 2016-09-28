//
//  circularDisplay.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 05.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef circularDisplay_hpp
#define circularDisplay_hpp

#include "category.hpp"
#include <opencv2/opencv.hpp>
#include <stdio.h>

class CircularDisplay : public Category {
  public:
    CircularDisplay();
    void analyse(cv::Mat img);
    cv::Mat getLines(cv::Mat img);
    void setCircleMiddle(cv::Point point1, cv::Point point2, cv::Point point3);
    void setCircleRadius(cv::Point middle, cv::Point lin);

  private:
    cv::Point middle;
    cv::Vec4i pointer;
    int radius;
    double amount;
};

#endif /* circularDisplay_hpp */
