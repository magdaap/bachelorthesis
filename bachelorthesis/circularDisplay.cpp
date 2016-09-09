//
//  circularDisplay.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 05.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "circularDisplay.hpp"
#include <opencv2/opencv.hpp>
#include "Utils.hpp"


using namespace cv;
Point p;

void setCoordinates(int event,int x, int y, int flags, void* userdata) {
    
    if  ( event == EVENT_LBUTTONDOWN )
    {
       
        p.x = x;
        p.y = y;
        
        std::cout << p << "koordinaten" << std::endl;

    };
};


 std::vector<Mat> CircularDisplay::analyseCircular(Mat img){
    std::vector<Mat> dest;

    Mat lines, shape;
     char k,l;
     Point p;
     while (true){
         imshow("analyseCircular", img);
         setMouseCallback("analyseCircular", setCoordinates , NULL);
         k = waitKey(0);
            if (k == 'q')
         {
             break;
         };
         if (k == 'l'){
            img = CircularDisplay::getLines(img, p);             
         }
     };
     
    shape = CircularDisplay::getShape(img);
     
     dest.push_back(img);
     dest.push_back(lines);
     dest.push_back(shape);
    return dest;
};


Mat CircularDisplay::getLines(Mat img, Point p){
    Mat dest,edges;
    std::vector<Vec4i> lines;
    
    Canny(img,edges,50,100,3, true);
    
    cvtColor(edges, dest, CV_GRAY2BGR);
    HoughLinesP(edges, lines, 1, CV_PI/180, 110, 10, 5);
    
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        
        if ((((l[0] - p.x) < 10) && ((l[0] - p.x) > -10)) && (((l[1] - p.y) < 10) && ((l[1] - p.y) > -10))){
            std::cout << Point(l[0], l[1]) << std::endl;
            line( dest, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 2, 8, 0);

        
        }
        
    }
    return dest;
    
};


Mat CircularDisplay::getShape(Mat src){
    
    Mat dest,edges, test ;
    //cvtColor( src, edges, CV_BGR2GRAY );
    
    //   Canny(src,edges,50,0,3, true);
    GaussianBlur( src, edges, Size(9,9), 6, 6);
   // std::cout << edges << std::endl;

    std::vector<Vec3f> circles;
    cvtColor(edges, edges, CV_GRAY2BGR);
    Canny(edges,dest,50,200,3, true);
    
    test = dest;
    

    HoughCircles(dest, circles, CV_HOUGH_GRADIENT, 1, edges.rows/8, 200, 100, 0, 0 );

    for( size_t i = 0; i < circles.size(); i++ )
    {
      //  std::cout << circles[i] << std::endl;

        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle( dest, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        circle( dest, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    return dest;
};

