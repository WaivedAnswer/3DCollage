//
//  FaceVoxel.cpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-24.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "FaceVoxel.hpp"


FaceVoxel::FaceVoxel(float width, float height, float depth)
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        coordinates[i] = 0.0;
    }
    
    dimensions[0] = width;
    dimensions[1] = height;
    dimensions[2] = depth;
}

FaceVoxel::FaceVoxel(const float (&vdimensions)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        dimensions[i] = vdimensions[i];
    }
}

FaceVoxel::FaceVoxel()
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        coordinates[i] = 0.0;
    }
    
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        dimensions[i] = 0.0;
    }
}

void FaceVoxel::SetDimensions(const float (&vdimensions)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        dimensions[i] = vdimensions[i];
    }
}

void FaceVoxel::GetDimensions(float (&vdimensions)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
       vdimensions[i] = dimensions[i];
    }
}

void FaceVoxel::SetCoordinates(const float (&vcoordinates)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        coordinates[i] = vcoordinates[i];
    }
}

void FaceVoxel::GetCoordinates(float (&vcoordinates)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        vcoordinates[i] = coordinates[i];
    }
}

std::vector<Face*>::iterator FaceVoxel::GetBeginIterator()
{
    return faceList.begin();
}

std::vector<Face*>::iterator FaceVoxel::GetEndIterator()
{
    return faceList.end();
}

bool FaceVoxel::CheckIfInVoxel(Face *face)
{
    float fcoordinates[COORDINATESIZE] = {0.0,0.0,0.0};
    face->GetCentre(fcoordinates);
    
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        if(fcoordinates[i] < coordinates[i] ||
           fcoordinates[i] > coordinates[i] + dimensions[i])
        {
            return false;
        }
    }
    
    return true;
}

void FaceVoxel::AddFace(Face *face)
{
    if(face == NULL)
    {
        return;
    }
    faceList.push_back(face);
}