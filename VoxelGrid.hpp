//
//  VoxelGrid.hpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-24.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef VoxelGrid_hpp
#define VoxelGrid_hpp

#include <stdio.h>
#include "SmfCommon.hpp"
#include "FaceVoxel.hpp"
#include "FaceMap.hpp"

class VoxelGrid
{
public:
    VoxelGrid(const float (&maxDimensions)[COORDINATESIZE], const float (&minDimensions)[COORDINATESIZE], int voxelDimension);
    ~VoxelGrid();
    void InsertFaces(FaceMap &faceList);
    void GetVoxelsAlongRay(const Point &origin, const Vector3 &ray, std::vector<FaceVoxel> &voxelList);
    FaceVoxel ***GetGrid();
    int GetGridDimension();
private:
    FaceVoxel ***grid;
    float maxDims[COORDINATESIZE];
    float minDims[COORDINATESIZE];
    float voxelDimensions[COORDINATESIZE];
    int gridDimension;
};
#endif /* VoxelGrid_hpp */
