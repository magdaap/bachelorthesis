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
    cv::Mat getLineAndScale(cv::Mat img);
    void setCircleMiddle(cv::Point p1, cv::Point p2, cv::Point p3);
    void setCircleRadius(cv::Point middle, cv::Point lin);
    void calculate(cv::Mat img);
    double getLinearAmount(double offset, double maxAmount, double amount);
  private:
    cv::Point middle, pointer;
    int radius;
    double amount;
};

#endif /* circularDisplay_hpp */
