//
//  circularDisplay.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 05.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef circularDisplay_hpp
#define circularDisplay_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "category.hpp"

using namespace cv;
class CircularDisplay : public Category {
    
    
       
    
public: CircularDisplay();
    
    std::vector<Mat> analyse(Mat img);
    Mat getLines(Mat img);
    void setCircleMiddle(Point point1, Point point2, Point point3);
    void setCircleRadius(Point middle, Point lin);
private:
    Point middle;
    Vec4i pointer;
    int radius;
    double amount;
    
    
};

#endif /* circularDisplay_hpp */
