//
//  digitDisplay.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 29.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef digitDisplay_hpp
#define digitDisplay_hpp

#include "category.hpp"
#include <opencv2/opencv.hpp>
#include <stdio.h>

class DigitDisplay : public Category {

  public:
    DigitDisplay();
    DigitDisplay(cv::Rect roi);
    cv::Mat getElements(cv::Mat img);
    void analyse(cv::Mat img);
    double getAmount();

  private:
    cv::Rect roi;
    double shownAmount;
};

#endif /* digitDisplay_hpp */
