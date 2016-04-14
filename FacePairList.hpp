//
//  FacePairList.hpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-17.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef FacePairList_hpp
#define FacePairList_hpp

#include <stdio.h>
#include <list>
#include "Face.hpp"
#include "FacePair.hpp"
class FacePairList
{
    FacePairList();
    ~FacePairList();
    void AddPair(FacePair* pair);
    FacePair *GetPair(int index);
    
private:
    std::list<FacePair*> pairList;
    int count;
};
#endif /* FacePairList_hpp */
