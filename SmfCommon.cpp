//
//  SmfCommon.cpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-0COORDINATESIZE-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "SmfCommon.hpp"
#include <cmath>

int mod(int x, int m)
{
    return (x%m + m)%m;
}

void NormalizeVector(float (&vector)[COORDINATESIZE])
{
    float length = sqrtf(powf(vector[0], 2.0) + powf(vector[1], 2.0) + powf(vector[2], 2.0));
    if(length != 0)
    {
        for(int i = 0; i<COORDINATESIZE; i++)
        {
            vector[i] = vector[i]/length;
        }
    }
}
//coord1 -> coord2
void CreateVector(const float (&coord1)[COORDINATESIZE], const float (&coord2)[COORDINATESIZE], float (&vector)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        vector[i] = coord2[i] - coord1[i];
    }
}
//coord1 -> coord2
void CreateVector(const float *coord1, const float *coord2, float (&vector)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        vector[i] = coord2[i] - coord1[i];
    }
}

void AddVectorToPoint(const float (&vec)[3], const float (&point)[3], float vectorScaleFactor, float (&result)[3])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        result[i] = point[i] + vec[i]*vectorScaleFactor;
    }
}

//vec1 X vec 2
void VectorCross(const float (&vec1)[COORDINATESIZE], const float (&vec2)[COORDINATESIZE], float (&cross)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        cross[i] = (vec1[mod(i+1,COORDINATESIZE)]*vec2[mod(i-1,COORDINATESIZE)] -
        vec1[mod(i-1,COORDINATESIZE)]*vec2[mod(i+1,COORDINATESIZE)]);
    }
    /*cross[0] = vec1[1]*vec2[2] - vec1[2] * vec2[1];
    cross[1] = vec1[2]*vec2[0] - vec1[0] * vec2[2];
    cross[2] = vec1[0]*vec2[1] - vec1[1] * vec2[0];*/
}
