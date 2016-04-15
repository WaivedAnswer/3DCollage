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


#define  BUFSIZE 256

using namespace std;
using namespace Eigen;

int vnum, fnum;
MatrixXd Mesh, fMesh;
string filename;
double objPca[3], clusterPca[3], cen[3];


void readSMF(string filename){
    
    int a,b;
    string token0, token1, token2, token3;
    ifstream inFile(filename.c_str(),ios::in);
    Mesh.resize(0,0);
    fMesh.resize(0,0);
    
    if (inFile.is_open())
    {
        // cout << "OPen" << endl;
        //Get first line for # n m
        inFile>>token0 >>token1 >> token2;
        a = atoi(token1.c_str());
        b = atoi(token2.c_str());
        vnum = a;
        fnum = b;
        MatrixXd curM(3,vnum);
        MatrixXd faceM(3,fnum);
        int verNum =0;
        int faceNum = 0;
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
            if (token0 == "f"){
                //Get face information
                inFile >> token1;
                faceM(0,faceNum) = atof(token1.c_str());
                inFile >> token2;
                faceM(1,faceNum) = atof(token2.c_str());
                inFile >> token3;
                faceM(2,faceNum) = atof(token3.c_str());
                faceNum++;
            }
        }
        
        // cout << "size = " <<curM.rows() << " x " << curM.cols() << endl;
        fMesh = faceM;
        Mesh = curM;
        inFile.close();
    }else{
        cout << "Unable to open file" << endl;
    }
    
    return;
    
}

int matlabObjPCA(double M_x[], double M_y[], double M_z[],int vsize, string filename, int type) //type1 = cluster, type2 = smf file input
{
    Engine *ep;
    mxArray *X = NULL, *Y = NULL, *Z = NULL;
//    char buffer[BUFSIZE+1];
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
    
    if (type==1){
//        cout << "gen cluster file" << endl;
        engEvalString(ep, "cenX = sum(V(:,1))/numPoint;");
        engEvalString(ep, "cenY = sum(V(:,2))/numPoint;");
        engEvalString(ep, "cenZ = sum(V(:,3))/numPoint;");
        engEvalString(ep, "fileID = fopen('clusterPCA.txt','a+t','n');");
        engEvalString(ep, "formatSpec = '\\nC %s %d %d %d %s %s %s %s %s %s %s %s %s';");
        engEvalString(ep, "fprintf(fileID,formatSpec,filename, cenX, cenY, cenZ, num2str(pc(1,1)), num2str(pc(1,2)),num2str(pc(1,3)),num2str(pc(2,1)),num2str(pc(2,2)),num2str(pc(2,3)),num2str(pc(3,1)),num2str(pc(3,2)),num2str(pc(3,3)));");
        engEvalString(ep, "fclose(fileID);");
    }else{
        engEvalString(ep, "fileID = fopen('objPCA.txt','a+t','n');");
        engEvalString(ep, "formatSpec = '\\nO %s %s %s %s %s %s %s %s %s %s';");
        engEvalString(ep, "fprintf(fileID,formatSpec,filename, num2str(pc(1,1)), num2str(pc(1,2)),num2str(pc(1,3)),num2str(pc(2,1)),num2str(pc(2,2)),num2str(pc(2,3)),num2str(pc(3,1)),num2str(pc(3,2)),num2str(pc(3,3)));");
        engEvalString(ep, "fclose(fileID);");
    }
    
    
    //free variables
    mxDestroyArray(X);
    mxDestroyArray(Y);
    mxDestroyArray(Z);
    engClose(ep);
    
    return EXIT_SUCCESS;
    
}

void getobjPCA(string filename){
    string token0, token1, token2, token3;
    string objFileindex = "objPCA.txt";
    ifstream inFile(objFileindex.c_str(),ios::in);
    
    if (inFile.is_open())
    {
        while(inFile >> token0){
            if (token0 == "O"){
                //Get vertices information
                inFile >> token0;
                if ( token0 == filename)
                inFile >> token1;
                objPca[0] = atof(token1.c_str());
                inFile >> token1;
                objPca[1] = atof(token1.c_str());
                inFile >> token1;
                objPca[2] = atof(token1.c_str());
            }
        inFile.close();
        }
    }else{
        cout << "Unable to open file" << endl;
    }
}

void getClusterPCA(string filename){
    string token0, token1, token2, token3;
    string objFileindex = "clusterPCA.txt";
    ifstream inFile(objFileindex.c_str(),ios::in);
    
    if (inFile.is_open())
    {
        while(inFile >> token0){
            if (token0 == "O"){
                //Get vertices information
                inFile >> token0;
                if ( token0 == filename)
                    inFile >> token1;
                clusterPca[0] = atof(token1.c_str());
                inFile >> token1;
                clusterPca[1] = atof(token1.c_str());
                inFile >> token1;
                clusterPca[2] = atof(token1.c_str());
            }
            inFile.close();
        }
    }else{
        cout << "Unable to open file" << endl;
    }
}

