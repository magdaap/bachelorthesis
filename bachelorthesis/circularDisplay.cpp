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
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;

Point p1, p2, p3;
CircularDisplay::CircularDisplay()
    : middle(), pointer(), radius(), amount(), shownAmount(),
      testDeleteLATER() {
    testDeleteLATER = 0;
};
CircularDisplay::CircularDisplay(int radius, cv::Point middle, int min, int max,
                                 bool manual)
    : middle(middle), pointer(), radius(radius), amount(), min(min), max(max),
      shownAmount(), manual(manual), testDeleteLATER() {
    testDeleteLATER = 0;
};

// Names for imshow()
const char *analyzeCirc = "analyze Circular";
const char *setConfig = "select Scale with three points";
const char *progress = "Progress";

// Method to set coordinates for defining the scale's circle
void setCoordinates(int event, int x, int y, int flags, void *userdata) {

    if (flags == (EVENT_FLAG_SHIFTKEY + EVENT_FLAG_LBUTTON)) {
        p1.x = x;
        p1.y = y;
    } else if (flags == (EVENT_FLAG_CTRLKEY + EVENT_FLAG_LBUTTON)) {
        p2.x = x;
        p2.y = y;
    } else if (flags == (EVENT_FLAG_ALTKEY + EVENT_FLAG_LBUTTON)) {
        p3.x = x;
        p3.y = y;
    }
};

// help method to get the scale endings -> gets the leftmost and rightmost
// points
std::vector<Point> getLeftRightMost(std::vector<std::vector<Point>> cont) {
    Point leftmost = cont[0][0];
    Point rightmost = cont[0][0];
    for (int i = 0; i < cont.size(); i++) {
        for (int j = 0; j < cont[i].size(); j++) {
            if (cont[i][j].x < leftmost.x) {
                leftmost = cont[i][j];
            }
            if (cont[i][j].x > rightmost.x) {
                rightmost = cont[i][j];
            }
        }
    }
    return {leftmost, rightmost};
};

void CircularDisplay::analyze(Mat img) {
    Mat res;
    res = getLineAndScale(img);
    calculate(res);
    double a = getLinearAmount(min, max);
    std::cout << a << " amount" << std::endl;
    testDeleteLATER++;
};

// returns an image containing the sceleton of the scale and the pointer
Mat CircularDisplay::getLineAndScale(Mat src) {
    Mat c, l, res;
    src.copyTo(res);
    // two empty pictures for
    c = Mat(src.rows, src.cols, CV_8UC1);
    l = Mat(src.rows, src.cols, CV_8UC1);

    l = getLines(res);
    circle(c, middle, radius, Scalar(255, 255, 255), 3, 8, 0);
    if (manual) {
        imshow("c", c);
        waitKey(0);
        imshow("l", l);
        waitKey(0);
        destroyWindow("c");
        destroyWindow("l");
    }
    circle(l, middle, radius, Scalar(255, 255, 255));
    if (manual) {
        imshow("l", l);
        waitKey(0);
    }
    return l;

    /* This is for recognition if the region of interest isn't set on the scale
     * endings.*/
    /*
     threshold(res, res, 50, 250, THRESH_BINARY);
     imshow("threshold", res);
     waitKey(0);
     bitwise_not(res, res);
     if (manual) {
         imshow("c", c);
         imshow("res", res);
         waitKey(0);
     }
     fastNlMeansDenoising(c, c);
     fastNlMeansDenoising(res, res);
     if (manual) {
         imshow("c", c);
         imshow("res", res);
         waitKey(0);
         destroyWindow("c");
         destroyWindow("res");
     }
     Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2), Point(-1, -1));
     //erode(c, c, kernel);
    // dilate(c, c, kernel);
     //  cvtColor(res, res, CV_RGB2GRAY);
     bitwise_and(res, c, res);

     Canny(res, res, 50, 100, 3, true);
     if (manual) {
         imshow(progress, res);
         waitKey(0);
         destroyWindow(progress);
     }
     return res;*/
};

