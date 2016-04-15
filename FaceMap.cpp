//
//  FaceMap.cpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "FaceMap.hpp"

FaceMap::FaceMap()
{
    count = 0;
    dirty = false;
}

FaceMap::FaceMap(int size)
{
    count = 0;
    dirty = false;
    faceList.reserve(size);
}

void FaceMap::Reserve(int size)
{
    faceList.reserve(size);
}
//To be called after decimation or subdivision
void FaceMap::UpdateFaceIndexes()
{
    int updateCount = 0;
    for (std::vector<Face>::iterator it = faceList.begin(); it != faceList.end(); ++it)
    {
        it->SetIndex(updateCount);
        updateCount++;
    }
    dirty = false;
}

std::vector<Face>::iterator FaceMap::GetBeginIterator()
{
    return faceList.begin();
}

std::vector<Face>::iterator FaceMap::GetEndIterator()
{
    return faceList.end();
}

Face *FaceMap::GetFace(int index)
{
    /*if(index > count || dirty == true)
    {
        return NULL;
    }
    int faceCount = 0;
    for (std::vector<Face>::iterator it = faceList.begin(); it != faceList.end(); ++it)
    {
        if (faceCount == index)
        {
            return &(*it);
        }
        faceCount++;
    }
    return NULL;*/
    if(index > count || dirty == true)
    {
        return NULL;
    }
    return &faceList[index];
}

Face *FaceMap::GetFace(Face *face)
{
    if(face == NULL)
    {
        return NULL;
    }
    std::vector<Face>::iterator it = find (faceList.begin(), faceList.end(), *face);
    if(it != faceList.end())
    {
        return &(*it);
    }
    return NULL;
}

Face *FaceMap::GetLastFace()
{
    return &faceList.back();
}


//to be called after import
void FaceMap::CalculateNormals()
{
    for (std::vector<Face>::iterator it = faceList.begin(); it != faceList.end(); ++it)
    {
        it->CalculateNormal();
    }
}


void FaceMap::AddFace(Face face)
{
    face.SetIndex(count);
    faceList.push_back(face);
    count++;
}

void FaceMap::AddFace()
{
    Face newFace;
    newFace.SetIndex(count);
    faceList.push_back(newFace);
    count++;
}

bool FaceMap::EraseFace(Face *face)
{
    if(face == NULL)
    {
        return false;
    }
    std::vector<Face>::iterator it = std::find(faceList.begin(), faceList.end(), *face);
    if(it != faceList.end())
    {
        faceList.erase(it);
        count--;
        dirty = true;
        return true;
    }
    return false;
}

bool FaceMap::EraseFace(int index)
{
    if(index > count )
    {
        return false;
    }
    int faceCount = 0;
    for (std::vector<Face>::iterator it = faceList.begin(); it != faceList.end(); ++it)
    {
        if (faceCount == index)
        {
            faceList.erase(it);
            count--;
            dirty = true;
            return true;
        }
        faceCount++;
    }
    
    return false;
}

int FaceMap::GetCount()
{
    return count;
}

void FaceMap::Reset()
{
    faceList.clear();
    count = 0;
    dirty = false;
}