int getMeshVnum(string filename){
    int objVnum = 0;
    
    int a;
    string token0, token1, token2, token3;
    ifstream inFile(filename.c_str(),ios::in);
    Mesh.resize(0,0);
    fMesh.resize(0,0);
    
    if (inFile.is_open())
    {
        // cout << "OPen" << endl;
        //Get first line for # n m
        inFile>>token0 >>token1 >> token2;
        a = atoi(token1.c_str());
        objVnum = a;
        inFile.close();
    }else{
        cout << "Unable to open file" << endl;
    }
    
    return objVnum;
}

int getMeshFnum(string filename){
    int objFnum = 0;
    
    int a;
    string token0, token1, token2, token3;
    ifstream inFile(filename.c_str(),ios::in);
    Mesh.resize(0,0);
    fMesh.resize(0,0);
    
    if (inFile.is_open())
    {
        // cout << "OPen" << endl;
        //Get first line for # n m
        inFile>>token0 >>token1 >> token2;
        a = atoi(token2.c_str());
        objFnum = a;
        inFile.close();
    }else{
        cout << "Unable to open file" << endl;
    }
    
    return objFnum;
    
}

void getClusterCen(string index){
    string token0, token1, token2, token3;
    ifstream inFile(filename.c_str(),ios::in);
    Mesh.resize(0,0);
    fMesh.resize(0,0);
    if (inFile.is_open())
    {
        while(inFile >> token0){
            if (token0 == "C"){
                inFile >>token0;
                if (token0 == index){
                    inFile>>token0 >>token0 >> token0;
                    inFile>>token0 >>token0 >> token0;
                    inFile>>token0 >>token0 >> token0;
                    inFile>>token1 >>token2 >> token3;
                    
                    cen[0] = atoi(token1.c_str());
                    cen[1] = atoi(token2.c_str());
                    cen[2] = atoi(token3.c_str());
                }
            }
        }
        inFile.close();
    }else{
        cout << "Unable to open file" << endl;
    }
    
}

int newMeshPCA(string filename, string index)
{
    //pass object filename to matlab
    //pass center of cluster to matlab
    //pass pca of cluster to matlab
    //use matlab to compute PCA
    
    Engine *ep;
    string opc1, opc2, opc3, cpc1, cpc2, cpc3, cen1, cen2, cen3;
    string comm="";
    
    //Call engOpen with the application's Path. Open a Matlab process.
    if (!(ep = engOpen("/Applications/MATLAB_R2015b.app/bin/./matlab"))) {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        return EXIT_FAILURE;
    }
    getobjPCA(filename);
    getClusterPCA(index);
    
    comm = ("newMesh('"+filename+"', "+index+")");
    cout << comm << endl;
    engEvalString(ep, comm.c_str());

//    
//    
//    //free variables
//    mxDestroyArray(X);
//    mxDestroyArray(Y);
//    mxDestroyArray(Z);
    
    return EXIT_SUCCESS;
    
}



