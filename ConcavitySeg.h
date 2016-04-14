//
//  ConcavitySeg.h
//  3DCollage
//
//  Created by Quinn Ramsay on 2016-04-13.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
//

#ifndef ConcavitySeg_h
#define ConcavitySeg_h

// give the function a list of pairs of faces that can see each other
// and a list of pairs of faces that cannot see each other in 2 FacePairLists
// as well as a list of all faces and the desired number of output groups
// receive back a FaceClusterList
// need to delete the returned FaceClusterList
FaceClusterList* getSegmentationMap(FacePairList *canSee, FacePairList *cannotSee, FaceMap *faceList, int k);

#endif /* ConcavitySeg_h */
