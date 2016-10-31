//
//  digitDisplay.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 29.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef digitDisplay_hpp
#define digitDisplay_hpp

#include "Display.hpp"
#include <opencv2/opencv.hpp>
#include <stdio.h>

class DigitDisplay : public Display {

  public:
    DigitDisplay();
    DigitDisplay(int max, bool manual);

    virtual void analyze(cv::Mat img) override;
    virtual double getAmount() override;
    bool roiIsset();
    virtual void selectRegionOfInterest(const cv::Mat &img) override;

  private:
    cv::Mat preprocessImage(cv::Mat img);

    void getText(cv::Mat img);

    int max;
    double shownAmount;
    bool manual;
};

#endif /* digitDisplay_hpp */
