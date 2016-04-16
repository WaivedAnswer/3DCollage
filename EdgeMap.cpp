//
//  EdgeMap.cpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "EdgeMap.hpp"
#include <stdlib.h>   

EdgeMap::EdgeMap(int size, VertexMap *vList, FaceMap *fList)
{
    tableSize = size;
    table = new WingedEdge*[tableSize];
    for (int i =0; i<tableSize; i++)
    {
        table[i] = NULL;
    }
    edgeCount = 0;
    vertexList = vList;
    faceList = fList;
}

EdgeMap::~EdgeMap()
{
    if(table != NULL)
    {
        for (int i = 0; i < tableSize; i++)
        {
            WingedEdge * current = table[i];
            WingedEdge * next = NULL;
            while(current != NULL)
            {
                next = current->GetNext();
                delete current;
                current = next;
            }
        }
        delete table;
        table = NULL;
    }
}

void EdgeMap::RemoveEdge(WingedEdge *edge)
{
    //Fix edgeMap connections
    WingedEdge *prev = edge->GetPrev();
    WingedEdge *next = edge->GetNext();
    if( prev != NULL)
    {
        prev->SetNext(next);
    }
    else
    {
        table[edge->GetStartIndex()] = next;
    }
    
    if(next != NULL)
    {
        next->SetPrev(prev);
    }
    //else do nothing
}

void EdgeMap::AddEdge(WingedEdge *edge)
{
    if(edge == NULL)
    {
        return;
    }
    
    WingedEdge * current = table[edge->GetStartIndex()];
    if(current == NULL)
    {
        table[edge->GetStartIndex()] = edge;
    }
    else
    {
        while(current->GetNext() != NULL)
        {
            current = current->GetNext();
        }
        current->SetNext(edge);
        edge->SetPrev(current);
    }
    edgeCount++;
    
}
//have to be connected
Vertex * GetAdjoiningVertex(Vertex *edgeStart, Vertex *edgeEnd, WingedEdge *adjoiningEdge)
{
    if(adjoiningEdge->GetStartVertex() != edgeStart && adjoiningEdge->GetStartVertex() != edgeEnd)
    {
        return adjoiningEdge->GetStartVertex();
    }
    else
    {
        return adjoiningEdge->GetEndVertex();
    }
    
}

Face * GetAdjoiningFace(Face *face, WingedEdge * edge)
{
    if(edge->GetRightFace() != face)
    {
        return edge->GetRightFace();
    }
    else
    {
        return edge->GetLeftFace();
    }
    
}

//Tries to remap edge by 1 flips
//called after edges have been remapped to eachother so e0 will never equal edge again, get the edge on an edge e1 that exists first
bool RemapVertexsEdgeBy1Flip(Vertex *vertex, WingedEdge *edgeToAvoid)
{
    WingedEdge *e0 = vertex->GetEdge();
    WingedEdge *edge = e0;
    WingedEdge *e1 = NULL;
    
    if(edge!=edgeToAvoid)
    {
        return true;
    }
    
    if(edge->GetEndVertex() == vertex )
    {
        if(edge != edgeToAvoid)
        {
            vertex->SetEdge(edge);
            return true;
        }
        
        edge = edge->GetRightNext();
        e1 = edge;
    }
    else if(edge->GetStartVertex() == vertex)
    {
        edge = edge->GetLeftNext();
        e1 = edge;
    }

    
    int maxIterations = 10;
    do {
        /*maxIterations--;
        if(maxIterations < 0)
        {
            break;
        }*/
        if(edge->GetEndVertex() == vertex )
        {
            if(edge != edgeToAvoid)
            {
                vertex->SetEdge(edge);
                return true;
            }
            edge = edge->GetRightNext();
        }
        else if(edge->GetStartVertex() == vertex)
        {
            //checks if other vertex is mapped to this edge
            if(edge != edgeToAvoid && !edge->GetEndVertex()->CheckVertexForEdge(edge))
            {
                edge->FlipEdge();
                vertex->SetEdge(edge);
                return true;
            }
            edge = edge->GetLeftNext();
        }
        else if(edge != e0)
        {
            return false;
        }
    } while (edge != e1);
    
    return false;
}

