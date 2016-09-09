//
//  digitDisplay.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 29.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef digitDisplay_hpp
#define digitDisplay_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
class DigitDisplay {

    DigitDisplay(){};
    
public: static Mat getLines(Mat img);
};

#endif /* digitDisplay_hpp */
