//
//  main.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 05.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "Utils.hpp"
#include "circularDisplay.hpp"
#include "digitDisplay.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

const char* src_window = "Select ROI";

Point point;
void setPoint(int event, int x, int y, int flags, void *userdata) {

    if (event == (EVENT_LBUTTONDOWN)) {
        point.x = x;
        point.y = y;}

};

Mat selectAreaOfInterest(Mat img){
    Mat aoi;
    Point leftup, rightdown;
    char l;
    
    while(true){
        imshow(src_window, img);
        setMouseCallback(src_window, setPoint, NULL);
        l = waitKey(0);
        if (l == '1'){
            leftup = point;
        }
        else if (l == '2'){
            rightdown = point;
            rectangle(img, leftup, rightdown, Scalar(255));
        }
        else if (l == 'q'){
            aoi = img (Rect(leftup,rightdown));
            break;
        }
        
    }
    
    return aoi;
};

int main(int argc, const char *argv[]) {
    try {

        if (argc < 2) {
            std::cout << "No image data given \n" << std::endl;
            return -1;
        }
        cv::Mat image1, image2, src, roi;
        char k;
        CircularDisplay cd;
        DigitDisplay dd;

        VideoCapture vid("/Users/magdalenaprobstl/Desktop/test_video1_kurz.mp4");
        if (!vid.isOpened()){
            std::cout << "Cannot open video!\n";
            return -1;
        }
        image1 = imread(argv[1], CV_LOAD_IMAGE_COLOR);
        image2 = imread(argv[2], CV_LOAD_IMAGE_COLOR);

        while(true){
            vid.read(src);
            std::cout << "format " << vid.get(CV_CAP_PROP_FORMAT) << std::endl;
            imshow("videoinput", src);
            
            k = waitKey(0);
            if (k == 'c'){
                roi = selectAreaOfInterest(src);
                cd.analyse(roi);
            }
            else if (k == 'd'){
                roi = selectAreaOfInterest(src);
                dd.analyse(roi);
            }
            
            else if (k == 'q')
            {
                break;
            }
            
        
        }
        
      //  dd.analyse(image2);

        if (!image1.data) {
            std::cout << "No image data1 \n" << std::endl;
        }
        if (!image2.data) {
            std::cout << "No image data2 \n" << std::endl;
        }

      //  cd.analyse(image1);

        return 0;

    } catch (Exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