//called after edges have been remapped to eachother so e0 will never equal edge again, get the edge on an edge e1 that exists first
bool RemapVertexsEdge(Vertex *vertex, WingedEdge *edgeToAvoid)
{
    WingedEdge *e0 = vertex->GetEdge();
    WingedEdge *edge = e0;
    WingedEdge *e1 = NULL;

    if(edge!=edgeToAvoid)
    {
        return true;
    }
    
    if(edge->GetEndVertex() == vertex )
    {
        if(edge != edgeToAvoid)
        {
            vertex->SetEdge(edge);
            return true;
        }
        
        edge = edge->GetRightNext();
        e1 = edge;
    }
    else if(edge->GetStartVertex() == vertex)
    {
        edge = edge->GetLeftNext();
        e1 = edge;
    }


    int maxIterations = 10;
    do {
        /*maxIterations--;
        if(maxIterations < 0)
        {
            break;
        }*/
        //only remap edges that start with vertex
        if(edge->GetEndVertex() == vertex )
        {
            if(edge != edgeToAvoid)
            {
                vertex->SetEdge(edge);
                return true;
            }

            edge = edge->GetRightNext();
        }
        else if(edge->GetStartVertex() == vertex)
        {
            edge = edge->GetLeftNext();
        }
        else if(edge != e0)
        {
            return false;
        }
        
    } while (edge != e1);

    //return false;
    return RemapVertexsEdgeBy1Flip(vertex, edgeToAvoid);
}

bool RemapFacesEdge(Face *face, WingedEdge *edgeToAvoid)
{
    WingedEdge *e0 = face->GetEdge();
    WingedEdge *edge = e0;
    
    if(edge!=edgeToAvoid)
    {
        return true;
    }
    int maxIterations = 10;
    do {
        /*maxIterations--;
        if(maxIterations < 0)
        {
            break;
        }*/
        if(edge->GetRightFace() == face)
        {
            if(edge != edgeToAvoid)
            {
                face->SetEdge(edge);
                return true;
            }
            edge = edge->GetRightNext();
        }
        else if(edge->GetLeftFace() == face)
        {
            if(edge != edgeToAvoid)
            {
                face->SetEdge(edge);
                return true;
            }
            edge = edge->GetLeftNext();
        }
        else if(edge != e0)
        {
            return false;
        }
    } while(edge != e0);
    return false;
    
}

bool RemapEdgesFace(WingedEdge *edge, Face *oldFace, Face *newFace )
{
    if(edge->GetRightFace() == oldFace)
    {
        edge->SetRight(newFace);
    }
    else if(edge->GetLeftFace() == oldFace)
    {
        edge->SetLeft(newFace);
    }
    else
    {
        return false;
    }
    return true;
}


bool RemapEdgesTouchingVertex(Vertex *oldVertex, Vertex *newVertex, WingedEdge *edgeToAvoid)
{
    if(oldVertex == NULL || newVertex == NULL)
    {
        return false;
    }
    WingedEdge *e0 = oldVertex->GetEdge();
    WingedEdge *edge = e0;
    int maxIterations = 10;
    do {
        /*maxIterations--;
        if(maxIterations < 0)
        {
            break;
        }*/
        if(edge->GetEndVertex() == oldVertex )
        {
            if(edge != edgeToAvoid)
            {
                edge->SetEndVertex(newVertex);
            }
            edge = edge->GetRightNext();
        }
        else if(edge->GetStartVertex() == oldVertex)
        {
            if(edge != edgeToAvoid)
            {
                edge->SetStartVertex(newVertex);
            }
            edge = edge->GetLeftNext();
        }
        else
        {
            return false;
        }
        /*else if(edge->CheckEdgeForVertex(newVertex)) //add other case this shouldnt ever happen
        {
            if(edge->GetEndVertex() == newVertex )
            {
                edge = edge->GetRightNext();
            }
            else if(edge->GetStartVertex() == newVertex)
            {
                edge = edge->GetLeftNext();
            }
        }*/
        
    } while (edge != e0);

    return true;
}

