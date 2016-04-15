//
//  VoxelGrid.cpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-24.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "VoxelGrid.hpp"
#include <cmath>
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
                float voxelCoordinates[COORDINATESIZE] = {minDims[0] + voxelDimensions[0]*static_cast<float>(i),minDims[1] + voxelDimensions[1]*static_cast<float>(j), minDims[2] + voxelDimensions[2]*static_cast<float>(k)};
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
    
    

    for ( std::vector<Face>::iterator it = faceList.GetBeginIterator(); it != faceList.GetEndIterator(); ++it )
    {
        Face * currFace = &(*it);
        if(currFace != NULL)
        {
            float vertices[3][3];
            currFace->GetVertices(vertices);
            float maxFaceDim[3] = {vertices[0][0], vertices[0][1], vertices[0][2]};
            float minFaceDim[3] = {vertices[0][0], vertices[0][1], vertices[0][2]};
            //get bounding volume
            for (int i =0; i <3; i++)
            {
                for (int j =0; j <COORDINATESIZE; j++)
                {
                    if(vertices[i][j] > maxFaceDim[j])
                    {
                        maxFaceDim[j] = vertices[i][j];
                    }
                    if(vertices[i][j] < minFaceDim[j])
                    {
                        minFaceDim[j] = vertices[i][j];
                    }
                }
            }
            int maxIndexes[3];
            int minIndexes[3];
            for (int i=0; i<COORDINATESIZE; i++)
            {
                maxIndexes[i] = static_cast<int>(ceilf((maxFaceDim[i]-minDims[i])/voxelDimensions[i]));
                if(maxIndexes[i]>gridDimension-1)
                {
                    maxIndexes[i] = gridDimension-1;
                }
                else if(maxIndexes[i]<0)
                {
                    maxIndexes[i] = 0;
                }
                minIndexes[i] = static_cast<int>(floorf((minFaceDim[i]-minDims[i])/voxelDimensions[i]));
                if(minIndexes[i]>gridDimension-1)
                {
                    minIndexes[i] = gridDimension-1;
                }
                else if(minIndexes[i]<0)
                {
                    minIndexes[i] = 0;
                }
            }
            
            for(int i=minIndexes[0]; i<maxIndexes[0]+1; i++)
            {
                for(int j=minIndexes[1]; j<maxIndexes[1]+1; j++)
                {
                    for(int k=minIndexes[2]; k<maxIndexes[2]+1; k++)
                    {
                        grid[i][j][k].AddFace(currFace);
                    }
                }
            }
            /*float faceCoordinates[COORDINATESIZE] = {0.0,0.0,0.0};
            currFace->GetCentre(faceCoordinates);
            int indexes[COORDINATESIZE] = {0,0,0};
            for (int i = 0; i<COORDINATESIZE; i++)
            {
                
                indexes[i] = static_cast<int>((faceCoordinates[i]-minDims[i])/voxelDimensions[i] + 0.5);
                if(indexes[i]>gridDimension-1)
                {
                    indexes[i] = gridDimension-1;
                }
            }
            
            grid[indexes[0]][indexes[1]][indexes[2]].AddFace(currFace);*/
        
        }
    }

}

void VoxelGrid::GetVoxelsAlongRay(const Point &origin, const Vector3 &ray, std::vector<FaceVoxel> &voxelList)
{
    float voxelDimensions[COORDINATESIZE] = {0.0,0.0,0.0};
    CalculateVoxelDimensions(voxelDimensions, maxDims, minDims, gridDimension);
    
    //starting grid indexes
    
    int indexes[COORDINATESIZE] = {0,0,0};
    for (int i = 0; i<COORDINATESIZE; i++)
    {
        indexes[i] = static_cast<int>((origin[i]-minDims[i])/voxelDimensions[i] + 0.5);
        if(indexes[i]>gridDimension-1)
        {
            indexes[i] = gridDimension-1;
        }
    }
    
    int steps[3];
    for(int i = 0; i <3 ; i++)
    {
        if(ray[i]>0)
        {
            steps[i] = 1;
        }
        else if(ray[i]<0)
        {
            steps[i] = -1;
        }
        else
        {
            steps[i] = 0;
        }
    }
    
    float tMax[3];
    for(int i =0; i<3; i++)
    {
        float nextCoordinate = static_cast<float>(indexes[i] + steps[i])*voxelDimensions[i] + minDims[i];
        float difference = nextCoordinate - origin[i];
        tMax[i] = difference/ray[i];
    }
    
    float tDelta[3];
    for(int i =0; i<3; i++)
    {
        tDelta[i] = fabs(voxelDimensions[i]/ray[i]);
    }
    
    while(1)
    {
        if(tMax[0] < tMax[1])
        {
            if(tMax[0] < tMax[2])
            {
                indexes[0]= indexes[0] + steps[0];
                if(indexes[0] == gridDimension || indexes[0]<0)
                    return; /* outside grid */
                tMax[0]= tMax[0] + tDelta[0];
            }
            else
            {
                indexes[2]= indexes[2] + steps[2];
                if(indexes[2] == gridDimension || indexes[2]<0)
                    return;
                tMax[2]= tMax[2] + tDelta[2];
            }
        }
        else
        {
            if(tMax[1] < tMax[2])
            {
                indexes[1]= indexes[1] + steps[1];
                if(indexes[1] == gridDimension || indexes[1]<0)
                    return;
                tMax[1]= tMax[1] + tDelta[1];
            }
            else
            {
                indexes[2]= indexes[2] + steps[2];
                if(indexes[2] == gridDimension || indexes[2]<0)
                    return;
                tMax[2]= tMax[2] + tDelta[2];
                
            }
        }
        voxelList.push_back(grid[indexes[0]][indexes[1]][indexes[2]]);
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