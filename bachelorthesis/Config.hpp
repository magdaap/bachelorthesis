//
//  Config.hpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 15.10.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#ifndef Config_hpp
#define Config_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include <boost/variant.hpp>

class Config {

    struct Scale {
        Scale(int min, int max, int roiLeftX, int roiLeftY, int roiRightX,
              int roiRightY);

        const int min;
        const int max;
        const int roiLeftX;
        const int roiLeftY;
        const int roiRightX;
        const int roiRightY;
    };

  public:
    struct CircularScale : public Scale {
        CircularScale(int min, int max, int base, int roiLeftX, int roiLeftY,
                      int roiRightX, int roiRightY, const int middleX,
                      const int middleY, const int radius, const std::string calculationType);

        const int middleX;
        const int middleY;
        const int radius;
        const std::string calculationType;
        const int base;
    };

    struct DigitScale : public Scale {
        DigitScale(int min, int max, int roiLeftX, int roiLeftY, int roiRightX,
                   int roiRightY, const int digitCount);

        const int digitCount;
    };

    using ScaleVariant = boost::variant<CircularScale, DigitScale>;

    Config(bool video, bool manual);

    template <typename T> void addScale(const T &scale) {
        scales.emplace_back(scale);
    }

    /* --- Getter start --- */
    bool is_video() const;

    bool is_manual() const;

    const std::vector<ScaleVariant> &getScales() const;

    /* --- Getter end --- */

  private:
    /* --- Member start --- */
    bool manual;
    bool video;
    std::vector<ScaleVariant> scales;
    /* --- Member end --- */
};

#endif /* Config_hpp */
