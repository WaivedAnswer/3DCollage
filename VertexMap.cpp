//
//  VertexMap.cpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "VertexMap.hpp"

VertexMap::VertexMap()
{
    count = 0;
    dirty = false;
}


//To be called after decimation or subdivision
void VertexMap::UpdateVertexIndexes()
{
    int updateCount = 0;
    for (std::list<Vertex>::iterator it = vertexList.begin(); it != vertexList.end(); it++)
    {
        it->SetIndex(updateCount);
        updateCount++;
    }
    count = updateCount;
    dirty = false;
}

std::list<Vertex>::iterator VertexMap::GetBeginIterator()
{
    return vertexList.begin();
}

std::list<Vertex>::iterator VertexMap::GetEndIterator()
{
    return vertexList.end();
}

Vertex *VertexMap::GetVertex(int index)
{
    if(index > count || dirty == true)
    {
        return NULL;
    }
    int vertexCount = 0;
    for (std::list<Vertex>::iterator it = vertexList.begin(); it != vertexList.end(); it++)
    {
        if (vertexCount == index)
        {
            return &(*it);
        }
        vertexCount++;
    }
    return NULL;
}

//to be called after import
void VertexMap::InitQuadrics()
{
    for (std::list<Vertex>::iterator it = vertexList.begin(); it != vertexList.end(); it++)
    {
        it->InitQuadric();
    }
}

//to be called after import
void VertexMap::CalculateNormals()
{
    for (std::list<Vertex>::iterator it = vertexList.begin(); it != vertexList.end(); it++)
    {
        it->CalculateVertexNormal();
    }
}


void VertexMap::AddVertex(Vertex vertex)
{
    vertex.SetIndex(count);
    vertexList.push_back(vertex);
    count++;
}

bool VertexMap::EraseVertex(Vertex *vertex)
{
    if(vertex == NULL)
    {
        return false;
    }
    std::list<Vertex>::iterator it = std::find(vertexList.begin(), vertexList.end(), *vertex);
    if(it != vertexList.end())
    {
        vertexList.erase(it);
        count--;
        dirty = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool VertexMap::EraseVertex(int index)
{
    if(index > count )
    {
        return false;
    }
    int vertexCount = 0;
    for (std::list<Vertex>::iterator it = vertexList.begin(); it != vertexList.end(); it++)
    {
        if (vertexCount == index)
        {
            vertexList.erase(it);
            count--;
            dirty = true;
            return true;
        }
        vertexCount++;
    }
    
    return false;
}

int VertexMap::GetCount()
{
    return count;
}

void VertexMap::Reset()
{
    vertexList.clear();
    count = 0;
    dirty = false;
}