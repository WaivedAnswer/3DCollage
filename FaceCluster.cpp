//
//  FaceCluster.cpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-17.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "FaceCluster.hpp"

FaceCluster::FaceCluster()
{
    count = 0;
}

FaceCluster::FaceCluster(int size)
{
    count = 0;
    faceList.reserve(size);
}

void FaceCluster::Reserve(int size)
{
    faceList.reserve(size);
}

FaceCluster::~FaceCluster()
{
    
}

std::vector<Face*>::iterator FaceCluster::GetBeginIterator()
{
    return faceList.begin();
}

std::vector<Face*>::iterator FaceCluster::GetEndIterator()
{
    return faceList.end();
}

void FaceCluster::AddFace(Face* face)
{
    faceList.push_back(face);
    count++;
}
Face *FaceCluster::GetFace(int index)
{
    /*if(index > count)
    {
        return NULL;
    }
    int faceCount = 0;
    for (std::vector<Face*>::iterator it = faceList.begin(); it != faceList.end(); ++it)
    {
        if (faceCount == index)
        {
            return *it;
        }
        faceCount++;
    }
    return NULL;*/
    if(index > count)
    {
        return NULL;
    }
    return faceList[index];
}

void FaceCluster::Clear() {
    faceList.clear();
    count = 0;
}

int FaceCluster::GetCount() {
    return count;
}
