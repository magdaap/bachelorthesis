//
//  category.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 26.09.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef category_hpp
#define category_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>


using namespace cv;
class Category {
public: virtual std::vector<Mat> analyse(Mat img);

};

#endif /* category_hpp */
