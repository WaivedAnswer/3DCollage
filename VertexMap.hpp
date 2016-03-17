//
//  VertexMap.hpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef VertexMap_hpp
#define VertexMap_hpp

#include <stdio.h>
#include <list>
#include "Vertex.hpp"
#include "Face.hpp"
#include "WingedEdge.hpp"
#include "SmfCommon.hpp"

class VertexMap
{
public:
    VertexMap();
    
    
    //To be called after decimation or subdivision
    void UpdateVertexIndexes();
    
    std::list<Vertex>::iterator GetBeginIterator();
    
    std::list<Vertex>::iterator GetEndIterator();
    
    Vertex *GetVertex(int index);
    
    //to be called after import
    void InitQuadrics();
    //to be called after import
    //have to call face calculate before vertex calculate
    void CalculateNormals();
    
    void AddVertex(Vertex vertex);
    
    bool EraseVertex(Vertex *vertex);
    
    bool EraseVertex(int index);
    
    int GetCount();
    
    void Reset();
    
    
private:
    bool dirty;
    std::list<Vertex> vertexList;
    int count;
};

#endif /* VertexMap_hpp */