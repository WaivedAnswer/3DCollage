//
//  VertexPair.hpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-15.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef VertexPair_hpp
#define VertexPair_hpp

#include <stdio.h>
#include "Vertex.hpp"

class VertexPair
{
public:
    VertexPair();
    void MakePair(Vertex *v1, Vertex *v2);
    void SetVertex1(Vertex *v1);
    void SetVertex2(Vertex *v2);
    Vertex *GetVertex1();
    Vertex *GetVertex2();
    //Vertex *GetVertexes();
private:
    Vertex *vertex1;
    Vertex *vertex2;
    //Vertex *vertexes[2];
};
#endif /* VertexPair_hpp */
