//
//  FacePair.cpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-17.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "FacePair.hpp"

FacePair::FacePair()
{
    face1 = NULL;
    face2 = NULL;
}

void FacePair::MakePair(Face *f1, Face *f2)
{
    face1 = f1;
    face2 = f2;
}

Face *FacePair::GetFace1()
{
    return face1;
}

Face *FacePair::GetFace2()
{
    return face2;
}
