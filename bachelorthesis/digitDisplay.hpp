//
//  digitDisplay.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 29.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef digitDisplay_hpp
#define digitDisplay_hpp

#include <opencv2/opencv.hpp>
#include <stdio.h>

class DigitDisplay {

  public:
    DigitDisplay();
    DigitDisplay(cv::Rect roi, int max);
    void getText(cv::Mat img);
    void analyse(cv::Mat img);
    cv::Mat preprocessImage(cv::Mat img);
    double getAmount();
    bool roi_isset();
    void set_roi(cv::Rect roi);

  private:
    int max;
    cv::Rect roi;
    double shownAmount;
};

#endif /* digitDisplay_hpp */
