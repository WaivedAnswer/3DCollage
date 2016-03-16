//
//  EdgeMap.hpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef EdgeMap_hpp
#define EdgeMap_hpp

#include <stdio.h>
#include "Vertex.hpp"
#include "VertexMap.hpp"
#include "Face.hpp"
#include "FaceMap.hpp"
#include "WingedEdge.hpp"
#include "SmfCommon.hpp"

class EdgeMap
{
public:
    EdgeMap(int size, VertexMap *vList, FaceMap *fList);
    
    ~EdgeMap();
    
    void AddEdge(WingedEdge *edge);
    //removes from hash linking but doesn't delete
    void RemoveEdge(WingedEdge *edge);
    
    void CollapseEdge(WingedEdge *edge, float (&newVertexCoordinates)[COORDINATESIZE]);
    
    int GetHashCount(int index);
    
    void ReHashEdge(WingedEdge *edge);
    void ReHashEdges();
    
    //void RecalculateEdgeIndexes();
    
    WingedEdge *GetRandomEdgeAtIndex(int index);
    
    //currently not fully weighted.. I could change it to be equally weighted but it shouldn't be a huge issue
    //max degree is likely no more than 7
    WingedEdge *GetRandomEdge();
    //need to delete pointer after use
    WingedEdge **GetkRandomEdges(int k);
    
    WingedEdge *GetEdge(int vertexStart, int vertexEnd);
    
    void MCDecimateEdge(int k);
    void MCDecimateNEdges(int N, int k);
    
private:
    WingedEdge **table;
    VertexMap *vertexList;
    FaceMap *faceList;
    int tableSize;
    int edgeCount;
};

#endif /* EdgeMap_hpp */
