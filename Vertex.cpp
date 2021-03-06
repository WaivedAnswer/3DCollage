//
//  Vertex.cpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "Vertex.hpp"
#include "Face.hpp"

Vertex::Vertex()
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        coordinates[i] = 0;
    }
    
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        normal[i] = 0;
    }
    
    for(int i = 0; i<QUADRICSIZE; i++)
    {
        quadrics[i] = 0;
    }
    index = -1;
};

bool Vertex::operator==(const Vertex& rhs)
{
    if ((this->index == rhs.index) &&
        (this->coordinates == rhs.coordinates) &&
        (this->normal == rhs.normal) &&
        (this->quadrics == rhs.quadrics) &&
        (this->edge == rhs.edge))
    {
        return true;
    }
    
    return false;
}

void Vertex::SetIndex(int vindex)
{
    index = vindex;
}

int Vertex::GetIndex()
{
    return index;
}

void Vertex::SetCoordinates(float (&newCoordinates)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        coordinates[i] = newCoordinates[i];
    }
}

void Vertex::SetCoordinates(Vertex *vertex)
{
    if(vertex == NULL)
    {
        return;
    }
    float vertexCoord[COORDINATESIZE];
    vertex->GetCoordinates(vertexCoord);
    
    for (int i = 0; i<COORDINATESIZE; i++)
    {
        coordinates[i] = coordinates[i] + vertexCoord[i];
    }
    
}
void Vertex::GetCoordinates(float (&vCoordinates)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        vCoordinates[i] = coordinates[i];
    }
}

float *Vertex::GetCoordinates()
{
    return coordinates;
}

void Vertex::CalculateVertexNormal()
{
    WingedEdge *e0 = edge;
    WingedEdge *vedge = e0;
    float faceNormal[COORDINATESIZE];
    float surfaceNormalTotal[COORDINATESIZE] = {0,0,0};
    int surfaceCount = 0;
    do
    {
        //TODO refix this
        if(vedge->GetEndVertex() == this)
        {
            vedge->GetRightFace()->GetNormal(faceNormal);
            
            surfaceNormalTotal[0]+=faceNormal[0];
            surfaceNormalTotal[1]+=faceNormal[1];
            surfaceNormalTotal[2]+=faceNormal[2];
            surfaceCount++;
            vedge = vedge->GetRightNext();
        }
        else
        {
            vedge->GetLeftFace()->GetNormal(faceNormal);

            surfaceNormalTotal[0]+=faceNormal[0];
            surfaceNormalTotal[1]+=faceNormal[1];
            surfaceNormalTotal[2]+=faceNormal[2];
            surfaceCount++;
            vedge = vedge->GetLeftPrev();
        }
    } while (edge != e0);
    
    normal[0] = surfaceNormalTotal[0]/surfaceCount;
    normal[1] = surfaceNormalTotal[1]/surfaceCount;
    normal[2] = surfaceNormalTotal[2]/surfaceCount;
    
    NormalizeVector(normal);
    //normalize vector
}

void Vertex::GetNormal(float (&vNormal)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        vNormal[i] = normal[i];
    }
}

WingedEdge *Vertex::GetEdge()
{
    return edge;
}
void Vertex::SetEdge(WingedEdge *vEdge)
{
    edge = vEdge;
}

bool Vertex::CheckVertexForEdge(WingedEdge *vEdge)
{
    return vEdge == edge;
}

void Vertex::InitQuadric()
{
    WingedEdge *e0 = edge;
    WingedEdge *vedge = e0;
    float faceQuadric[QUADRICSIZE] = {0,0,0};
    
    do
    {
        //TODO refix this
        if(vedge->GetEndVertex() == this)
        {
            vedge->GetRightFace()->GetQuadric(this, faceQuadric);
            AddQuadric(faceQuadric);
            vedge = vedge->GetRightNext();
        }
        else
        {
            vedge->GetLeftFace()->GetQuadric(this, faceQuadric);
            AddQuadric(faceQuadric);
            vedge = vedge->GetLeftPrev();
        }
    } while (edge != e0);


}


void Vertex::GetQuadric(float (&vQuadric)[QUADRICSIZE])
{
    for(int i = 0; i<QUADRICSIZE; i++)
    {
        vQuadric[i] = quadrics[i];
    }
}

float Vertex::GetQuadricCost()
{
    return
      (coordinates[0]*(quadrics[0]*coordinates[0] + quadrics[1]*coordinates[1] + quadrics[2]*coordinates[2] +quadrics[3]))
    + (coordinates[1]*(quadrics[1]*coordinates[0] + quadrics[4]*coordinates[1] + quadrics[5]*coordinates[2] +quadrics[6]))
    + (coordinates[2]*(quadrics[2]*coordinates[0] + quadrics[5]*coordinates[1] + quadrics[7]*coordinates[2] +quadrics[8]))
    + (               (quadrics[3]*coordinates[0] + quadrics[6]*coordinates[1] + quadrics[8]*coordinates[2] +quadrics[9]));
}


bool Vertex::InvertQuadric(double (&vInvQuadric)[16])
{
    double quadricMatrix[16];
    
    quadricMatrix[0] = quadrics[0];
    quadricMatrix[1] = quadrics[1];
    quadricMatrix[2] = quadrics[2];
    quadricMatrix[3] = quadrics[3];
    quadricMatrix[4] = quadrics[1];
    quadricMatrix[5] = quadrics[4];
    quadricMatrix[6] = quadrics[5];
    quadricMatrix[7] = quadrics[6];
    quadricMatrix[8] = quadrics[2];
    quadricMatrix[9] = quadrics[5];
    quadricMatrix[10] = quadrics[7];
    quadricMatrix[11] = quadrics[8];
    quadricMatrix[12] = 0;
    quadricMatrix[13] = 0;
    quadricMatrix[14] = 0;
    quadricMatrix[15] = 1;
    
    if(!gluInvertMatrix(quadricMatrix, vInvQuadric))
    {
        return false;
    }
    return true;
    
}


void Vertex::AddQuadric(Vertex &vertex)
{
    float vertexQuad[QUADRICSIZE];
    vertex.GetQuadric(vertexQuad);
    
    for (int i = 0; i<QUADRICSIZE; i++)
    {
        quadrics[i] = quadrics[i] + vertexQuad[i];
    }
}

void Vertex::AddQuadric(float (&quadric)[QUADRICSIZE])
{
    for (int i = 0; i<QUADRICSIZE; i++)
    {
        quadrics[i] = quadrics[i] + quadric[i];
    }
}
                       
                       
