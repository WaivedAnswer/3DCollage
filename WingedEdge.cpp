//
//  WingedEdge.cpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "WingedEdge.hpp"
#include "Vertex.hpp"
#include "Face.hpp"

WingedEdge::WingedEdge()
{
    vertexIndexes[0] = -1;
    vertexIndexes[1] = -1;
    
    start = NULL;
    end = NULL;
    
    left = NULL;
    right = NULL;
    
    left_prev = NULL;
    left_next = NULL;
    
    right_prev = NULL;
    right_next = NULL;
    
    hashNext = NULL;
    hashPrev = NULL;
    
}

/*WingedEdge::~WingedEdge()
{
    if(prev)
}*/

void WingedEdge::FlipEdge()
{
    if(start == NULL || end == NULL)
    {
        return;
    }
    int tempIndex = vertexIndexes[0];
    vertexIndexes[0] = vertexIndexes[1];
    vertexIndexes[1] = tempIndex;
    
    Vertex *tempVertex = start;
    start = end;
    end = tempVertex;
    
    Face *tempFace = left;
    left = right;
    right = tempFace;
    
    WingedEdge *tempPrev = left_prev;
    WingedEdge *tempNext = left_next;
    left_prev = right_prev;
    left_next = right_next;
    
    right_prev = tempPrev;
    right_next = tempNext;
    
}


bool WingedEdge::CheckEdgeIndexes(int vstart, int vend)
{
    return ((vstart == vertexIndexes[0] &&
             vend == vertexIndexes[1]) ||
            (vstart == vertexIndexes[1] &&
             vend == vertexIndexes[0]));
    
}

bool WingedEdge::CheckEdgeForVertex(Vertex *vertex)
{
    if(start == NULL || end == NULL)
    {
        return false;
    }

    return(start == vertex || end == vertex);
}

void GetBestCoordinatesFromInverseQuadric(float (&coordinates)[3], double (&inverse)[16])
{
    coordinates[0] = inverse[3];
    coordinates[1] = inverse[7];
    coordinates[2] = inverse[11];
}

float WingedEdge::CalculateCost(float (&coordinates)[COORDINATESIZE])
{
    if(start == NULL || end == NULL)
    {
        return 0.0;
    }
    //used as a test
    Vertex tempVertex;

    tempVertex.AddQuadric(*start);
    tempVertex.AddQuadric(*end);
    
    double inverse[16];
    if(tempVertex.InvertQuadric(inverse))
    {
        GetBestCoordinatesFromInverseQuadric(coordinates, inverse);
        tempVertex.SetCoordinates(coordinates);

    }
    else
    {
        tempVertex.SetCoordinates(start);
        tempVertex.GetCoordinates(coordinates);
    }
    
    return tempVertex.GetQuadricCost();
}

int WingedEdge::GetStartIndex()
{
    return vertexIndexes[0];
}

void WingedEdge::SetStartIndex(int vstart)
{
    if(vstart >= 0)
    {
        vertexIndexes[0] = vstart;
    }
}

int WingedEdge::GetEndIndex()
{
    return vertexIndexes[1];
}

void WingedEdge::SetEndIndex(int vend)
{
    if(vend >= 0)
    {
        vertexIndexes[1] = vend;
    }
}

void WingedEdge::RecalculateIndexes()
{
    if(start != NULL)
    {
        vertexIndexes[0] = start->GetIndex();
    }
    else
    {
        return;
    }
    if(end != NULL)
    {
        vertexIndexes[1] = end->GetIndex();
    }
    else{
        return;
    }
}


Vertex *WingedEdge::GetStartVertex()
{
    return start;
}
void WingedEdge::SetStartVertex(Vertex *vstart)
{
    if(vstart != NULL)
    {
        start = vstart;
    }
    else
    {
        return;
    }
}

Vertex *WingedEdge::GetEndVertex()
{
    return end;
}
void WingedEdge::SetEndVertex(Vertex *vend)
{
    if(vend != NULL)
    {
        end = vend;
    }
    else
    {
        return;
    }
}

Face *WingedEdge::GetLeftFace()
{
    return left;
}
void WingedEdge::SetLeft(Face *fleft)
{
    if(fleft != NULL)
    {
        left = fleft;
    }
}

Face *WingedEdge::GetRightFace()
{
    return right;
}
void WingedEdge::SetRight(Face *fright)
{
    if(fright != NULL)
    {
        right = fright;
    }
}

WingedEdge *WingedEdge::GetLeftPrev()
{
    return left_prev;
}
void WingedEdge::SetLeftPrev(WingedEdge *eleft_prev)
{
    if(eleft_prev != NULL)
    {
        left_prev = eleft_prev;
    }
}

WingedEdge *WingedEdge::GetLeftNext()
{
    return left_next;
}
void WingedEdge::SetLeftNext(WingedEdge *eleft_next)
{
    if(eleft_next != NULL)
    {
        left_next = eleft_next;
    }
}

WingedEdge *WingedEdge::GetRightPrev()
{
    return right_prev;
}

void WingedEdge::SetRightPrev(WingedEdge *eright_prev)
{
    if(eright_prev != NULL)
    {
        right_prev = eright_prev;
    }
}

WingedEdge *WingedEdge::GetRightNext()
{
    return right_next;
}
void WingedEdge::SetRightNext(WingedEdge *eright_next)
{
    if(eright_next != NULL)
    {
        right_next = eright_next;
    }
}

WingedEdge *WingedEdge::GetNext()
{
    return hashNext;
}
void WingedEdge::SetNext(WingedEdge *next)
{
    if(next != NULL)
    {
        hashNext = next;
    }
}

WingedEdge *WingedEdge::GetPrev()
{
    return hashPrev;
}
void WingedEdge::SetPrev(WingedEdge *prev)
{
    if(prev != NULL)
    {
        hashPrev = prev;
    }
}

