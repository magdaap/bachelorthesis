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
int r;
CircularDisplay::CircularDisplay()
    : middle(), pointer(), radius(), amount(), shownAmount(){};
CircularDisplay::CircularDisplay(int radius, cv::Point middle, int min, int max,
                                 bool manual)
    : middle(middle), pointer(), radius(radius), amount(), min(min), max(max),
      shownAmount(), manual(manual){};

const char *analyseCirc = "analyse Circular";
const char *setConfig = "select Scale with three points";

const char *progress = "Progress";

cv::Mat preprocessImage(cv::Mat img) { return img; };

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

void CircularDisplay::selectRegionOfInterest(const cv::Mat &img) {
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

void CircularDisplay::analyse(Mat img) {
    Mat res;
    res = getLineAndScale(img);
    calculate(res);
    double a = getLinearAmount(min, max);
    std::cout << a << " amount" << std::endl;
};

Mat CircularDisplay::getLineAndScale(Mat img) {
    Mat c, l, res, newLine, a, b;
    img.copyTo(res);
    // two empty pictures for
    c = Mat(img.rows, img.cols, CV_8UC1);
    l = Mat(img.rows, img.cols, CV_8UC1);

    l = getLines(img);
    circle(c, middle, radius, Scalar(255, 255, 255), 3, 8, 0);
    if (manual) {
        imshow(progress, c);
        waitKey(0);
        imshow(progress, l);
        waitKey(0);
        destroyWindow(progress);
    }
    bitwise_or(c, l, c);
    if (manual) {
        imshow(progress, c);
        waitKey(0);
        destroyWindow(progress);
    }

    bitwise_not(res, res);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2), Point(-1, -1));
    erode(c, c, kernel);
    dilate(c, c, kernel);
    //  cvtColor(res, res, CV_RGB2GRAY);
    bitwise_and(res, c, res);

    Canny(res, res, 50, 100, 3, true);
    if (manual) {
        imshow(progress, res);
        waitKey(0);
        destroyWindow(progress);
    }
    return res;
};

void CircularDisplay::setCircleMiddle(const Point point1, const Point point2,
                                      const Point point3) {

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

Mat CircularDisplay::getLines(Mat img) {
    Mat dest, edges, result, middlelines, asdf;
    std::vector<Vec4i> lines, second_lines;
    Mat element = getStructuringElement(MORPH_RECT, Size(2, 2), Point(-1, -1));
    erode(img, img, element);
    fastNlMeansDenoising(img, img);
    if (manual){
        imshow("denoise", img);
        waitKey(0);
        destroyWindow("denoise");
    }

    blur(img, img, Size(3, 3));

    Canny(img, edges, 50, 200, 3, true);

    HoughLinesP(img, lines, 1, CV_PI / 180, 20, 50, 1);

    result = Mat(img.rows, img.cols, CV_8UC1);
    middlelines = Mat(img.rows, img.cols, CV_8UC1);
    for (size_t i = 0; i < lines.size(); i++) {
        Vec4i l = lines[i];
        if (manual) {
            std::cout << l << std::endl;
        }
        line(result, Point(l[0], l[1]), Point(l[2], l[3]),
             Scalar(255, 255, 255), 2, 8, 0);
    }
    if (lines.size() != 0) {
        pointer = Point(lines[0][0], lines[0][1]);
    } else {
        pointer = Point(-1, -1);
    }
    //  imshow("middle", result);
   /* if (manual) {
        // gets the exact pointer
        bitwise_not(result, result);
        bitwise_and(result, middlelines, result);
        HoughLinesP(result, second_lines, 1, CV_PI / 180, 40, 70, 1);
        result.copyTo(asdf);
        for (size_t i = 0; i < second_lines.size(); i++) {
            Vec4i b = second_lines[i];

            line(result, Point(b[0], b[1]), Point(b[2], b[3]),
                 Scalar(255, 255, 255), 2, 8, 0);
        }

        imshow(progress, result);
        waitKey(0);
        if (second_lines.size() != 0) {
            pointer = Point(second_lines[0][0], second_lines[0][1]);
        } else {
            pointer = Point(-1, -1);
        }
    }*/
    return result;
};

void CircularDisplay::calculate(const Mat img) {

    std::vector<std::vector<Point>> contours;
    if (pointer == Point(-1, -1)) {
        std::cout << "No pointer was recognized" << std::endl;
        return;
    }
    findContours(img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    Mat test = Mat::zeros(img.size(), CV_8UC3);
    drawContours(test, contours, -1, Scalar(255), CV_FILLED);
    std::vector<Point> leftRightMost = getLeftRightMost(contours);
    Point leftmost = leftRightMost[0];
    Point rightmost = leftRightMost[1];

    // calculates the three vectors which define scale and pointer
    Eigen::Vector2d leftvec(leftmost.x - middle.x, leftmost.y - middle.y),
        rightvec(rightmost.x - middle.x, rightmost.y - middle.y),
        pointervec(pointer.x - middle.x, pointer.y - middle.y);

    line(test, leftmost, middle, Scalar(255, 255, 255));
    line(test, rightmost, middle, Scalar(255, 255, 255));
    line(test, pointer, middle, Scalar(255, 255, 255));
    if (manual){
        imshow(progress, test);
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

// calculates Linear Amount
double CircularDisplay::getLinearAmount(double minAmount, double maxAmount) {
    shownAmount = (amount * maxAmount) + minAmount;
    return shownAmount;
};

double CircularDisplay::getLogarithmicAmount(double minAmount, double maxAmount,
                                             double base) {
    shownAmount = log(getLinearAmount(minAmount, maxAmount)) / log(base);
    return shownAmount;
};

double CircularDisplay::getAmount() { return shownAmount; };

bool CircularDisplay::roi_isset() {
    return (regionOfInterestRect() != Rect(Point(0, 0), Point(0, 0)));
};
