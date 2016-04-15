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
#include <vector>
#include "Face.hpp"
#include "FacePair.hpp"
class FacePairList
{
public:
    FacePairList();
    FacePairList(int size);
    ~FacePairList();
    void Reserve(int size);
    void AddPair(const FacePair &pair);
    FacePair *GetPair(int index);
    int GetCount();
private:
    std::vector<FacePair> pairList;
    int count;
};
#endif /* FacePairList_hpp */
