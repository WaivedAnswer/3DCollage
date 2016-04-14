//
//  FaceClusterList.hpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-17.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef FaceClusterList_hpp
#define FaceClusterList_hpp

#include <stdio.h>
#include <list>
#include "Face.hpp"
#include "FaceCluster.hpp"
class FaceClusterList
{
public:
    FaceClusterList();
    ~FaceClusterList();
    void AddCluster(FaceCluster* cluster);
    FaceCluster *GetCluster(int index);
    void Clear();
    int GetCount();
    
private:
    std::list<FaceCluster*> clusterList;
    int count;
};
#endif /* FaceClusterList_hpp */