Mat CircularDisplay::getLines(Mat img) {
    Mat dest, edges, result, middlelines;
    std::vector<Vec4i> lines, second_lines;

    threshold(img, img, 60, 250, THRESH_BINARY);
    if (manual) {
        imshow("denoise", img);
        waitKey(0);
        destroyWindow("denoise");
    }
    fastNlMeansDenoising(img, img, 30);
    if (manual) {
        imshow("denoise", img);
        waitKey(0);
        destroyWindow("denoise");
    }
    Canny(img, edges, 50, 200, 3, true);
    if (manual) {
        imshow("denoise", edges);
        waitKey(0);
        destroyWindow("denoise");
    }
    HoughLinesP(img, lines, 1, CV_PI / 180, 50, 50, 1);

    result = Mat(img.rows, img.cols, CV_8UC1);
    middlelines = Mat(img.rows, img.cols, CV_8UC1);

    if ((lines.size() != 0) && (lines.size() < 10)) {

        pointer = Point(lines[0][2], lines[0][3]);
        line(result, pointer, middle, Scalar(255, 255, 255), 2, 8, 0);

    } else {
        pointer = Point(-1, -1);
    }

    return result;
};

void CircularDisplay::selectRegionOfInterest(Mat img) {
    selectROI(img);

    if (manual) {
        imshow(setConfig, regionOfInterest());
        setMouseCallback(setConfig, setCoordinates, NULL);
        while (true) {
            auto k = waitKey(0);
            if (k == 'c') {
                setCircleMiddle(p1, p2, p3);
                setCircleRadius(middle, p1);
            } else if (k == 'q') {
                destroyWindow(setConfig);
                break;
            }
        }
        std::cout << "Middle: " << middle << std::endl;
        std::cout << "Radius: " << radius << std::endl;
    }
}

double CircularDisplay::getAmount() { return shownAmount; };

bool CircularDisplay::roiIsset() {
    return (regionOfInterestRect() != Rect(Point(0, 0), Point(0, 0)));
};

/*--- CALCULATION METHODS ---*/

/** Calculates the center of the circle which is defined by the three points
 */
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

// Calculates the radius between the center and a point lying on the circle the
// radius builds
void CircularDisplay::setCircleRadius(Point middle, Point lin) {
    Point diff = lin - middle;
    radius = sqrt((diff.x * diff.x) + (diff.y * diff.y));
    std::cout << "The radius is:\n" << radius << std::endl;
};

// calculates Linear Amount
double CircularDisplay::getLinearAmount(double minAmount, double maxAmount) {
    shownAmount = (amount * maxAmount) + minAmount;
    return shownAmount;
};

// calculates Logarithmic Amount
double CircularDisplay::getLogarithmicAmount(double minAmount, double maxAmount,
                                             double base) {
    shownAmount = log(getLinearAmount(minAmount, maxAmount)) / log(base);
    return shownAmount;
};

void CircularDisplay::calculate(Mat img) {
    Mat src = Mat::zeros(img.rows, img.cols, CV_8UC1);
    line(src, pointer, middle, Scalar(255, 255, 255));
    circle(src, middle, radius, Scalar(255, 255, 255));
    std::vector<std::vector<Point>> contours;
    if ((pointer.x == -1) && (pointer.y == -1)) {
        amount = 0;
        std::cout << "No pointer was recognized" << std::endl;
        return;
    }
    findContours(src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    drawContours(src, contours, -1, Scalar(255), CV_FILLED);
    std::vector<Point> leftRightMost = getLeftRightMost(contours);
    Point leftmost = leftRightMost[0];
    Point rightmost = leftRightMost[1];

    // calculates the three vectors which define scale and pointer
    Eigen::Vector2d leftvec(leftmost.x - middle.x, leftmost.y - middle.y),
        rightvec(rightmost.x - middle.x, rightmost.y - middle.y),
        pointervec(pointer.x - middle.x, pointer.y - middle.y);

    if (manual) {
        line(src, leftmost, middle, Scalar(255, 255, 255));
        line(src, rightmost, middle, Scalar(255, 255, 255));
        line(src, pointer, middle, Scalar(255, 255, 255));

        imshow(progress, src);
        waitKey(0);
        destroyWindow(progress);
    }
    // normalize every pointer for calculation
    leftvec.normalize();
    rightvec.normalize();
    pointervec.normalize();

    /** dataprodAmount: dotproduct between leftmost and pointer
     *  dotprodGeneral: dotproduct between leftmost and rightmost
     */
    double dotprodAmount = leftvec.dot(pointervec);
    double dotprodGeneral = leftvec.dot(rightvec);

    // calculates the angles between leftmost and pointer and between leftmost
    // and rightmost
    double angleAmount = acos(dotprodAmount);
    double angleGeneral = acos(dotprodGeneral);

    // amount independent of scale
    amount = angleAmount / angleGeneral;
};
