//
//  circularDisplay.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 05.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef circularDisplay_hpp
#define circularDisplay_hpp

#include "Display.hpp"
#include <opencv2/opencv.hpp>
#include <stdio.h>

class CircularDisplay : public Display {
  public:
    CircularDisplay();
    CircularDisplay(int radius, cv::Point middle, int min, int max,
                    bool manual);

    virtual void analyse(cv::Mat img) override;

    void config(cv::Mat img);

    virtual void selectRegionOfInterest(const cv::Mat &img) override;

    virtual double getAmount() override;
    bool roi_isset();

  private:
    cv::Mat getLines(cv::Mat img);
    cv::Mat getLineAndScale(cv::Mat img);
    void setCircleMiddle(cv::Point p1, cv::Point p2, cv::Point p3);
    void setCircleRadius(cv::Point middle, cv::Point lin);
    void calculate(cv::Mat img);
    double getLinearAmount(double offset, double maxAmount);
    double getLogarithmicAmount(double offset, double maxAmount, double base);

    cv::Point middle, pointer;
    int radius;
    double amount;
    double shownAmount;
    int min;
    int max;
    bool manual;
};

#endif /* circularDisplay_hpp */
