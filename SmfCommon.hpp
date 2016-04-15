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
typedef float Point[COORDINATESIZE];
typedef float Vector3[COORDINATESIZE];
int mod(int x, int m);

void NormalizeVector(float (&vector)[3]);
//coord1 -> coord2
void CreateVector(const float (&coord1)[3], const float (&coord2)[3], float (&vector)[3]);
void CreateVector(const float *coord1, const float *coord2, float (&vector)[3]);
void ScaleVector(float (&vec)[3], float scalingFactor);
void ScaleXDimVector(float (&vec)[3], float scalingFactor);
void ScaleYDimVector(float (&vec)[3], float scalingFactor);
void ScaleZDimVector(float (&vec)[3], float scalingFactor);
void AddCoordinates(const float (&point1)[3], const float (&point2)[3], float (&result)[3]);
void ScaleCoordinates(float (&point)[3], float scalingFactor);
void ScaleXDimCoordinate(float (&vec)[3], float scalingFactor);
void ScaleYDimCoordinate(float (&vec)[3], float scalingFactor);
void ScaleZDimCoordinate(float (&vec)[3], float scalingFactor);
void AddVectorToPoint(const float (&vec)[3], const float (&point)[3], float vectorScaleFactor, float (&result)[3]);

//vec1 X vec 2
void VectorCross(const float (&vec1)[3], const float (&vec2)[3], float (&cross)[3]);
void VectorCross(const float *vec1, const float *vec2, float (&cross)[3]);
float VectorDot(const float (&vec1)[3], const float (&vec2)[3]);
float VectorDot(const float *vec1, const float *vec2);
//from glu matrix
bool gluInvertMatrix(const double (&m)[16], double (&invOut)[16]);

#endif /* SmfCommon_hpp */
