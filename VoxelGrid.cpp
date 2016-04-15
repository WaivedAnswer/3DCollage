//
//  VoxelGrid.cpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-24.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "VoxelGrid.hpp"
void CalculateVoxelDimensions(float (&voxelDimensions)[COORDINATESIZE], const float (&maxDimensions)[COORDINATESIZE], const float (&minDimensions)[COORDINATESIZE], int gridDimension)
{
    float negMinDimensions[COORDINATESIZE] = {0.0,0.0,0.0};
    //copies minDimensions to negMinDimensions so we can negate
    AddCoordinates(negMinDimensions, minDimensions, negMinDimensions);
    ScaleCoordinates(negMinDimensions, -1.0);
    //adds max dimensions and min dimensions(determines width height and depth
    AddCoordinates(maxDimensions, negMinDimensions, voxelDimensions);
    //divides dimensions by how many voxels are in each dimension
    ScaleCoordinates(voxelDimensions, gridDimension);
}

VoxelGrid::VoxelGrid(const float (&maxDimensions)[COORDINATESIZE], const float (&minDimensions)[COORDINATESIZE], int voxelGridDimension)
{
    float voxelDimensions[COORDINATESIZE] = {0.0,0.0,0.0};
    gridDimension = voxelGridDimension;
    //copies boundaries
    for (int i = 0; i<COORDINATESIZE; i++)
    {
        maxDims[i] = maxDimensions[i];
        minDims[i] = minDimensions[i];
    }
    
    CalculateVoxelDimensions(voxelDimensions, maxDims, minDims, gridDimension);
    
    grid = new FaceVoxel**[gridDimension];
    for(int i = 0; i<gridDimension; i++)
    {
        grid[i] = new FaceVoxel*[gridDimension];
        for(int j = 0; j<gridDimension; j++)
        {
            grid[i][j] = new FaceVoxel[gridDimension];
            for(int k = 0; k<gridDimension;k++)
            {
                float voxelCoordinates[COORDINATESIZE] = {minDims[0] + voxelDimensions[0]*i,minDims[1] + voxelDimensions[1]*j, minDims[2] + voxelDimensions[2]*k};
                grid[i][j][k].SetDimensions(voxelDimensions);
                grid[i][j][k].SetCoordinates(voxelCoordinates);
            }
        }
    }
}



VoxelGrid::~VoxelGrid()
{
    
    for(int i = 0; i<gridDimension; i++)
    {
        for(int j = 0; j<gridDimension; j++)
        {
            
            delete[] grid[i][j];
        }
        delete[] grid[i];
    }
    delete[] grid;
}

void VoxelGrid::InsertFaces(FaceMap &faceList)
{
    float voxelDimensions[COORDINATESIZE] = {0.0,0.0,0.0};
    
    CalculateVoxelDimensions(voxelDimensions, maxDims, minDims, gridDimension);

    for ( std::list<Face>::iterator it = faceList.GetBeginIterator(); it != faceList.GetEndIterator(); ++it )
    {
        Face * currFace = &(*it);
        if(currFace != NULL)
        {
            float faceCoordinates[COORDINATESIZE] = {0.0,0.0,0.0};
            currFace->GetCentre(faceCoordinates);
            int indexes[COORDINATESIZE] = {0,0,0};
            for (int i = 0; i<COORDINATESIZE; i++)
            {
                indexes[i] = (faceCoordinates[i]-minDims[i])/voxelDimensions[i];
            }
            grid[indexes[0]][indexes[1]][indexes[2]].AddFace(currFace);
        
        }
    }

}

FaceVoxel ***VoxelGrid::GetGrid()
{
    return grid;
}

int VoxelGrid::GetGridDimension()
{
    return gridDimension;
}