//
//  WingedEdge.hpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef WingedEdge_hpp
#define WingedEdge_hpp

#include <stdio.h>

#include "SmfCommon.hpp"

class Vertex;
class Face;

class WingedEdge
{
public:
    WingedEdge();
    //~WingedEdge();
    void FlipEdge();
    
    bool CheckEdgeIndexes(int vstart, int vend);
    
    bool CheckEdgeForVertex(Vertex *vertex);
    
    float CalculateCost(float (&coordinates)[COORDINATESIZE]);
    
    int GetStartIndex();
    
    void SetStartIndex(int vstart);
    
    int GetEndIndex();
    
    void SetEndIndex(int vend);
    
    void RecalculateIndexes();
    
    Vertex *GetStartVertex();
    void SetStartVertex(Vertex *vstart);
    
    Vertex *GetEndVertex();
    void SetEndVertex(Vertex *vend);
    
    Face *GetLeftFace();
    void SetLeft(Face *fleft);
    
    Face *GetRightFace();
    void SetRight(Face *fright);
    
    WingedEdge *GetLeftPrev();
    void SetLeftPrev(WingedEdge *eleft_prev);
    
    WingedEdge *GetLeftNext();
    void SetLeftNext(WingedEdge *eleft_next);
    
    WingedEdge *GetRightPrev();
    
    void SetRightPrev(WingedEdge *eright_prev);
    
    WingedEdge *GetRightNext();
    void SetRightNext(WingedEdge *eright_next);
    
    WingedEdge *GetNext();
    void SetNext(WingedEdge *next);
    
    WingedEdge *GetPrev();
    void SetPrev(WingedEdge *prev);
    
    
private:
    int vertexIndexes[2];
    Vertex *start, *end;
    Face *left, *right;
    WingedEdge *left_prev , *left_next;
    WingedEdge *right_prev, *right_next;
    WingedEdge *hashNext, *hashPrev;
};

#endif /* WingedEdge_hpp */
