//
//  Utils.cpp
//  bachelorthesis
//
//  Created by Magdalena Pröbstl on 25.08.16.
//  Copyright © 2016 Magdalena Pröbstl. All rights reserved.
//

#include "Utils.hpp"


void Utils::showImages(std::vector<Mat> imgs, std::string title){

    for( size_t i = 0; i < imgs.size(); i++ ){
   //     std::cout << i << std::endl;
        imshow(std::to_string(i)+ title, imgs[i]);
      
        
    }
    
};

