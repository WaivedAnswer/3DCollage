//
//  FacePairList.cpp
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-03-17.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#include "FacePairList.hpp"

FacePairList::FacePairList()
{
    count = 0;
}

FacePairList::FacePairList(int size)
 {
     count = 0;
     pairList.reserve(size);
 }

void FacePairList::Reserve(int size)
{
    pairList.reserve(size);
}

FacePairList::~FacePairList()
{
    
}
void FacePairList::AddPair(const FacePair &pair)
{
    pairList.push_back(pair);
    count++;
}

FacePair* FacePairList::GetPair(int index)
{
    if(index > count)
    {
        return NULL;
    }
    /*int pairCount = 0;
    for (std::vector<FacePair>::iterator it = pairList.begin(); it != pairList.end(); ++it)
    {
        if (pairCount == index)
        {
            return &(*it);
        }
        pairCount++;
    }
    return NULL;*/
    return &pairList[index];
}

int FacePairList::GetCount() {
    return count;
}