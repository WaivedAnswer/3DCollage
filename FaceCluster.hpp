//
//  FaceCluster.hpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-17.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef FaceCluster_hpp
#define FaceCluster_hpp

#include <stdio.h>
#include <list>
#include "Face.hpp"
class FaceCluster
{
    FaceCluster();
    ~FaceCluster();
    void AddFace(Face* face);
    Face *GetFace(int index);
    
private:
    std::list<Face*> faceList;
    int count;
};
#endif /* FaceCluster_hpp */
