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
#include <vector>
#include "Face.hpp"
class FaceCluster
{
public:
    FaceCluster();
    FaceCluster(int size);
    ~FaceCluster();
    
    void Reserve(int size);
    
    std::vector<Face*>::iterator GetBeginIterator();
    
    std::vector<Face*>::iterator GetEndIterator();
    
    void AddFace(Face* face);
    Face *GetFace(int index);
    void Clear();
    int GetCount();
    
private:
    std::vector<Face*> faceList;
    int count;
};
#endif /* FaceCluster_hpp */
