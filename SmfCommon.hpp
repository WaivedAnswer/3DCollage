//
//  SmfCommon.hpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef SmfCommon_hpp
#define SmfCommon_hpp

#include <stdio.h>

const int COORDINATESIZE = 3;
const int MESHITEMSIZE = 3;
const int QUADRICSIZE = 10;

int mod(int x, int m);

void NormalizeVector(float (&vector)[3]);
//coord1 -> coord2
void CreateVector(const float (&coord1)[3], const float (&coord2)[3], float (&vector)[3]);
void CreateVector(const float *coord1, const float *coord2, float (&vector)[3]);
void AddVectorToPoint(const float (&vec)[3], const float (&point)[3], float vectorScaleFactor, float (&result)[3]);

//vec1 X vec 2
void VectorCross(const float (&vec1)[3], const float (&vec2)[3], float (&cross)[3]);

#endif /* SmfCommon_hpp */