void fclPCA(FaceClusterList *fcl)
{
    cout << "fclPCA" << endl;
    int vsize = 0;
    string filename ="";
    for (int i = 0; i < fcl->GetCount(); i++){
        FaceCluster *fci = fcl->GetCluster(i);
        filename = i; // filename = cluster index
        stringstream ss;
        ss << i;
        filename = ss.str();
        
        vsize = fci->GetCount()*3;
        double F_x[vsize] , F_y[vsize] ,F_z[vsize];
        int k=0;
        for (int j = 0; j < fci->GetCount(); j++){
            Face *face = fci->GetFace(j);
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
        matlabObjPCA(F_x, F_y, F_z, vsize, filename, 1);
    }

    
}


void bestFit(int index, double tPCA[], double tcen[]){
    //get index, get pca[]
    //read ObjPCA file
    //find min difference
    //output to match.txt
    //format: number, filename, cen_x,y,z
    string filename = "objPCA.txt";
    string tType, tObjFilename, token;
    ifstream inFile(filename.c_str(),ios::in);
    double minDiff=0;
    string minFile;
    
    if (inFile.is_open())
    {
        double objpc[9];
        
        //initiate min data from object 1
        if(inFile >> tType){
            if (tType == "O"){
                inFile >> tObjFilename;
                for ( int i = 0; i<9; i++){
                    inFile >> token;
                    objpc[i] = atof(token.c_str());
                    minDiff = minDiff + abs(tPCA[i] - objpc[i]);
                }
                minFile = tObjFilename;
            }
        }
            
        //look for other data and compare
        while(inFile >> tType){
            if (tType == "O"){
                double cur_diff = 0;
                inFile >> tObjFilename;
                for ( int i = 0; i<9; i++){
                    inFile >> token;
                    objpc[i] = atof(token.c_str());
                    cur_diff = cur_diff + abs(tPCA[i] - objpc[i]);
                }
                if ( cur_diff < minDiff){
                    minDiff = cur_diff;
                    minFile = tObjFilename;
                }
                
            }
        }
        inFile.close();
        //Append new match data to match.txt
        ofstream outfile;
        outfile.open("match.txt", std::ios_base::app);
        outfile <<"\nM " << index <<" " << minFile.c_str() << " " <<tcen[0] << " " <<tcen[1] << " " <<tcen[2];
        outfile.close();

    }else{
        cout << "Unable to open file" << endl;
    }
    return;
}


void comparePCA(){
    string filename = "clusterPCA.txt";
    string tType, tObjIndex, token;
    ifstream inFile(filename.c_str(),ios::in);
    
    if (inFile.is_open())
    {
        double tarpc[9], cen[3];
        int tarIndex=0;
        while(inFile >> tType){
            if (tType == "C"){
                //Get Target PCA information
                inFile >> tObjIndex;
                tarIndex = atof(tObjIndex.c_str());
                for ( int i = 0; i<3; i++){
                    inFile >> token;
                    cen[i] = atof(token.c_str());
                }
                for ( int i = 0; i<9; i++){
                    inFile >> token;
                    tarpc[i] = atof(token.c_str());
                }
                bestFit(tarIndex, tarpc, cen);
            }
        }
        inFile.close();
    }else{
        cout << "Unable to open file" << endl;
    }
    return;
    
}


void generateNewMeshFile()
{
    string filename = "match.txt";
    string tObjfilename, token, cenX, cenY, cenZ;
    ifstream inFile(filename.c_str(),ios::in);
   
    
    if (inFile.is_open())
    {
       int verCount =0;
        while(inFile >> token){
            
            //for each match, copy object file mesh and translate to center, add to existing new mesh file
            if (token == "M"){
                
                inFile >> token >> tObjfilename >> cenX >> cenY >> cenZ;
                int x,y,z;
                x = atoi(cenX.c_str());
                y = atoi(cenY.c_str());
                z = atoi(cenZ.c_str());
                ifstream inMeshFile(tObjfilename.c_str(),ios::in);
                int count=0;
                if ( inMeshFile.is_open()){
 
                        ofstream outfile;
                        outfile.open("newMesh.txt", std::ios_base::app);
                    
                        //Get first line for # n m
                        int a,b;
                        string token0, token1, token2, token3;
                        inMeshFile>>token0 >>token1 >> token2;
                        a = atoi(token1.c_str());
                        b = atoi(token2.c_str());
                        double curM[3];
                        int faceM[3];
                        while(inMeshFile >> token0){
                            
                            if (token0 == "v"){
                                //Get vertices information and translate to originate at cluster center
                                inMeshFile >> token1;
                                curM[0] = atof(token1.c_str()) + x;
                                inMeshFile >> token2;
                                curM[1] = atof(token2.c_str()) + y;
                                inMeshFile >> token3;
                                curM[2] = atof(token3.c_str()) + z;
                                outfile <<"\nv " <<curM[0] << " " <<curM[1] << " "<<curM[2];
                                count++;
                            }
                            cout << verCount << endl;
                            if (token0 == "f"){
                                //Get face information and write to file
                                inMeshFile >> token1;
                                faceM[0] = atof(token1.c_str()) + verCount;
                                inMeshFile >> token2;
                                faceM[1] = atof(token2.c_str()) + verCount;
                                inMeshFile >> token3;
                                faceM[2] = atof(token3.c_str()) + verCount;
                                outfile <<"\nf " <<faceM[0] << " " <<faceM[1] << " "<<faceM[2];
                            }
                            
                        }

                    verCount = verCount + count;
                    cout << "verCount = " << verCount << endl;
                    inMeshFile.close();
                    outfile.close();
                    
                }else{
                    cout << "Unable to open file" << endl;
                }

                
            }
        }
        inFile.close();
    }else{
        cout << "Unable to open file" << endl;
    }
    return;
}

void deleteFiles(){
        if( remove( "match.txt" ) != 0 || remove( "objPCA.txt" ) != 0 || remove( "clusterPCA.txt" ) != 0 )
            perror( "Error deleting file" );
        else
            puts( "File successfully deleted" );
    return;
}


int test()
{
    string filename = "man.smf";
    string index = "0";
//    readSMF(filename);
//    if (vnum!=0){
//        double M_x[vnum],M_y[vnum],M_z[vnum];
//        for (int i=0; i<vnum; i++){
//            M_x[i] = Mesh(0,i);
//            M_y[i] = Mesh(1,i);
//            M_z[i] = Mesh(2,i);
//        }
//        matlabObjPCA(M_x,M_y,M_z,vnum, filename, 2);
//    }
//    comparePCA();
//    generateNewMeshFile();
    cout << "test()" << endl;
    newMeshPCA(filename, index);

    return 0;
}

