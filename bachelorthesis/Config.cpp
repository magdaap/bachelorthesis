//
//  Config.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 15.10.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "Config.hpp"

Config::Scale::Scale(int min, int max, int roiLeftX, int roiLeftY,
                     int roiRightX, int roiRightY)
    : min(min), max(max), roiLeftX(roiLeftX), roiLeftY(roiLeftY),
      roiRightX(roiRightX), roiRightY(roiRightY) {}

Config::CircularScale::CircularScale(int min, int max, int base, int roiLeftX,
                                     int roiLeftY, int roiRightX, int roiRightY,
                                     const int middleX, const int middleY,
                                     const int radius,
                                     const int calculationType)
    : Scale(min, max, roiLeftX, roiLeftY, roiRightX, roiRightY),
      middleX(middleX), middleY(middleY), radius(radius), base(base), calculationType(calculationType) {}


Config::DigitScale::DigitScale(int min, int max, int roiLeftX, int roiLeftY,
                               int roiRightX, int roiRightY)
    : Scale(min, max, roiLeftX, roiLeftY, roiRightX, roiRightY) {}

bool Config::is_manual() const { return manual; }

bool Config::is_video() const { return video; }

const std::vector<Config::ScaleVariant> &Config::getScales() const {
    return scales;
}

Config::Config(bool video, bool manual) : video(video), manual(manual) {}
