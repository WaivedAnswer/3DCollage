//
//  Face.cpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "Face.hpp"

Face::Face()
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        normal[i] = 0;
    }
    
    offset = 0.0;
    
    index = -1;
};

bool Face::operator==(const Face& rhs)
{
    if ((this->index == rhs.index) &&
        (this->normal == rhs.normal) &&
        (this->offset == rhs.offset) &&
        (this->edge == rhs.edge))
    {
        return true;
    }
    
    return false;
}


void Face::SetIndex(int findex)
{
    index = findex;
}

int Face::GetIndex()
{
    return index;
}
//todo maybe make it const
WingedEdge *Face::GetEdge()
{
    return edge;
}

void Face::SetEdge(WingedEdge *fEdge)
{
    edge = fEdge;
}


void Face::CalculateNormal()
{
    WingedEdge *originalFaceEdge = edge;
    //W_edge *firstIndexEdge = &EdgeList[0];
    WingedEdge *currFaceEdge = originalFaceEdge;
    float vec1[3] = {0,0,0};
    float vec2[3] = {0,0,0};
    //only need 3 vertexes for this
    Vertex * vertexes[3] = {NULL, NULL, NULL};
    
    for( int j=0; j<3; j++)
    {
        if(currFaceEdge->GetRightFace() == this)
        {
            vertexes[j] = currFaceEdge->GetStartVertex();
            currFaceEdge = currFaceEdge->GetRightPrev();
        }
        else
        {
            vertexes[j] = currFaceEdge->GetEndVertex();
            currFaceEdge = currFaceEdge->GetLeftPrev();
        }
    }
    
    CreateVector(vertexes[0]->GetCoordinates(), vertexes[1]->GetCoordinates(), vec1);
    CreateVector(vertexes[2]->GetCoordinates(), vertexes[1]->GetCoordinates(), vec2);
    
    VectorCross(vec2, vec1, normal);
    
    NormalizeVector(normal);
    
}


void Face::GetCentre(float (&fCentre)[COORDINATESIZE])
{
    
    WingedEdge *originalFaceEdge = edge;
    //W_edge *firstIndexEdge = &EdgeList[0];
    WingedEdge *currFaceEdge = originalFaceEdge;
    
    //could do while loop instead
    for( int j=0; j<MESHITEMSIZE; j++)
    {
        
        if(currFaceEdge->GetRightFace() == this)
        {
            //as the edges have specific orderings if we use right prev must use starting index
            //std::cout << " " << currFaceEdge->vertexIndexes[0] + 1;
            
            float coordinates[COORDINATESIZE] = {0,0,0};
            currFaceEdge->GetStartVertex()->GetCoordinates(coordinates);
            
            AddCoordinates(fCentre, coordinates, fCentre);
            
            currFaceEdge = currFaceEdge->GetRightPrev();
        }
        else
        {
            //if using left prev need to use end vertex index
            //std::cout << " " << currFaceEdge->vertexIndexes[1] + 1;
            
            float coordinates[COORDINATESIZE] = {0,0,0};
            currFaceEdge->GetEndVertex()->GetCoordinates(coordinates);
            
            AddCoordinates(fCentre, coordinates, fCentre);
            currFaceEdge = currFaceEdge->GetLeftPrev();
        }
    }
    
    ScaleCoordinates(fCentre, MESHITEMSIZE);
    

}

void Face::GetVertices(float (&vertices)[MESHITEMSIZE][COORDINATESIZE])
{
    WingedEdge *originalFaceEdge = edge;
    //W_edge *firstIndexEdge = &EdgeList[0];
    WingedEdge *currFaceEdge = originalFaceEdge;
    
    //could do while loop instead
    for( int j=0; j<MESHITEMSIZE; j++)
    {
        
        if(currFaceEdge->GetRightFace() == this)
        {
            //as the edges have specific orderings if we use right prev must use starting index
            currFaceEdge->GetStartVertex()->GetCoordinates(vertices[j]);
            
            currFaceEdge = currFaceEdge->GetRightPrev();
        }
        else
        {
            //if using left prev need to use end vertex index
            currFaceEdge->GetEndVertex()->GetCoordinates(vertices[j]);

            currFaceEdge = currFaceEdge->GetLeftPrev();
        }
    }

}

void Face::GetNormal(float (&vNormal)[COORDINATESIZE])
{
    for(int i = 0; i<COORDINATESIZE; i++)
    {
        vNormal[i] = normal[i];
    }
}


void Face::GetQuadric(Vertex *vertex, float (&vQuadric)[QUADRICSIZE])
{
    float vertexCoord[3] = {0,0,0};
    float d = 0;
    
    vertex->GetCoordinates(vertexCoord);
    for (int i = 0; i < COORDINATESIZE; i++)
    {
        d += -(normal[i]*vertexCoord[i]);
    }
    
    vQuadric[0] = normal[0]*normal[0];
    vQuadric[1] = normal[0]*normal[1];
    vQuadric[2] = normal[0]*normal[2];
    vQuadric[3] = normal[0]*d;
    vQuadric[4] = normal[1]*normal[1];
    vQuadric[5] = normal[1]*normal[2];
    vQuadric[6] = normal[1]*d;
    vQuadric[7] = normal[2]*normal[2];
    vQuadric[8] = normal[2]*d;
    vQuadric[9] = d*d;
}