bool RemapEdgeFromEdge(WingedEdge *edge, WingedEdge *oldEdge, WingedEdge *newEdge)
{
    if(edge->GetRightNext() == oldEdge)
    {
        edge->SetRightNext(newEdge);
    }
    else if(edge->GetRightPrev() == oldEdge)
    {
        edge->SetRightPrev(newEdge);
    }
    else if(edge->GetLeftNext() == oldEdge)
    {
        edge->SetLeftNext(newEdge);
    }
    else if(edge->GetLeftPrev() == oldEdge)
    {
        edge->SetLeftPrev(newEdge);
    }
    else
    {
        return false;
    }
    return true;
}


void EdgeMap::CollapseEdge(WingedEdge *edge, float (&newVertexCoordinates)[COORDINATESIZE])
{
    if(edge == NULL)
    {
        return;
    }
    //TODO finish edge collapse items
    //Will draw
    //remap surroundings, delete 3 edges, delete 2 faces, delete 1 vertex
    WingedEdge *rightNext = edge->GetRightNext();
    WingedEdge *rightPrev = edge->GetRightPrev();
    WingedEdge *leftNext = edge->GetLeftNext();
    WingedEdge *leftPrev = edge->GetLeftPrev();
    
    Vertex *start = edge->GetStartVertex();
    Vertex *end = edge->GetEndVertex();
    Vertex *rightFaceVertex = GetAdjoiningVertex(start, end, rightNext);
    Vertex *leftFaceVertex = GetAdjoiningVertex(start, end, leftNext);
    
    Face *rightFace = edge->GetRightFace();
    Face *leftFace = edge->GetLeftFace();
    Face *rightNextAdjoiningFace = GetAdjoiningFace(rightFace, rightNext);
    //Face *rightPrevAdjoiningFace = GetAdjoiningFace(rightFace, rightPrev);
    //Face *leftNextAdjoiningFace = GetAdjoiningFace(rightFace, leftNext);
    Face *leftPrevAdjoiningFace = GetAdjoiningFace(rightFace, leftPrev);
    
    
    if(!RemapFacesEdge(rightNextAdjoiningFace, rightNext)) // check
    {
        return;
    }
    
    if(!RemapFacesEdge(leftPrevAdjoiningFace, leftPrev)) // check
    {
        return;
    }
    
    if(!RemapEdgesTouchingVertex(end, start, edge))
    {
        return;
    }
    //remap outer left
    if(leftPrev->GetLeftFace() == leftFace)
    {
        if(!RemapEdgeFromEdge(leftPrev->GetRightNext(), leftPrev, leftPrev->GetLeftPrev()))
        {
            return;
        }
        if(!RemapEdgeFromEdge(leftPrev->GetLeftPrev(), leftPrev, leftPrev->GetRightNext()))
        {
            return;
        }
    }
    else
    {
        if(!RemapEdgeFromEdge(leftPrev->GetRightPrev(), leftPrev, leftPrev->GetLeftNext()))
        {
            return;
        }
        if(!RemapEdgeFromEdge(leftPrev->GetLeftNext(), leftPrev, leftPrev->GetRightPrev()))
        {
            return;
        }
    }
    //remap outer right
    if(rightNext->GetRightFace() == rightFace)
    {
        if(!RemapEdgeFromEdge(rightNext->GetRightNext(), rightNext, rightNext->GetLeftPrev()))
        {
            return;
        }
        if(!RemapEdgeFromEdge(rightNext->GetLeftPrev(), rightNext, rightNext->GetRightNext()))
        {
            return;
        }
    }
    else
    {
        if(!RemapEdgeFromEdge(rightNext->GetRightPrev(), rightNext, rightNext->GetLeftNext()))
        {
            return;
        }
        if(!RemapEdgeFromEdge(rightNext->GetLeftNext(), rightNext, rightNext->GetRightPrev()))
        {
            return;
        }
    }
    
    //remap inner left
    if(leftPrev->GetLeftFace() == leftFace)
    {
        if(!RemapEdgeFromEdge(leftPrev->GetRightPrev(), leftPrev, leftNext))
        {
            return;
        }
        if(!RemapEdgeFromEdge(leftNext, edge, leftPrev->GetRightPrev()))
        {
            return;
        }
        
    }
    else
    {
        if(!RemapEdgeFromEdge(leftPrev->GetLeftPrev(), leftPrev, leftNext))
        {
            return;
        }
        if(!RemapEdgeFromEdge(leftNext, edge, leftPrev->GetLeftPrev()))
        {
            return;
        }
    }
    
    //remap inner right
    if(rightNext->GetRightFace() == rightFace)
    {
        if(!RemapEdgeFromEdge(rightNext->GetRightPrev(), rightNext, rightPrev))
        {
            return;
        }
        if(!RemapEdgeFromEdge(rightPrev, edge, rightNext->GetRightPrev()))
        {
            return;
        }
        
    }
    else
    {
        if(!RemapEdgeFromEdge(rightNext->GetRightNext(), rightNext, rightPrev))
        {
            return;
        }
        if(!RemapEdgeFromEdge(rightPrev, edge, rightNext->GetRightNext()))
        {
            return;
        }
    }
    
    if(!RemapVertexsEdge(rightFaceVertex, rightNext))
    {
        return;
    }
    
    if(!RemapVertexsEdge(leftFaceVertex, leftPrev))
    {
        return;
    }
    
    /*if(!RemapVertexsEdge(start, edge))
    {
        return;
    }*/
    
    //remap rightprev and leftnext faces
    if(!RemapEdgesFace(rightPrev, rightFace, rightNextAdjoiningFace))
    {
        return;
    }
    if(!RemapEdgesFace(leftNext, leftFace, leftPrevAdjoiningFace))
    {
        return;
    }
    
    
    //Adds quadrics
    start->AddQuadric(*end);
    start->SetCoordinates(newVertexCoordinates);
    //Remove 2 Faces, 3 edges and 1 vertex
    faceList->EraseFace(rightFace);
    faceList->EraseFace(leftFace);
    if(rightPrev->CheckEdgeForVertex(end))
    {
        return;
    }
    if(leftNext->CheckEdgeForVertex(end))
    {
        return;
    }
    if(leftNext->GetRightNext()->CheckEdgeForVertex(end))
    {
        return;
    }
    if(leftNext->GetRightPrev()->CheckEdgeForVertex(end))
    {
        return;
    }
    if(leftNext->GetLeftNext()->CheckEdgeForVertex(end))
    {
        return;
    }
    if(leftNext->GetLeftPrev()->CheckEdgeForVertex(end))
    {
        return;
    }
    if(rightPrev->GetRightNext()->CheckEdgeForVertex(end))
    {
        return;
    }
    if(rightPrev->GetRightPrev()->CheckEdgeForVertex(end))
    {
        return;
    }
    if(rightPrev->GetLeftNext()->CheckEdgeForVertex(end))
    {
        return;
    }
    if(rightPrev->GetLeftPrev()->CheckEdgeForVertex(end))
    {
        return;
    }
    
    vertexList->EraseVertex(end);
    
    
    if(rightNext != NULL)
    {
        RemoveEdge(rightNext);
        delete rightNext;
        rightNext = NULL;
        edgeCount--;
    }
    if(leftPrev != NULL)
    {
        RemoveEdge(leftPrev);
        delete leftPrev;
        leftPrev = NULL;
        edgeCount--;
    }
    if(edge != NULL)
    {
        RemoveEdge(edge);
        delete edge;
        edge = NULL;
        edgeCount--;
    }
    
    
}

