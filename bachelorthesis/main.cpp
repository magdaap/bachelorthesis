//
//  main.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 05.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "circularDisplay.hpp"

using namespace cv;

int main(int argc, const char * argv[]) {
    try{
        if ( argc != 2 )
    {
        return -1;
            
        }
    
    cv::Mat image;
    image = cv::imread( argv[1], 0 );
    if ( !image.data )
    {
        std::cout << "No image data \n" << std::endl;

    }
    Mat after = CircularDisplay::getLine(image);

        std::cout << "test" << std::endl;
        std::cout << after << std::endl;

        
    CircularDisplay::showImages(image, image);
    return 0;
        
    } catch(cv::Exception& e){
        std::cerr << e.what() << std::endl;
    }

}
