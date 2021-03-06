//
//  FaceMap.hpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef FaceMap_hpp
#define FaceMap_hpp

#include <stdio.h>
#include <list>
#include <vector>
#include <algorithm>
#include "Vertex.hpp"
#include "Face.hpp"
#include "WingedEdge.hpp"
#include "SmfCommon.hpp"

class FaceMap
{
public:
    FaceMap();
    FaceMap(int size);
    //To be called after decimation or subdivision
    void UpdateFaceIndexes();
    
    void Reserve(int size);

    std::vector<Face>::iterator GetBeginIterator();
    
    std::vector<Face>::iterator GetEndIterator();
    
    Face *GetFace(int index);
    
    Face *GetFace(Face *face);
    
    Face *GetLastFace();
    
    //to be called after import
    //have to call face calculate before vertex calculate
    void CalculateNormals();
    
    void AddFace(Face face);
    
    void AddFace();
    
    bool EraseFace(Face *face);
    
    bool EraseFace(int index);
    
    int GetCount();
    
    void Reset();
    
    
private:
    bool dirty;
    std::vector<Face> faceList;
    int count;
};

#endif /* FaceMap_hpp */
