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
#include "digitDisplay.hpp"
#include "Utils.hpp"

using namespace cv;

int main(int argc, const char * argv[]) {
    try{
        
        if( argc < 2 ) {
            std::cout << "No image data given \n" << std::endl;
            return -1;
        }
     /*   Mat test;
        VideoCapture camera(0);
        char k;
        bool t = false;
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4,0,1,8);
        while(true){
            camera.read(test);
            if (t)
            {
                std::string text("Circle");
                IplImage testm(test);
                cvPutText(&testm, text.c_str(), cvPoint(100, 100), &font, cvScalar(255,255,255));
                
            }
            imshow("test", test);
            k = waitKey(10);
            
            if (k == 'q')
            {
                break;
            }
            else if (k == 'c')
            {
                t = !t;
            }
            

        };*/
        
        
        cv::Mat image1, image2;
        image1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
        image2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );

        
        if ( !image1.data ){
            std::cout << "No image data1 \n" << std::endl;
        }
        if ( !image2.data ){
            std::cout << "No image data2 \n" << std::endl;
        }

    
        std::vector<Mat> analizedCircular = CircularDisplay::analyseCircular(image1);
        Utils::showImages(analizedCircular, "image1");
        
        
 
        
        // Setup a rectangle to define your region of interest
        cv::Rect myROI(20, 20, image2.cols-40, image2.rows-40);
        
        // Crop the full image to that image contained by the rectangle myROI
        // Note that this doesn't copy the data
        cv::Mat croppedImage = image2(myROI);
        Mat lines2 = DigitDisplay::getLines(image2);

        std::vector<Mat> images;
        
        
        images.push_back(image2);
        images.push_back(lines2);
     
        Utils::showImages(images, "image2");
        
        waitKey(0);

        return 0;
        
    } catch(Exception& e){
        std::cerr << e.what() << std::endl;
    }
}
