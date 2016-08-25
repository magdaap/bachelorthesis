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
#include "Utils.hpp"

using namespace cv;

int main(int argc, const char * argv[]) {
    try{
        if ( argc != 2 )
    {
         std::cout << "No image data given \n" << std::endl;
        return -1;

        
        }
    
    cv::Mat image;
    image = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
    if ( !image.data )
    {
        std::cout << "No image data \n" << std::endl;

    }
    Mat after = CircularDisplay::getLine(image, 10, 70);

        std::cout << "test" << std::endl;
        std::cout << after << std::endl;

        
    Utils::showImages(image, after);
    return 0;
        
    } catch(Exception& e){
        std::cerr << e.what() << std::endl;
    }

}
