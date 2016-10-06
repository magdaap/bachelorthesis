//
//  Utils.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 25.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "Utils.hpp"

void Utils::showImages(std::vector<cv::Mat> imgs, std::string title) {

    for (size_t i = 0; i < imgs.size(); i++) {
        //     std::cout << i << std::endl;
        imshow(std::to_string(i) + title, imgs[i]);
    }
};

void Utils::getVideo() {
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
     cvPutText(&testm, text.c_str(), cvPoint(100, 100), &font,
     cvScalar(255,255,255));

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

    // Utils::showImages(analizedCircular, "image1");

    // Setup a rectangle to define your region of interest
    //    cv::Rect myROI(20, 20, image2.cols-40, image2.rows-40);

    // Crop the full image to that image contained by the rectangle myROI
    // Note that this doesn't copy the data
    //     cv::Mat croppedImage = image2(myROI);
}
