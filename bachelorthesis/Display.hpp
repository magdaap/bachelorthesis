//
//  Display.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 30.10.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef Display_hpp
#define Display_hpp

#include <stdio.h>

#include <opencv/cv.hpp>

class Display {

  public:
    /**
     * Calibrate this display for the given image and display type.
     * @param baseImage
     * @return True if calibrating was successful, false otherwise
     */
    virtual void selectRegionOfInterest(const cv::Mat &img) = 0;

    virtual void analyze(cv::Mat img) = 0;

    virtual double getAmount() = 0;

    const cv::Mat &regionOfInterest();

    const cv::Rect &regionOfInterestRect();
    void setROI(cv::Rect roi);

  protected:
    void selectROI(const cv::Mat &img);

  private:
    cv::Rect roi;
    cv::Mat roiImg;
    bool manual;
};

#endif /* Display_hpp */
