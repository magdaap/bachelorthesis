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

// Lets use a class for nice, epic immutable containedness
class Config {

    // Base Scale configuration object, all shared values go here
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
    // The CircularScale object, inherits from Scale
    // Holds the CircularScale-only values
    struct CircularScale : public Scale {
        CircularScale(int min, int max, int roiLeftX, int roiLeftY,
                      int roiRightX, int roiRightY, const int middleX,
                      const int middleY, const int radius);

        const int middleX;
        const int middleY;
        const int radius;
    };

    // Same as CircularScale, but Digit!
    struct DigitScale : public Scale {
        DigitScale(int min, int max, int roiLeftX, int roiLeftY, int roiRightX,
                   int roiRightY, const int digitCount);

        const int digitCount;
    };

    using ScaleVariant = boost::variant<CircularScale, DigitScale>;

    // Simply the constructor for our class
    Config(bool some_bool, const std::string &some_string, int some_int);

    template <typename T> void addScale(const T &scale) {
        scales.emplace_back(scale);
    }

    // Self-explanatory from here on

    /* --- Getter start --- */
    bool isSome_bool() const;

    bool is_manual() const;

    const std::string &getSome_string() const;

    int getSome_int() const;

    const std::vector<ScaleVariant> &getScales() const;

    /* --- Getter end --- */

  private:
    /* --- Member start --- */
    bool some_bool;
    bool manual;
    std::string some_string;
    int some_int;
    std::vector<ScaleVariant> scales;
    /* --- Member end --- */
};

#endif /* Config_hpp */
