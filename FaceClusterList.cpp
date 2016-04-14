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
FaceClusterList::~FaceClusterList()
{
    
}
void FaceClusterList::AddCluster(FaceCluster* cluster)
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
    int clusterCount = 0;
    for (std::list<FaceCluster*>::iterator it = clusterList.begin(); it != clusterList.end(); it++)
    {
        if (clusterCount == index)
        {
            return *it;
        }
        clusterCount++;
    }
    return NULL;
}

void FaceClusterList::Clear() {
    for (std::list<FaceCluster*>::iterator it = clusterList.begin(); it != clusterList.end(); it++) {
        (*it)->Clear();
    }
}

int FaceClusterList::GetCount() {
    return count;
}
