

#include <stdio.h>
#include <math.h>
#include <iostream>
#include "Face.hpp"
#include "FacePair.hpp"
#include "FaceMap.hpp"
#include "FacePairList.hpp"
#include "FaceClusterList.hpp"
#include "FaceCluster.hpp"


// give the function a list of pairs of faces that can see each other
// and a list of pairs of faces that cannot see each other in 2 FacePairLists
// as well as a list of all faces and the desired number of output groups
// receive back a FaceClusterList
FaceClusterList* getSegmentationMap(FacePairList *canSee, FacePairList *cannotSee, FaceMap *faceList, int k) {

    //// construct the LoS matrix
    int facen = faceList->GetCount();
    // make an array to represent the square LoS matrix
    int *conMatrix = new int[facen * facen]();

    // set each face as visible to itself
    for (int n = 0; n < facen; n++) {
        conMatrix[n*facen + n] = 1;
    }

    // set each pair in canSee as visible (+1)
    for (int i = 0; i < canSee->GetCount(); i++) {
        FacePair *pair = canSee->GetPair(i);
        int ind1 = pair->GetFace1()->GetIndex();
        int ind2 = pair->GetFace2()->GetIndex();
        conMatrix[ind1*facen + ind2] = 1;
        conMatrix[ind2*facen + ind1] = 1;
    }

    // set each pair in cannotSee as not visible (-1)
    for (int i = 0; i < cannotSee->GetCount(); i++) {
        FacePair *pair = cannotSee->GetPair(i);
        int ind1 = pair->GetFace1()->GetIndex();
        int ind2 = pair->GetFace2()->GetIndex();
        conMatrix[ind1*facen + ind2] = -1;
        conMatrix[ind2*facen + ind1] = -1;
    }

    //// Split the faces with k-means
    FaceClusterList *segs = new FaceClusterList();
    for (int i = 0; i < k; i++) {
        segs->AddCluster(FaceCluster());
    }

    // assign each face an initial group
    for (int n = 0; n < facen; n++) {
        segs->GetCluster(n % k)->AddFace(faceList->GetFace(n));
    }

    double *centroids = new double[k * facen]();
    double *oldCentroids = new double[k * facen]();

    bool changed = true;
    // loop until nothing changes
    while (changed) {
        changed = false;
    
        // calculate group centriods
        delete[] oldCentroids;
        oldCentroids = centroids;
        centroids = new double[k * facen]();
        for (int i = 0; i < k; i++) {
            FaceCluster *faceC = segs->GetCluster(i);
            if (faceC->GetCount() == 0) {
                // dont bother trying to divide by zero
                continue;
            }
            for (int j = 0; j < faceC->GetCount(); j++) {
                // add face connectivity data to centroid
                int faceInd = faceC->GetFace(j)->GetIndex();
                for (int n = 0; n < facen; n++) {
                    centroids[k*facen + n] += conMatrix[faceInd*facen + n];
                }
            }
            for (int n = 0; n < facen; n++) {
                centroids[k*facen + n] /= faceC->GetCount();
            }
        }
        for (int i = 0; i < k * facen; i++) {
            if (oldCentroids[i] != centroids[i]) {
                changed = true;
                break;
            }
        }

        // assign each face to the group with the nearest centriod
        segs->ClearClusters();
        for (int n = 0; n < facen; n++) {
            double closest = -1;
            int clClust = 0;
            for (int i = 0; i < k; i++) {
                double hyp = 0;
                for (int j = 0; j < facen; j++) {
                    hyp += pow(centroids[i*facen + j] - conMatrix[n*facen + j], 2);
                }
                hyp = pow(hyp, 0.5);
                if (hyp < closest || closest == -1) {
                    closest = hyp;
                    clClust = i;
                }
            }
            segs->GetCluster(clClust)->AddFace(faceList->GetFace(n));
        }

    }
        
    

    delete[] centroids;
    delete[] oldCentroids;
    delete[] conMatrix;

    return segs;
}


/*
int main(int argc, char * argv[]) {

    std::cout << "1" << std::endl;
    return;
    Face f1, f2, f3, f4;
    f1.SetIndex(1);
    f2.SetIndex(2);
    f3.SetIndex(3);
    f4.SetIndex(4);

    std::cout << "2" << std::endl;
    FaceMap fm;
    fm.AddFace(f1);
    fm.AddFace(f2);
    fm.AddFace(f3);
    fm.AddFace(f4);

    std::cout << "3" << std::endl;
    FacePairList cs, cns;
    FacePair fp12, fp34, fp13, fp14, fp23, fp24;
    fp12.MakePair(&f1, &f2);
    fp34.MakePair(&f3, &f4);
    fp13.MakePair(&f1, &f3);
    fp14.MakePair(&f1, &f4);
    fp23.MakePair(&f2, &f3);
    fp24.MakePair(&f2, &f4);
    std::cout << "4" << std::endl;
    cs.AddPair(&fp12);
    cs.AddPair(&fp34);
    cns.AddPair(&fp13);
    cns.AddPair(&fp14);
    cns.AddPair(&fp23);
    cns.AddPair(&fp24);

    std::cout << "5" << std::endl;
    return;
    int k = 2;
    FaceClusterList *fcl = getSegmentationMap(&cs, &cns, &fm, k);

    for (int i = 0; i < k; i++) {
        FaceCluster *fci = fcl->GetCluster(i);
        std::cout << i << ": ";
        for (int n = 0; n < fci->GetCount(); n++) {
            std::cout << fci->GetFace(n)->GetIndex() << ", ";
        }
        std::cout << std::endl;
    }

}
*/
