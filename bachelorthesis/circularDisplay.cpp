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
#include <Eigen/Dense>



using namespace cv;
Point p1, p2, p3, m, middle;
Vec4i pointer;
int r;
int radius;




void setCoordinates(int event,int x, int y, int flags, void* userdata) {
    
    if (flags == (EVENT_FLAG_SHIFTKEY + EVENT_FLAG_LBUTTON)){
        p1.x = x;
        p1.y = y;
        std::cout << p1 << "P1 koordinaten" << std::endl;
    } else if ( flags == (EVENT_FLAG_CTRLKEY + EVENT_FLAG_LBUTTON)){
        p2.x = x;
        p2.y = y;
        std::cout << p2 << "P2 koordinaten" << std::endl;
    } else if ( flags == (EVENT_FLAG_ALTKEY + EVENT_FLAG_LBUTTON)){
        p3.x = x;
        p3.y = y;
        std::cout << p3 << "P3 koordinaten" << std::endl;
    }
};

Point getMiddle(Point point1, Point point2, Point point3){
    Point middle;
    Eigen::Matrix3f A;
    Eigen::Vector3f b;
    A << 1,-point1.x,-point1.y,  1,-point2.x,-point2.y,  1,-point3.x,-point3.y;
    b << -(point1.x*point1.x+point1.y*point1.y), -(point2.x*point2.x+point2.y*point2.y), -(point3.x*point3.x+point3.y*point3.y);
    Eigen::Vector3f res = A.colPivHouseholderQr().solve(b);
    middle.x = res[1]/2;
    middle.y = res[2]/2;
    return middle;
};

int getRadius(Point middle, Point lin){
    Point diff = lin - middle;
    return sqrt((diff.x*diff.x) + (diff.y*diff.y));
};


 std::vector<Mat> CircularDisplay::analyseCircular(Mat img){
    std::vector<Mat> dest;

    Mat lines, c, res;
    char k, a;
    Point p;
    
     res = img;
    while (true){
        imshow("analyseCircular", img);
        imshow("result", res);
        setMouseCallback("analyseCircular", setCoordinates , NULL);
        k = waitKey(0);
        if (k == 'q'){
            break;
        
        } else if (k == 'c'){
            c = Mat(img.rows, img.cols, CV_8UC1);
            m = getMiddle(p1, p2, p3);
            std::cout << "The middle is:\n" << m << std::endl;
            r = getRadius(m, p1);
            std::cout << "The radius is:\n" << r << std::endl;
            circle(c, p1, 10, Scalar(42,42, 165), 2, 8, 0);
            circle(c, p2, 10, Scalar(165,42,42), 2, 8, 0);
            circle(c, p3, 10, Scalar(144,255, 30), 2, 8, 0);
            circle(c, m, r, Scalar(144,255, 30), 4, 8, 0);
            imshow("c", c);
            a = waitKey(0);
            if (a == 'y'){
                middle = m;
                radius = r;
                bitwise_and(img, c, res);
                Canny(res,res,50,100,3, true);
                
            }
        } else if (k == 'l'){
            line( img, m, p2, Scalar(144,255, 30), 2, 8, 0);
            a = waitKey(0);
            if (a == 'y'){
                pointer = {p1.x,p1.y,p2.x,p2.y};
            }
         }
    };
    dest.push_back(img);
    dest.push_back(lines);
    return dest;
};


Mat CircularDisplay::getLines(Mat img, Point p){
    Mat dest,edges;
    std::vector<Vec4i> lines;
    
    Canny(img,edges,50,100,3, true);
    
    cvtColor(edges, dest, CV_GRAY2BGR);
    HoughLinesP(edges, lines, 1, CV_PI/180, 100, 10, 5);

    
    for( size_t i = 0; i < lines.size(); i++ )
    {
        std::cout << lines[i] << "koordinaten" << std::endl;
        Vec4i l = lines[i];
        
        if ((((l[0] - p.x) < 20) && ((l[0] - p.x) > -20)) && (((l[1] - p1.y) < 20) && ((l[1] - p.y) > -20))){
            std::cout << Point(l[0], l[1]) << std::endl;
            line( dest, p, Point(l[2], l[3]), Scalar(0,0,255), 2, 8, 0);
        } else if ((((l[2] - p.x) < 20) && ((l[2] - p.x) > -20)) && (((l[3] - p1.y) < 20) && ((l[3] - p.y) > -20))){
            std::cout << Point(l[0], l[1]) << std::endl;
            line( dest, Point(l[0], l[1]),p, Scalar(0,0,255), 2, 8, 0);
        }
    }
    return dest;
};



