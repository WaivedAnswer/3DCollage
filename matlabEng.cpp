//
//  matlabEng.cpp
//  464proj
//
//  Created by Cathy Yiu on 2016-04-13.
//  Copyright © 2016 Cathy Yiu. All rights reserved.
//

#include "matlabEng.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include "include/engine.h"
#include "SmfCommon.hpp"
#include "Vertex.hpp"
#include "Face.hpp"
#include "FaceCluster.hpp"
//#include "FaceClusterList.hpp"
#include "Eigen/Dense"
#include <fstream>


//create a folder in local that contains all the matlab include files
//copy or put shortcut of libmx and libeng in local and add to libker library

#define  BUFSIZE 256

using namespace std;
using namespace Eigen;

int vnum;
MatrixXd Mesh;
string filename;


void readSMF(){
    filename = "man.smf";
    int a;
    string token0, token1, token2, token3;
    ifstream inFile(filename.c_str(),ios::in);
    
    if (inFile.is_open())
    {
        // cout << "OPen" << endl;
        //Get first line for # n m
        inFile>>token0 >>token1 >> token2;
        a = atoi(token1.c_str());
        vnum = a;
        MatrixXd curM(3,vnum);
        int verNum =0;
        while(inFile >> token0){
            if (token0 == "v"){
                //Get vertices information
                inFile >> token1;
                curM(0,verNum) = atof(token1.c_str());
                inFile >> token2;
                curM(1,verNum) = atof(token2.c_str());
                inFile >> token3;
                curM(2,verNum) = atof(token3.c_str());
                verNum++;
            }
        }
        
        // cout << "size = " <<curM.rows() << " x " << curM.cols() << endl;
        Mesh = curM;
        inFile.close();
    }else{
        cout << "Unable to open file" << endl;
    }
    
    return;
    
}

int matlabObjPCA(double M_x[], double M_y[], double M_z[],int vsize, string filename)
{
    Engine *ep;
    mxArray *X = NULL, *Y = NULL, *Z = NULL;
    char buffer[BUFSIZE+1];
    double MX[vsize], MY[vsize], MZ[vsize];
    
    
    
    for (int i=0; i<vsize; i++){
        MX[i] = M_x[i];
        MY[i] = M_y[i];
        MZ[i] = M_z[i];
    }
    
    
    //Call engOpen with the application's Path. Open a Matlab process.
    if (!(ep = engOpen("/Applications/MATLAB_R2015b.app/bin/./matlab"))) {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        return EXIT_FAILURE;
    }
    
    
    //Create variables for matlab data
    X = mxCreateDoubleMatrix(vsize, 1, mxREAL);
    memcpy((void *)mxGetPr(X), (void *)MX, sizeof(MX));
    Y = mxCreateDoubleMatrix(vsize, 1, mxREAL);
    memcpy((void *)mxGetPr(Y), (void *)MY, sizeof(MY));
    Z = mxCreateDoubleMatrix(vsize, 1, mxREAL);
    memcpy((void *)mxGetPr(Z), (void *)MZ, sizeof(MZ));
    
    
    //Pass in data to Matlab workspace
    engPutVariable(ep, "V1", X);
    engPutVariable(ep, "V2", Y);
    engPutVariable(ep, "V3", Z);
    engPutVariable(ep, "V3", Z);
    
    //integrate data to one single matrix
    engEvalString(ep, "[numPoint, dim] = size(V1);");
    engEvalString(ep, "V = zeros(numPoint, 3);");
    engEvalString(ep, "V(:,1) = V1(:);");
    engEvalString(ep, "V(:,2) = V2(:);");
    engEvalString(ep, "V(:,3) = V3(:);");
    
    
    //code for echoing matlab command output
    //    buffer[BUFSIZE] = '\0';
    //    engOutputBuffer(ep, buffer, BUFSIZE);
    //    printf("%s", buffer);
    
    string comm1 = ("filename = '"+filename+"'");
    engEvalString(ep, comm1.c_str());
    engEvalString(ep, "[pc,~,~,~] = pca(V);");
    engEvalString(ep, "fileID = fopen('objPCA.txt','a+t','n');");
    engEvalString(ep, "formatSpec = '\\n%s %s %s %s %s %s %s %s %s %s';");
    engEvalString(ep, "fprintf(fileID,formatSpec,filename, num2str(pc(1,1)), num2str(pc(1,2)),num2str(pc(1,3)),num2str(pc(2,1)),num2str(pc(2,2)),num2str(pc(2,3)),num2str(pc(3,1)),num2str(pc(3,2)),num2str(pc(3,3)));");
    engEvalString(ep, "fclose(fileID);");
    
    
    //free variables
    mxDestroyArray(X);
    mxDestroyArray(Y);
    mxDestroyArray(Z);
    engClose(ep);
    
    return EXIT_SUCCESS;
    
}


void fclPCA(FaceClusterList *fcl)
{
    int vsize = 0;
    string filename ="";
    for (int i = 0; i < fcl->GetCount(); i++){
        FaceCluster *fci = fcl->GetCluster(i);
        filename = i; // filename = cluster index
        vsize = fci->GetCount()*3;
        double F_x[vsize] , F_y[vsize] ,F_z[vsize];
        int k=0;
        for (int j = 0; j < fci->GetCount(); j++){
            Face *face = fci->GetFace(j);
            for ( int h = 0; h < COORDINATESIZE; h ++){
                float ver[3][3];
                face->GetVertices(ver);
                //retrieve all the vertice and pass them to PCA function
                for (int v = 0; v<3; v++){
                    F_x[k] = ver[v][0];
                    F_y[k] = ver[v][1];
                    F_z[k] = ver[v][2];
                    k++;
                }
            }
        }
        matlabObjPCA(F_x, F_y, F_z, vsize, filename);
    }

    
}

int test()
{
    readSMF();
    if (vnum!=0){
        double M_x[vnum],M_y[vnum],M_z[vnum];
        for (int i=0; i<vnum; i++){
            M_x[i] = Mesh(0,i);
            M_y[i] = Mesh(1,i);
            M_z[i] = Mesh(2,i);
        }
        matlabObjPCA(M_x,M_y,M_z,vnum, filename);
    }
    
    
    return 0;
}

