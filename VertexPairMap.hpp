//
//  VertexPairMap.hpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-15.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef VertexPairMap_hpp
#define VertexPairMap_hpp

#include <stdio.h>
#include <list>
#include "VertexPair.hpp"
#include "Vertex.hpp"
class VertexPairMap
{
public:
    VertexPairMap();
    ~VertexPairMap();
    void AddPair(VertexPair *pair);
    void GetPair(int index);
    
private:
    std::list<VertexPair> pairList;
};
#endif /* VertexPairMap_hpp */
