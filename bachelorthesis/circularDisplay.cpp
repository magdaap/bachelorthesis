//
//  circularDisplay.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 05.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "circularDisplay.hpp"
#include "Utils.hpp"
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

using namespace cv;

Point p1, p2, p3;
int r;
CircularDisplay::CircularDisplay() : middle(), pointer(), radius(){};

void setCoordinates(int event, int x, int y, int flags, void *userdata) {

    if (flags == (EVENT_FLAG_SHIFTKEY + EVENT_FLAG_LBUTTON)) {
        p1.x = x;
        p1.y = y;
        std::cout << p1 << "P1 koordinaten" << std::endl;
    } else if (flags == (EVENT_FLAG_CTRLKEY + EVENT_FLAG_LBUTTON)) {
        p2.x = x;
        p2.y = y;
        std::cout << p2 << "P2 koordinaten" << std::endl;
    } else if (flags == (EVENT_FLAG_ALTKEY + EVENT_FLAG_LBUTTON)) {
        p3.x = x;
        p3.y = y;
        std::cout << p3 << "P3 koordinaten" << std::endl;
    }
};

void CircularDisplay::setCircleMiddle(Point point1, Point point2,
                                      Point point3) {

    Eigen::Matrix3f A;
    Eigen::Vector3f b;
    A << 1, -point1.x, -point1.y, 1, -point2.x, -point2.y, 1, -point3.x,
        -point3.y;
    b << -(point1.x * point1.x + point1.y * point1.y),
        -(point2.x * point2.x + point2.y * point2.y),
        -(point3.x * point3.x + point3.y * point3.y);
    Eigen::Vector3f res = A.colPivHouseholderQr().solve(b);
    middle.x = res[1] / 2;
    middle.y = res[2] / 2;
    std::cout << "The middle is:\n" << middle << std::endl;

};

void CircularDisplay::setCircleRadius(Point middle, Point lin) {
    Point diff = lin - middle;
    radius = sqrt((diff.x * diff.x) + (diff.y * diff.y));
    std::cout << "The radius is:\n" << radius << std::endl;

};

void CircularDisplay::analyse(Mat img) {
    std::vector<Mat> dest;

    Mat lines, c, l, res, src;
    char k;
    Point p;
    img.copyTo(src);
    img.copyTo(res);
    while (true) {
        imshow("analyseCircular", img);
        imshow("original", src);
        setMouseCallback("analyseCircular", setCoordinates, NULL);
        k = waitKey(0);
        if (k == 'q') {
            break;

        } else if (k == 'c') {
            c = Mat(img.rows, img.cols, CV_8UC1);
            l = Mat(img.rows, img.cols, CV_8UC1);
            setCircleMiddle(p1, p2, p3);
            setCircleRadius(middle, p1);
            l = getLines(src);
            circle(c, middle, radius, Scalar(255, 255, 255), 1, 8, 0);
            bitwise_or(c, l, c);
            bitwise_not(res, res);
            imshow("line", l);
           // bitwise_and(res, c, res);

            imshow("result", res);
            imshow("c", c);


            waitKey(0);
            // GaussianBlur( res, res, Size(9,9), 6, 6);

            //  src.copyTo(img);

        } else if (k == 'l') {
            c = getLines(src);
            imshow("c", getLines(src));

            /*   line( img, middle, p2, Scalar(144,255, 30), 2, 8, 0);
               a = waitKey(0);
               if (a == 'y'){
                   pointer = {p1.x,p1.y,p2.x,p2.y};
               }*/
        }
    };
};

Mat CircularDisplay::getLines(Mat img) {
    Mat dest, edges, test, middlelines;
    std::vector<Vec4i> lines;

    Canny(img, edges, 50, 200, 3, true);
    cvtColor(edges, dest, CV_GRAY2BGR);
    HoughLinesP(edges, lines, 1, CV_PI / 180, 20, 50, 1);

    test = Mat(img.rows, img.cols, CV_8UC1);
    middlelines = Mat(img.rows, img.cols, CV_8UC1);

    for (size_t i = 0; i < lines.size(); i++) {
        Vec4i l = lines[i];

        std::cout << lines[i] << "pointer koordinaten" << std::endl;

        line(test, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 255),
             1, 8, 0);

        line(middlelines, Point(l[0], l[1]), middle, Scalar(255, 255, 255), 1,
             8, 0);
        line(middlelines, Point(l[2], l[3]), middle, Scalar(255, 255, 255), 1,
             8, 0);
    }

    bitwise_not(test, test);
    bitwise_and(test, middlelines, test);
    // GaussianBlur( res, res, Size(9,9), 6, 6);
    Canny(test, test, 50, 100, 3, true);

    return test;
};
