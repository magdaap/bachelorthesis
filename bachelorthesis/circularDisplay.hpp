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

using namespace cv;
class CircularDisplay {
    
            CircularDisplay(){};
    
    public: static Mat getLine(Mat img, double rho, double theta);
    
};

#endif /* circularDisplay_hpp */