int EdgeMap::GetHashCount(int index)
{
    index = mod(index, tableSize);
    int hashCount = 0;
    WingedEdge *current = table[index];
    if(current == NULL)
    {
        return hashCount;
    }
    
    do
    {
        hashCount++;
        current = current->GetNext();
    }while(current != NULL);
    
    return hashCount;
}


void EdgeMap::ReHashEdge(WingedEdge *edge)
{
    if(edge == NULL)
    {
        return;
    }
    int originalIndex = edge->GetStartIndex();
    //edge->RecalculateIndexes();
    int newIndex = edge->GetStartIndex();
    //returns if no change
    if(originalIndex == newIndex)
    {
        return;
    }
    
    RemoveEdge(edge);
    //gets new index

    //remaps the edge to the correct bin
    WingedEdge * current = table[edge->GetStartIndex()];
    if(current == NULL)
    {
        table[edge->GetStartIndex()] = edge;
    }
    else
    {
        while(current->GetNext() != NULL)
        {
            current = current->GetNext();
        }
        current->SetNext(edge);
    }

    
}

void EdgeMap::ReHashEdges()
{
    for (int i = 0; i < tableSize; i++)
    {
        WingedEdge * current = table[i];
        current->CheckEdgeForVertex(NULL);
        while(current != NULL)
        {
            ReHashEdge(current);
            current = current->GetNext();
        }
    }
}

