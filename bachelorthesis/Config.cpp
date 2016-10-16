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

Config::CircularScale::CircularScale(int min, int max, int roiLeftX,
                                     int roiLeftY, int roiRightX, int roiRightY,
                                     const int middleX, const int middleY,
                                     const int radius)
    : Scale(min, max, roiLeftX, roiLeftY, roiRightX, roiRightY),
      middleX(middleX), middleY(middleY), radius(radius) {}

Config::DigitScale::DigitScale(int min, int max, int roiLeftX, int roiLeftY,
                               int roiRightX, int roiRightY,
                               const int digitCount)
    : Scale(min, max, roiLeftX, roiLeftY, roiRightX, roiRightY),
      digitCount(digitCount) {}

bool Config::isSome_bool() const { return some_bool; }
bool Config::is_manual() const { return manual; }

const std::string &Config::getSome_string() const { return some_string; }

int Config::getSome_int() const { return some_int; }

const std::vector<Config::ScaleVariant> &Config::getScales() const {
    return scales;
}

Config::Config(bool some_bool, const std::string &some_string, int some_int,
               bool manual)
    : some_bool(some_bool), some_string(some_string), some_int(some_int),
      manual(manual) {}
