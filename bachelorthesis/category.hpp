//
//  category.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 26.09.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef category_hpp
#define category_hpp

#include <opencv2/opencv.hpp>
#include <stdio.h>

class Category {
  public:
    virtual void analyse(cv::Mat img) = 0;
};

#endif /* category_hpp */
