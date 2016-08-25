//
//  Utils.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 25.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "Utils.hpp"


void Utils::showImages(Mat src, Mat evol){
    namedWindow("Src", WINDOW_AUTOSIZE );
    imshow("Src", src);
    namedWindow("Evolved", WINDOW_AUTOSIZE );
    imshow("Evolved", evol);
    waitKey(0);
};
