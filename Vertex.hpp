//
//  Vertex.hpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>
#include "Vertex.hpp"
#include "Face.hpp"
#include "WingedEdge.hpp"
#include "SmfCommon.hpp"


class Vertex
{
public:
    Vertex();
    
    bool operator==(const Vertex& rhs);
    
    void SetIndex(int vindex);
    int GetIndex();
    
    void SetCoordinates(float (&newCoordinates)[COORDINATESIZE]);
    void SetCoordinates(Vertex *vertex);
    void GetCoordinates(float (&vNormal)[COORDINATESIZE]);
    float *GetCoordinates();
    
    void CalculateVertexNormal();
    void GetNormal(float (&vNormal)[COORDINATESIZE]);
    
    WingedEdge *GetEdge();
    void SetEdge(WingedEdge *vEdge);
    bool CheckVertexForEdge(WingedEdge *vEdge);
    
    void InitQuadric();
    void GetQuadric(float (&vQuadric)[QUADRICSIZE]);
    float GetQuadricCost();
    bool InvertQuadric(double (&vInvQuadric)[16]);
    void AddQuadric(Vertex &vertex);
    void AddQuadric(float (&quadric)[QUADRICSIZE]);
    

    
private:
    int index;
    float coordinates[COORDINATESIZE]; // x, y, z
    float normal[COORDINATESIZE];
    WingedEdge *edge = NULL;

    //plane specified by p = [a b c d] ^T
    //stores upper triangle of error quadric matrix as symmetric
    //aa, ab, ac, ad, bb, bc, bd, cc, cd, dd
    //squared distance to plane is t^2 = v^T*Q*v
    float quadrics[QUADRICSIZE];
};//*VertexList;

#endif /* Vertex_hpp */
