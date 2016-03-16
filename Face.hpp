//
//  Face.hpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-03-11.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef Face_hpp
#define Face_hpp

#include <stdio.h>
#include "Vertex.hpp"
#include "WingedEdge.hpp"
#include "SmfCommon.hpp"

class Face
{
public:
    Face();
    
    bool operator==(const Face& rhs);
    
    
    void SetIndex(int findex);
    
    WingedEdge *GetEdge();
    void SetEdge(WingedEdge *fEdge);
    
    void CalculateNormal();
    
    void GetNormal(float (&vNormal)[COORDINATESIZE]);
    
    //called after normal is calculated
    void CalculatePlaneOffset();
    
    void GetQuadric(Vertex * vertex, float (&vQuadric)[QUADRICSIZE]);
    
private:
    
    int index;
    WingedEdge *edge = NULL;
    float normal[COORDINATESIZE];
    //probably should have been using 4 vectors
    float offset;
    //float quadrics[QUADRICSIZE];
    
};//*FaceList;

#endif /* Face_hpp */
