//
//  FaceClusterList.cpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-17.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "FaceClusterList.hpp"

FaceClusterList::FaceClusterList()
{
    count = 0;
}

FaceClusterList::FaceClusterList(int size)
{
    count = 0;
    clusterList.reserve(size);
}

void FaceClusterList::Reserve(int size)
{
    clusterList.reserve(size);
}

FaceClusterList::~FaceClusterList()
{
    
}

std::vector<FaceCluster>::iterator FaceClusterList::GetBeginIterator()
{
    return clusterList.begin();
}

std::vector<FaceCluster>::iterator FaceClusterList::GetEndIterator()
{
    return clusterList.end();
}

void FaceClusterList::AddCluster(const FaceCluster &cluster)
{
    clusterList.push_back(cluster);
    count++;
}

FaceCluster *FaceClusterList::GetCluster(int index)
{
    if(index > count)
    {
        return NULL;
    }
    /*int clusterCount = 0;
    for (std::vector<FaceCluster>::iterator it = clusterList.begin(); it != clusterList.end(); ++it)
    {
        if (clusterCount == index)
        {
            return &(*it);
        }
        clusterCount++;
    }
    return NULL;*/
    return &clusterList[index];
}

void FaceClusterList::ClearClusters() {
    for (std::vector<FaceCluster>::iterator it = clusterList.begin(); it != clusterList.end(); ++it) {
        (*it).Clear();
    }
}

int FaceClusterList::GetCount() {
    return count;
}