WingedEdge *EdgeMap::GetRandomEdgeAtIndex(int index)
{
    index = mod(index, tableSize);
    WingedEdge *current = table[index];
    if(current == NULL)
    {
        return NULL;
    }
    
    int hashCount = GetHashCount(index);
    if(hashCount == 0)
    {
        return NULL;
    }
    int randIndex = rand() % hashCount;
    
    for (int i = 0; i < randIndex; i++)
    {
        if(current->GetNext() == NULL)
        {
            return current;
        }
        else
        {
            current = current->GetNext();
        }
    }
    
    return current;
}

//currently not fully weighted.. I could change it to be equally weighted but it shouldn't be a huge issue
//max degree is likely no more than 7
WingedEdge *EdgeMap::GetRandomEdge()
{
    int randIndex = 0;
    do
    {
        randIndex = rand() % tableSize;
    }while(table[randIndex] == NULL);
    return GetRandomEdgeAtIndex(randIndex);
}

WingedEdge **EdgeMap::GetkRandomEdges(int k)
{
    WingedEdge **kEdges = new WingedEdge*[k];
    
    for (int i = 0; i<k; i++)
    {
        int retrievalCount = 10;
        
        kEdges[i] = NULL;
        
        while(kEdges[i] == NULL && retrievalCount > 0)
        {
            kEdges[i] = GetRandomEdge();
            retrievalCount--;
        }
    }
    
    return kEdges;
}

WingedEdge *EdgeMap::GetEdge(int vertexStart, int vertexEnd)
{
    WingedEdge *start = table[vertexStart];
    WingedEdge *end = table[vertexEnd];
    while(start != NULL)
    {
        if(start->CheckEdgeIndexes(vertexStart, vertexEnd))
        {
            return start;
        }
        start = start->GetNext();
    }
    
    while(end != NULL)
    {
        if(end->CheckEdgeIndexes(vertexStart, vertexEnd))
        {
            return end;
        }
        end = end->GetNext();
    }
    
    return NULL;
}

void EdgeMap::MCDecimateEdge(int k)
{
    WingedEdge** kEdges = GetkRandomEdges(k);
    if(kEdges == NULL)
    {
        return;
    }
    float maxCost = 0;
    WingedEdge *maxEdge = NULL;
    float newVcoordinates[COORDINATESIZE] = {0,0,0};
    
    for(int i = 0; i<k; i++)
    {
        if(kEdges[i] != NULL)
        {
            float coordinates[COORDINATESIZE] = {0,0,0};
            float cost = kEdges[i]->CalculateCost(coordinates);
        
            if(cost > maxCost)
            {
                maxCost = cost;
                maxEdge = kEdges[i];
                for(int j = 0; j < COORDINATESIZE; j++)
                {
                    newVcoordinates[j] = coordinates[j];
                }
            }
        }
    }
    //CollapseEdge(kEdges[2], newVcoordinates);
    CollapseEdge(maxEdge, newVcoordinates);
    //CollapseEdge(GetEdge(44,918), newVcoordinates);
    //ReHashEdges();
    //maxEdge->CheckEdgeForVertex(NULL);
    if(kEdges!=NULL)
    {
        delete[] kEdges;
    }
}
void EdgeMap::MCDecimateNEdges(int N, int k)
{
    if(N > edgeCount)
    {
        return;
    }
    for(int i = 0; i< N; i++)
    {
        MCDecimateEdge(k);
    }
    vertexList->UpdateVertexIndexes();
    //not sure how necessary the face one really is..
    faceList->UpdateFaceIndexes();
    ReHashEdges();
}
