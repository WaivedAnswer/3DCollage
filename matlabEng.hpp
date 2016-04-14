//
//  matlabEng.hpp
//  464proj
//
//  Created by Cathy Yiu on 2016-04-13.
//  Copyright © 2016 Cathy Yiu. All rights reserved.
//

#ifndef matlabEng_hpp
#define matlabEng_hpp

#include <iostream>
#include "FaceClusterList.hpp"


void deleteFiles();
void readSMF(std::string);
void fclPCA(FaceClusterList);
int matlabObjPCA(double[], double[], double[],int, std::string);
void bestFit(int, double[], double[]);
void comparePCA();
void generateNewMeshFile();

#endif /* matlabEng_hpp */