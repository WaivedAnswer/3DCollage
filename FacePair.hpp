//
//  FacePair.hpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-17.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef FacePair_hpp
#define FacePair_hpp

#include <stdio.h>
#include "Face.hpp"

class FacePair
{
public:
    FacePair();
    FacePair(Face *f1, Face *f2);
    void MakePair(Face *f1, Face *f2);
    Face *GetFace1();
    Face *GetFace2();
    //Vertex *GetVertexes();
private:
    Face *face1;
    Face *face2;
    //Vertex *vertexes[2];

};
#endif /* FacePair_hpp */
