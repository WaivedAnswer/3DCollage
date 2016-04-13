//
//  FaceVoxel.hpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-24.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef FaceVoxel_hpp
#define FaceVoxel_hpp

#include <stdio.h>
#include <vector>
#include "Face.hpp"
#include "SmfCommon.hpp"

class FaceVoxel
{
public:
    FaceVoxel(float width, float height, float depth);
    FaceVoxel(const float (&vdimensions)[COORDINATESIZE]);
    FaceVoxel();
    void SetDimensions(const float (&vdimensions)[COORDINATESIZE]);
    void GetDimensions(float (&vdimensions)[COORDINATESIZE]);
    void SetCoordinates(const float (&vcoordinates)[COORDINATESIZE]);
    void GetCoordinates(float (&vcoordinates)[COORDINATESIZE]);
    //make this quicker
    std::vector<Face*> GetFaceList();
    //fix this later
    bool CheckIfInVoxel(Face *face);
    void AddFace(Face *face);
    
private:
    std::vector<Face*> faceList;
    float coordinates[COORDINATESIZE];
    float dimensions[COORDINATESIZE];
};
#endif /* FaceVoxel_hpp */
