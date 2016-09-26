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


Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/** @function thresh_callback */
void thresh_callback(int, void* )
{
    Mat threshold_output;
    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> hierarchy;
    
    /// Detect edges using Threshold
    threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
    /// Find contours
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    /// Approximate contours to polygons + get bounding rects and circles
    std::vector<std::vector<Point> > contours_poly( contours.size() );
    std::vector<Rect> boundRect( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    }
    
    
    /// Draw polygonal contour + bonding rects + circles
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours_poly, i, color, 1, 8, std::vector<Vec4i>(), 0, Point() );
        rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
    }
    
    /// Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
}

int main(int argc, const char * argv[]) {
    try{
        
        if( argc < 2 ) {
            std::cout << "No image data given \n" << std::endl;
            return -1;
        }
        cv::Mat image1, image2;
        image1 = imread( argv[1], CV_LOAD_IMAGE_COLOR );
        image2 = imread( argv[2], CV_LOAD_IMAGE_COLOR );
        
        Mat lines2 = DigitDisplay::getLines(image2);
        
        /// Convert image to gray and blur it
        cvtColor( image2, src_gray, CV_BGR2GRAY );
        blur( src_gray, src_gray, Size(3,3) );
        
        /// Create Window
        
        namedWindow( "Source", CV_WINDOW_AUTOSIZE );
        imshow( "Source", lines2 );
        
        createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
        thresh_callback( 0, 0 );
        
        waitKey(0);
        
        
        
        
        
        if ( !image1.data ){
            std::cout << "No image data1 \n" << std::endl;
        }
        if ( !image2.data ){
            std::cout << "No image data2 \n" << std::endl;
        }
        
        CircularDisplay cd = CircularDisplay::CircularDisplay();
        std::vector<Mat> analizedCircular = cd.analyse(image1);
        //Utils::showImages(analizedCircular, "image1");
        
        
        
        
        // Setup a rectangle to define your region of interest
        //    cv::Rect myROI(20, 20, image2.cols-40, image2.rows-40);
        
        // Crop the full image to that image contained by the rectangle myROI
        // Note that this doesn't copy the data
        //     cv::Mat croppedImage = image2(myROI);
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


