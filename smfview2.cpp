//
//  main.cpp
//  Assignment1
//
//  Created by Quinn Ramsay on 2016-02-06.
//  Copyright © 2016 Quinn Ramsay. All rights reserved.
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <GLUI/glui.h>
#include <GLUT/glut.h>
#include "Vertex.hpp"
#include "Face.hpp"
#include "WingedEdge.hpp"
#include "VertexMap.hpp"
#include "FaceMap.hpp"
#include "EdgeMap.hpp"
#include "FaceVoxel.hpp"
#include "VoxelGrid.hpp"
#include "SmfCommon.hpp"


enum DisplayTypes {WIREFRAME, FLATSHADED, SMOOTHSHADED, SHADEDMESH};
enum ButtonValues {OPENBUTTON, SAVEBUTTON, DECIMATEBUTTON, QUITBUTTON};

/** These are the live variables passed into GLUI ***/
int DISPLAYMODE = WIREFRAME;
bool DODRAW = false;
int NDecimate = 0;
int KChoose = 0;
int MAIN_WINDOW;
float ROTATION_MATRIX[16];
float Z_TRANS = 0;
float XY_TRANS[2] = {0,0};
char OPENFILENAME[200] = "";
char SAVEFILENAME[200] = "";

//GLUI pointers
GLUI_Rotation *ROTATION;
GLUI_Translation *ZOOM;
GLUI_Translation *TRANSLATION;
GLUI_Button* DECIMATE;
//GLUI_Button* OPEN;
GLUI_EditText* OPENTEXT;

//GLUI_Button* SAVE;
GLUI_EditText* SAVETEXT;

GLUI_Button* QUIT;

//WingedEdge globals

//int faceListLength = 0;
int totalFaceListLength = 0;
//int vertexListLength = 0;
int totalVertexListLength = 0;
int edgeHelperListLength = 0;
//int edgeListLength = 0;
int totalEdgeListLength = 0;





/*struct W_edge
{
    W_edge()
    {
	vertexIndexes[0] = -1;
	vertexIndexes[1] = -1;
    };
    int vertexIndexes[2]; //kept for ordering purposes
    Vertex *start = NULL, *end = NULL;
    Face *left = NULL, *right = NULL;
    W_edge *left_prev = NULL, *left_next = NULL;
    W_edge *right_prev = NULL, *right_next = NULL;
}*EdgeList;

struct W_edgeHelper
{
    W_edgeHelper()
    {
	vertexIndexes[0] = -1;
	vertexIndexes[1] = -1;
        edgeIndex = -1;
        attachedToArray = false;
    };
    bool attachedToArray;
    int vertexIndexes[2];
    int edgeIndex;
    W_edgeHelper *next = NULL;
}*EdgeHelperList;*/

//std::vector<W_edge> EdgeList;

EdgeMap *EdgeMapList;

//std::vector<Face> FaceList;
FaceMap newFaceList;

VoxelGrid *gridList;


//std::vector<Vertex> VertexList;
VertexMap newVertexList;

void CleanupFaceList()
{
    /*if(FaceList != NULL)
    {
        delete FaceList;
        FaceList = NULL;
    }
    faceListLength = 0;*/
    
}
void CleanupVertexList()
{
    /*if(VertexList != NULL)
    {
        delete VertexList;
        VertexList = NULL;
    }
    vertexListLength = 0;*/
}

/*void CleanupEdgeList()
{
    if(EdgeList != NULL)
    {
        delete EdgeList;
        EdgeList = NULL;
    }
    edgeListLength = 0;
}
void CleanupEdgeHelper(W_edgeHelper *edgeHelper)
{
    if(edgeHelper == NULL)
    {
        return;
    }
    if(edgeHelper->next != NULL)
    {
        CleanupEdgeHelper(edgeHelper->next);
    }
    if(edgeHelper->attachedToArray)
    {
        delete edgeHelper;
        edgeHelper = NULL;
    }
    
}
void CleanupEdgeHelperList()
{
    if(EdgeHelperList != NULL)
    {
        for (int i = 0; i<edgeHelperListLength; i++)
        {
            CleanupEdgeHelper(&EdgeHelperList[i]);
        }

        delete EdgeHelperList;
        EdgeHelperList = NULL;
    }
    edgeHelperListLength = 0;
}*/

void CleanupLists()
{
    //TODO readd cleanup
    //CleanupEdgeHelperList();
    //CleanupEdgeList();
    newVertexList.Reset();
    newFaceList.Reset();
    if(EdgeMapList != NULL)
    {
        delete EdgeMapList;
        EdgeMapList = NULL;
    }
    if(gridList != NULL)
    {
        delete gridList;
        gridList = NULL;
    }
    CleanupFaceList();
    CleanupVertexList();
}

bool CheckRayTriangleIntersection(Point v1, Point v2, Point v3, Point origin, Vector3 ray)
{
    Vector3 e1, e2;
    Vector3 P, Q, T;
    float det, inv_det, u , v;
    float t;
    float eps = 0.0001;
    
    CreateVector(v2,v1,e1);
    CreateVector(v3,v1,e1);
    VectorCross(ray, e2, P);
    det = VectorDot(e1, P);
    if(det < eps )
    {
        return false;
    }
    
    return true;
}

//returns index of previous edge in edge list if exists for specified vertex indices
//if not returns -1
//generally O(n)
/*int CheckEdgeListForVertexIndexes(int start, int end)
{
    for(int i =0; i<edgeListLength; i++)
    {
        if ((start == EdgeList[i].vertexIndexes[0]
            && end == EdgeList[i].vertexIndexes[1]) ||
            (start == EdgeList[i].vertexIndexes[1]
            && end == EdgeList[i].vertexIndexes[0]))
        {
            return i;
        }
    }
    
    return -1;
}

//returns index of previous edge in edge list if exists for specified vertex indices
//if not returns -1
//Much Faster average O(1) worst O(n)
int CheckEdgeHelperListForVertexIndexes(int start, int end)
{
    W_edgeHelper * current = &EdgeHelperList[start];
    do
    {
        if((current->vertexIndexes[0] == start &&
           current->vertexIndexes[1] == end) ||
           (current->vertexIndexes[1] == start &&
            current->vertexIndexes[0] == end))
        {
            return current->edgeIndex;
        }
        current = current->next;
    }while (current != NULL);
    
    //check the other end
    current = &EdgeHelperList[end];
    do
    {
        if((current->vertexIndexes[0] == start &&
            current->vertexIndexes[1] == end) ||
           (current->vertexIndexes[1] == start &&
            current->vertexIndexes[0] == end))
        {
            return current->edgeIndex;
        }
        current = current->next;
    }while (current != NULL);
    
    return -1;
}*/

/*void CalculateSurfaceNormal(Face *face, float (&normal)[3])
{
    WingedEdge *originalFaceEdge = face->edge;
    //W_edge *firstIndexEdge = &EdgeList[0];
    WingedEdge *currFaceEdge = originalFaceEdge;
    float vec1[3] = {0,0,0};
    float vec2[3] = {0,0,0};
    //only need 3 vertexes for this
    Vertex * vertexes[3] = {NULL, NULL, NULL};
    
    for( int j=0; j<3; j++)
    {
        if(currFaceEdge->GetRightFace() == face)
        {
            vertexes[j] = currFaceEdge->GetStartVertex();
            currFaceEdge = currFaceEdge->GetRightPrev();
        }
        else
        {
            vertexes[j] = currFaceEdge->GetEndVertex();
            currFaceEdge = currFaceEdge->GetLeftPrev();
        }
    }
    
    CreateVector(vertexes[0]->coordinates, vertexes[1]->coordinates, vec1);
    CreateVector(vertexes[2]->coordinates, vertexes[1]->coordinates, vec2);
    
    VectorCross(vec2, vec1, normal);
    
    NormalizeVector(normal);
    
}*/


/*void CalculateVertexNormal(Vertex *vertex, float (&normal)[3])
{
    WingedEdge *e0 = vertex->edge;
    WingedEdge *edge = e0;
    float surfaceNormal[3] = {0,0,0};
    float surfaceNormalTotal[3] = {0,0,0};
    int surfaceCount = 0;
    do
    {
        if(edge->GetEndVertex() == vertex)
        {
            CalculateSurfaceNormal(edge->GetRightFace(), surfaceNormal);
            surfaceNormalTotal[0]+=surfaceNormal[0];
            surfaceNormalTotal[1]+=surfaceNormal[1];
            surfaceNormalTotal[2]+=surfaceNormal[2];
            surfaceCount++;
            edge = edge->GetRightNext();
        }
        else
        {
            CalculateSurfaceNormal(edge->GetLeftFace(), surfaceNormal);
            surfaceNormalTotal[0]+=surfaceNormal[0];
            surfaceNormalTotal[1]+=surfaceNormal[1];
            surfaceNormalTotal[2]+=surfaceNormal[2];
            surfaceCount++;
            edge = edge->GetLeftNext();
        }
    } while (edge != e0);
    
    normal[0] = surfaceNormalTotal[0]/surfaceCount;
    normal[1] = surfaceNormalTotal[1]/surfaceCount;
    normal[2] = surfaceNormalTotal[2]/surfaceCount;
    
    NormalizeVector(normal);
    //normalize vector
}*/

//This one could be speeded up somehow
void DrawSmoothShaded(void)
{
    //glEnable(GL_POLYGON_OFFSET_FILL);
    glColor3f(0.8f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
    for ( std::list<Face>::iterator it = newFaceList.GetBeginIterator(); it != newFaceList.GetEndIterator(); it++ )
    {
        Face * currFace = &(*it);
        if(currFace == NULL)
        {
            return;
        }
        WingedEdge *originalFaceEdge = currFace->GetEdge();
        //W_edge *firstIndexEdge = &EdgeList[0];
        WingedEdge *currFaceEdge = originalFaceEdge;
        
        
        for( int j=0; j<MESHITEMSIZE; j++)
        {
            
            if(currFaceEdge->GetRightFace() == currFace)
            {
                //as the edges have specific orderings if we use right prev must use starting index
                //std::cout << " " << currFaceEdge->vertexIndexes[0] + 1;
                float vertexNormal[COORDINATESIZE] = {0,0,0};
                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetStartVertex()->GetNormal(vertexNormal);
                currFaceEdge->GetStartVertex()->GetCoordinates(coordinates);
                //CalculateVertexNormal(currFaceEdge->GetStartVertex(), vertexNormal);
                glNormal3fv(vertexNormal);
                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetRightPrev();
            }
            else
            {
                //if using left prev need to use end vertex index
                //std::cout << " " << currFaceEdge->vertexIndexes[1] + 1;
                float vertexNormal[COORDINATESIZE] = {0,0,0};
                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetEndVertex()->GetNormal(vertexNormal);
                currFaceEdge->GetEndVertex()->GetCoordinates(coordinates);
                
                glNormal3fv(vertexNormal);
                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetLeftPrev();
            }
        }
        
    }
    glEnd();
    
    //glDisable(GL_POLYGON_OFFSET_FILL);
    
}

/*void DrawRays(void)
{
    
    glColor3f(1.0f, 0.0f, 1.0f);
    std::list<Face>::iterator firstFace = newFaceList.GetBeginIterator();
    float firstFaceCentre[COORDINATESIZE] = {0.0,0.0,0.0};
    //float origin[COORDINATESIZE] = {0.0,0.0,0.0};
    firstFace->GetCentre(firstFaceCentre);
    
    for ( std::list<Face>::iterator it = newFaceList.GetBeginIterator(); it != newFaceList.GetEndIterator(); it++ )
    {
        Face * currFace = &(*it);
        float currFaceCentre[COORDINATESIZE] = {0.0,0.0,0.0};
        if(currFace == NULL)
        {
            return;
        }
        
        currFace->GetCentre(currFaceCentre);
        
        glBegin(GL_LINE_STRIP);
        
        glVertex3fv(firstFaceCentre);
        
        glVertex3fv(currFaceCentre);

        glEnd();
    }
}*/


void DrawWireframe(void)
{
    
    glColor3f(1.0f, 1.0f, 1.0f);
    for ( std::list<Face>::iterator it = newFaceList.GetBeginIterator(); it != newFaceList.GetEndIterator(); it++ )
    {
        Face * currFace = &(*it);
        if(currFace == NULL)
        {
            return;
        }
        WingedEdge *originalFaceEdge = currFace->GetEdge();
        //W_edge *firstIndexEdge = &EdgeList[0];
        WingedEdge *currFaceEdge = originalFaceEdge;
        
        glBegin(GL_LINE_STRIP);
        //need to repeat first vertex coordinate so MESHITEM + 1
        for( int j=0; j<MESHITEMSIZE + 1; j++)
        {
            
            if(currFaceEdge->GetRightFace() == currFace)
            {
                //as the edges have specific orderings if we use right prev must use starting index
                //std::cout << " " << currFaceEdge->vertexIndexes[0] + 1;

                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetStartVertex()->GetCoordinates(coordinates);
                
                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetRightPrev();
            }
            else
            {
                //if using left prev need to use end vertex index
                //std::cout << " " << currFaceEdge->vertexIndexes[1] + 1;

                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetEndVertex()->GetCoordinates(coordinates);

                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetLeftPrev();
            }
        }
        glEnd();
    }
    
    //DrawRays();
}

void DrawVoxelShaded(void)
{
    //glEnable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_LIGHTING);
    int gridDim = gridList->GetGridDimension();
    int count = 0;
    FaceVoxel ***grid = gridList->GetGrid();
    Face * currFace = NULL;
    glBegin(GL_TRIANGLES);
    for(int i =0; i<gridDim; i++)
    {
        for(int j=0; j<gridDim;j++)
        {
            for(int k=0; k<gridDim; k++)
            {
                //FaceVoxel *test = &grid[i][j][k];
                std::vector<Face*> faceList = grid[i][j][k].GetFaceList();
                if(!faceList.empty())
                {
                    int colours = 8;
                    for(std::vector<Face*>::iterator it = faceList.begin(); it != faceList.end(); ++it)
                    {
                        currFace = *it;
                        switch (count%colours)
                        {
                            case 0:
                                glColor3f(0.2f, 0.2f, 0.2f);
                                break;
                            case 1:
                                glColor3f(0.2f, 0.2f, 0.8f);
                                break;
                            case 2:
                                glColor3f(0.2f, 0.8f, 0.2f);
                                break;
                            case 3:
                                glColor3f(0.2f, 0.8f, 0.8f);
                                break;
                            case 4:
                                glColor3f(0.8f, 0.2f, 0.2f);
                                break;
                            case 5:
                                glColor3f(0.8f, 0.2f, 0.8f);
                                break;
                            case 6:
                                glColor3f(0.8f, 0.8f, 0.2f);
                                break;
                            case 7:
                                glColor3f(0.8f, 0.8f, 0.8f);
                                break;
                        }
                        count++;
                        WingedEdge *originalFaceEdge = currFace->GetEdge();
                        //W_edge *firstIndexEdge = &EdgeList[0];
                        WingedEdge *currFaceEdge = originalFaceEdge;
                        
                        
                        //need to repeat first vertex coordinate so MESHITEM + 1
                        for( int j=0; j<MESHITEMSIZE; j++)
                        {
                            
                            if(currFaceEdge->GetRightFace() == currFace)
                            {
                                //TODO modify how/when surface normals are created
                                
                                float coordinates[COORDINATESIZE] = {0,0,0};
                                currFaceEdge->GetStartVertex()->GetCoordinates(coordinates);
                                
                                float surfaceNormal[3] = {0,0,0};
                                currFace->GetNormal(surfaceNormal);
                                
                                glNormal3fv(surfaceNormal);
                                glVertex3fv(coordinates);
                                currFaceEdge = currFaceEdge->GetRightPrev();
                            }
                            else
                            {
                                
                                float coordinates[COORDINATESIZE] = {0,0,0};
                                currFaceEdge->GetEndVertex()->GetCoordinates(coordinates);
                                
                                float surfaceNormal[3] = {0,0,0};
                                currFace->GetNormal(surfaceNormal);
                                
                                glNormal3fv(surfaceNormal);
                                glVertex3fv(coordinates);
                                currFaceEdge = currFaceEdge->GetLeftPrev();
                            }
                        }

                        
                    }
                }
            }
        }
    }
    glEnd();
    
    /*for ( std::list<Face>::iterator it = newFaceList.GetBeginIterator(); it != newFaceList.GetEndIterator(); it++ )
    {

        Face * currFace = &(*it);
        if(currFace == NULL)
        {
            return;
        }
        WingedEdge *originalFaceEdge = currFace->GetEdge();
        //W_edge *firstIndexEdge = &EdgeList[0];
        WingedEdge *currFaceEdge = originalFaceEdge;
        
        
        //need to repeat first vertex coordinate so MESHITEM + 1
        for( int j=0; j<MESHITEMSIZE; j++)
        {
            
            if(currFaceEdge->GetRightFace() == currFace)
            {
                //TODO modify how/when surface normals are created
                
                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetStartVertex()->GetCoordinates(coordinates);
                
                float surfaceNormal[3] = {0,0,0};
                currFace->GetNormal(surfaceNormal);
                
                glNormal3fv(surfaceNormal);
                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetRightPrev();
            }
            else
            {
                
                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetEndVertex()->GetCoordinates(coordinates);
                
                float surfaceNormal[3] = {0,0,0};
                currFace->GetNormal(surfaceNormal);
                
                glNormal3fv(surfaceNormal);
                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetLeftPrev();
            }
        }
        
    }
    glEnd();*/
    
    //glDisable(GL_POLYGON_OFFSET_FILL);
}

void DrawFlatShaded(void)
{
    //glEnable(GL_POLYGON_OFFSET_FILL);
    glColor3f(0.8f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
    for ( std::list<Face>::iterator it = newFaceList.GetBeginIterator(); it != newFaceList.GetEndIterator(); it++ )
    {
        Face * currFace = &(*it);
        if(currFace == NULL)
        {
            return;
        }
        WingedEdge *originalFaceEdge = currFace->GetEdge();
        //W_edge *firstIndexEdge = &EdgeList[0];
        WingedEdge *currFaceEdge = originalFaceEdge;
        
        
        //need to repeat first vertex coordinate so MESHITEM + 1
        for( int j=0; j<MESHITEMSIZE; j++)
        {
            
            if(currFaceEdge->GetRightFace() == currFace)
            {
                //TODO modify how/when surface normals are created
                
                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetStartVertex()->GetCoordinates(coordinates);

                float surfaceNormal[3] = {0,0,0};
                currFace->GetNormal(surfaceNormal);
                
                glNormal3fv(surfaceNormal);
                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetRightPrev();
            }
            else
            {
                
                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetEndVertex()->GetCoordinates(coordinates);
                
                float surfaceNormal[3] = {0,0,0};
                currFace->GetNormal(surfaceNormal);
                
                glNormal3fv(surfaceNormal);
                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetLeftPrev();
            }
        }
        
    }
    glEnd();

    //glDisable(GL_POLYGON_OFFSET_FILL);
}


void DrawShadedMesh(void)
{
    
    glColor3f(0.8f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
    for ( std::list<Face>::iterator it = newFaceList.GetBeginIterator(); it != newFaceList.GetEndIterator(); it++ )
    {
        Face *currFace = &(*it);
        if(currFace == NULL)
        {
            return;
        }
        WingedEdge *originalFaceEdge = currFace->GetEdge();
        //W_edge *firstIndexEdge = &EdgeList[0];
        WingedEdge *currFaceEdge = originalFaceEdge;
        
        
        //need to repeat first vertex coordinate so MESHITEM + 1
        for( int j=0; j<MESHITEMSIZE; j++)
        {
            
            if(currFaceEdge->GetRightFace() == currFace)
            {
                //TODO modify how/when surface normals are created
                
                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetStartVertex()->GetCoordinates(coordinates);
                
                float surfaceNormal[3] = {0,0,0};
                currFace->GetNormal(surfaceNormal);
                
                glNormal3fv(surfaceNormal);
                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetRightPrev();
            }
            else
            {
                
                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetEndVertex()->GetCoordinates(coordinates);
                
                float surfaceNormal[3] = {0,0,0};
                currFace->GetNormal(surfaceNormal);
                
                glNormal3fv(surfaceNormal);
                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetLeftPrev();
            }
        }
        
    }
    glEnd();
    
    glDisable(GL_LIGHTING);
    glPolygonOffset(-1.0, -1.0);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(0.0f, 0.0f, 0.0f);
    for ( std::list<Face>::iterator it = newFaceList.GetBeginIterator(); it != newFaceList.GetEndIterator(); it++ )
    {
        Face * currFace = &(*it);
        if(currFace == NULL)
        {
            return;
        }
        WingedEdge *originalFaceEdge = currFace->GetEdge();
        //W_edge *firstIndexEdge = &EdgeList[0];
        WingedEdge *currFaceEdge = originalFaceEdge;
        
        glBegin(GL_LINE_STRIP);
        //need to repeat first vertex coordinate so MESHITEM + 1
        for( int j=0; j<MESHITEMSIZE + 1; j++)
        {
            
            if(currFaceEdge->GetRightFace() == currFace)
            {
                //as the edges have specific orderings if we use right prev must use starting index
                //std::cout << " " << currFaceEdge->vertexIndexes[0] + 1;
                
                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetStartVertex()->GetCoordinates(coordinates);
                float vertexNormal[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetStartVertex()->GetNormal(vertexNormal);
                
                //AddVectorToPoint(vertexNormal, coordinates, 0.05, coordinates);
                
                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetRightPrev();
            }
            else
            {
                //if using left prev need to use end vertex index
                //std::cout << " " << currFaceEdge->vertexIndexes[1] + 1;
                
                float coordinates[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetEndVertex()->GetCoordinates(coordinates);
                float vertexNormal[COORDINATESIZE] = {0,0,0};
                currFaceEdge->GetStartVertex()->GetNormal(vertexNormal);
                
                //AddVectorToPoint(vertexNormal, coordinates, 0.05, coordinates);
                
                glVertex3fv(coordinates);
                currFaceEdge = currFaceEdge->GetLeftPrev();
            }
            
        }
        glEnd();
    }

    glEnable(GL_LIGHTING);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    //glDisable(GL_POLYGON_OFFSET_FILL);

}

void SetDisplay(int mode)
{
    //don't need to use mode as DISPLAYMODE is live variable
    //DISPLAYMODE = mode;
    
    switch(DISPLAYMODE)
    {
        case WIREFRAME:
            glShadeModel(GL_FLAT);
            //glDisable(GL_LIGHTING);
            break;
        case FLATSHADED:
            glShadeModel(GL_FLAT);
            //glEnable(GL_LIGHTING);
            break;
        case SMOOTHSHADED:
            glShadeModel(GL_SMOOTH);
            //glEnable(GL_LIGHTING);
            break;
        case SHADEDMESH:
            glShadeModel(GL_FLAT);
            //glEnable(GL_LIGHTING);
            break;
        default:
            glShadeModel(GL_FLAT);
            //glDisable(GL_LIGHTING);
            break;
            
    }
    glutPostRedisplay();
}
void ReadMeshFile(const char* filename);
void OutputMeshFile(const char* filename);

void glui_cb(int control)
{
    std::string savename = SAVEFILENAME;
    std::string openname = OPENFILENAME;
    /*const char * n = DECIMATENTEXT->get_text();
    std::string Nstr = n;
    const char * k = DECIMATEKTEXT->get_text();
    std::string Kstr = k;
    printf( "                 text: %s\n", DECIMATENTEXT->get_text() );
    printf( "                 text: %s\n", DECIMATEKTEXT->get_text() );*/
    switch (control)
    {
        case OPENBUTTON:
            ReadMeshFile(&openname[0]);
            break;
        case SAVEBUTTON:
            OutputMeshFile(&savename[0]);
            break;
        case DECIMATEBUTTON:
            if(EdgeMapList != NULL)
            {
                //EdgeMapList->ReHashEdges();
                DODRAW = false;
                EdgeMapList->MCDecimateNEdges(NDecimate, KChoose);
                DODRAW = true;
            }
            break;
        case QUITBUTTON:
	    CleanupLists();
            exit(EXIT_SUCCESS);
            break;
    }
}

void GetListSizes(std::string line)
{
    char *item = NULL;
    
    strtok(&line[0], " "); // not necessary (is hash sign)
    item = strtok(NULL, " ");
    totalVertexListLength = std::atoi(item);
    item = strtok(NULL, " ");
    totalFaceListLength = std::atoi(item);
    
    /*totalEdgeListLength = MESHITEMSIZE * totalFaceListLength / 2;
    
    edgeHelperListLength = totalVertexListLength;*/

}

void ReadMeshFile(const char * filename)
{
    CleanupLists();
    if(filename == NULL)
    {
	return;
    }

    std::string line;
    
    std::ifstream meshFile (filename);
    
    if(meshFile.is_open())
    {
        char * item = NULL;
        bool ListsInitialized = false;
        while (getline(meshFile, line))
        {
            //first Line
            if(!ListsInitialized && line[0] == '#')
            {
                GetListSizes(line);
                //TODO cleanup/clear vertex/face/edgeLists
                /*VertexList = new Vertex[totalVertexListLength];
                if( VertexList == NULL)
                {
                    std::cerr<< "Couldn't allocate vertex memory." << "\n";
                    delete VertexList;
                    meshFile.close();
                    return;
                }*/
                
                /*FaceList = new Face[totalFaceListLength];
                if( FaceList == NULL)
                {
                    std::cerr<< "Couldn't allocate face memory." << "\n";
                    delete FaceList;
                    meshFile.close();
                    return;
                }*/
                
                EdgeMapList = new EdgeMap(totalVertexListLength, &newVertexList, &newFaceList);
                if( EdgeMapList == NULL)
                {
                    std::cerr<< "Couldn't allocate edge map memory." << "\n";
                    delete EdgeMapList;
                    meshFile.close();
                    return;
                }
                
                ListsInitialized = true;
                
            }
            if(line[0] == 'v')
            {
                item = strtok(&line[0], " ");
                
                if(item == NULL)
                {
                    meshFile.close();
                    return;//some sort of length error missing
                }
                
                Vertex newVertex;
                float coordinates[3] = {0,0,0};
                for (int i = 0; i < COORDINATESIZE; i++)
                {
                    
                    item = strtok(NULL, " ");
                    if(item == NULL)
                    {
                        meshFile.close();
                        return; //some sort of length error missing
                    }
                    coordinates[i] = std::atof(item);
                    
                    //VertexList[vertexListLength]
                    //newVertex.coordinates[i] = std::stof(item);
                    //VertexList.push_back(newVertex);
                }
                newVertex.SetCoordinates(coordinates);
                newVertexList.AddVertex(newVertex);
                //vertexListLength++;
            }
            
            else if(line[0] == 'f')
            {
                item = strtok(&line[0], " ");
                if(item == NULL)
                {
                    meshFile.close();
                    return;//some sort of length error missing
                }
                
                int indexes[3];

                for (int i = 0; i < MESHITEMSIZE; i++)
                {
                    item = strtok(NULL, " ");
                    if(item == NULL)
                    {
                        meshFile.close();
                        return; //some sort of length error missing
                    }
                    indexes[i] = std::atoi(item);
                    //translates from 1 indexed to 0 indexed
                    indexes[i]--;
                }
                //will increment edgelist length after to create consistency with index number
                // instead of the harder to follow +1 -1 0 + 2 -2
                //need to fix later doubling efficiency issues
                //1st edge created by face list item
                //each face item ends up creating 3 edges
                //will however have double the edges each with half the info..
                //limitation might be to triangles..
               
                //checks if edge exists, if not it creates them. These will be passed to edgemap
                WingedEdge * edges[3];
                
                //checks if any edge in the face set already exists
                //if so changes index to that of existing edge
                for( int i = 0; i<MESHITEMSIZE; i++)
                {
                    int startVIndex = indexes[(MESHITEMSIZE - i + 1)%MESHITEMSIZE];
                    int endVIndex = indexes[(MESHITEMSIZE-i)%MESHITEMSIZE];
                    //edgeIndexes[i] = CheckEdgeListForVertexIndexes(startVIndex, endVIndex);
                    edges[i] = EdgeMapList->GetEdge(startVIndex, endVIndex);
                    if(edges[i] == NULL)
                    {
                        edges[i] = new WingedEdge;
                    }
                }
                
                //set face edge pointer to first edge in face, adds face to list
                Face newFace;
                newFace.SetEdge(edges[0]);
                newFaceList.AddFace(newFace);
                
                
                for( int i = 0; i<MESHITEMSIZE; i++)
                {
                    int startIndex = indexes[(MESHITEMSIZE - i + 1)%MESHITEMSIZE];
                    int endIndex = indexes[(MESHITEMSIZE-i)%MESHITEMSIZE];
                    //was originally debating creating local face and pushing back to list after creating
                    //but that messed up pointers, so I'll add to facelist
                    //and edit through the pointer to the last element in FaceList
                    //and switched back
                    //newFaceList.AddFace();
                    //Face *lastFace = newFaceList.GetLastFace();
                    
                    //std::cout << i <<" index\n";
                    
                    //std::cout << skippedindexes <<" indexes skipped \n";
                    //std::cout << changedIndexCount <<" indexes changed. \n";

                    //default values
                    //default values when no reassignment due to duplicate edges
                    WingedEdge *right_next = edges[mod(i+1, MESHITEMSIZE)];
                    //std::cout << mod(i-1, MESHITEMSIZE);
                    WingedEdge *right_prev = edges[mod(i-1, MESHITEMSIZE)];
                    int reverseIndex = indexes[(MESHITEMSIZE-i)%MESHITEMSIZE];
                    //if its new wedge
                    if(edges[i]->GetRightPrev() == NULL &&
                       edges[i]->GetRightNext() == NULL &&
                       edges[i]->GetRightFace() == NULL)
                    {
                        
                        //lastFace->edge = edges[i];
                        //sets right face to last face
                        edges[i]->SetRight(newFaceList.GetLastFace());
                        edges[i]->SetRightNext(right_next);
                        edges[i]->SetRightPrev(right_prev);
                        
                        //as facelist is listed as counterclockwise
                        //might be off sometimes as duplicate edges
                        //not even sure what these are mapping to
                        
                        //TODO double-check this one likely off
                        if (newVertexList.GetVertex(reverseIndex)->GetEdge() == NULL)
                        {
                            newVertexList.GetVertex(reverseIndex)->SetEdge(edges[i]);
                        }
                        edges[i]->SetStartIndex(startIndex);
                        edges[i]->SetEndIndex(endIndex);
                        edges[i]->SetStartVertex(newVertexList.GetVertex(startIndex));
                        if(edges[i]->GetStartVertex() == NULL)
                        {
                            return;
                        }
                        edges[i]->SetEndVertex(newVertexList.GetVertex(endIndex));
                        if(edges[i]->GetEndVertex() == NULL)
                        {
                            return;
                        }
                        
                        EdgeMapList->AddEdge(edges[i]);
                    }
                    else //edge has already been created, don't need to do anything with edge helpers right side or vertexes
                    {
                        //this might be off in some cases
                        //sets left face to last face
                        edges[i]->SetLeft(newFaceList.GetLastFace());
                        //TODO look into this
                        edges[i]->SetLeftNext(right_next);
                        edges[i]->SetLeftPrev(right_prev);
                        
                        //as facelist is listed as counterclockwise
                        //might be off sometimes as duplicate edges
                        if (newVertexList.GetVertex(reverseIndex)->GetEdge() == NULL)
                        {
                            newVertexList.GetVertex(reverseIndex)->SetEdge(edges[i]);
                        }
                    }
                }
                //newFaceList.GetLastFace()->CalculateNormal();
                //faceListLength++;
            }
        }
        meshFile.close();
        //have to call face calculate first
        float maxDimensions[COORDINATESIZE] = {0.0,0.0,0.0};
        float minDimensions[COORDINATESIZE] = {0.0,0.0,0.0};
        newVertexList.GetMaxAndMinDimensions(maxDimensions, minDimensions);
        
        
        gridList = new VoxelGrid(maxDimensions, minDimensions, 10);
        gridList->InsertFaces(newFaceList);
        newFaceList.CalculateNormals();
        newVertexList.CalculateNormals();
        newVertexList.InitQuadrics();
        //EdgeMapList->ReHashEdges();
        DODRAW = true;
    }
    else
    {
        std::cout << "Couldn't open " << filename << "\n";
    }
}

//TODO: need to redirect output to file here and filename
void OutputMeshFile(const char * filename)
{
    if(filename == NULL)
    {
        return;
    }
    std::ofstream meshFile(filename);
    
    //meshFile.open (filename);
    if(meshFile.is_open())
    {
        meshFile << "# " << newVertexList.GetCount() << " " << newFaceList.GetCount() <<"\n";
        for ( std::list<Vertex>::iterator it = newVertexList.GetBeginIterator(); it != newVertexList.GetEndIterator(); it++ )
        {
            meshFile << "v ";
            Vertex *currVertex = &(*it);
            float coordinates[3] = {0,0,0};
            currVertex->GetCoordinates(coordinates);
            for(int j = 0; j<COORDINATESIZE; j++)
            {
                meshFile << coordinates[j] << " ";
            }
            meshFile << "\n";

        }

        for ( std::list<Face>::iterator it = newFaceList.GetBeginIterator(); it != newFaceList.GetEndIterator(); it++ )
        {
            Face *currFace = &(*it);
            WingedEdge *originalFaceEdge = currFace->GetEdge();
            //WingedEdge *firstIndexEdge = &EdgeList[0];
            WingedEdge *currFaceEdge = originalFaceEdge;

            meshFile << "f";

            for( int j=0; j<MESHITEMSIZE; j++)
            {
                if(currFaceEdge->GetRightFace() == currFace)
                {
                    //as the edges have specific orderings if we use right prev must use starting index
                    meshFile << " " << currFaceEdge->GetStartIndex() + 1;
                    currFaceEdge = currFaceEdge->GetRightPrev();
                }
                else
                {
                    //if using left prev need to use end vertex index
                    meshFile << " " << currFaceEdge->GetEndIndex() + 1;
                    currFaceEdge = currFaceEdge->GetLeftPrev();
                }
	    
            }
            meshFile << "\n";
        }

        meshFile.close();
    }
    else
    {
        std::cout<< "Couldn't open " <<filename << "\n";
    }
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle( void )
{
    /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
    if ( glutGetWindow() != MAIN_WINDOW )
        glutSetWindow(MAIN_WINDOW);
    
    glutPostRedisplay();
}

/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y )
{
    float xy_aspect;
    
    xy_aspect = (float)x / (float)y;
    glViewport( 0, 0, x, y );
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0 );
    
    glutPostRedisplay();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay( void )
{
    
    if(DODRAW)
    {
    
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glTranslatef( XY_TRANS[0]*0.1, XY_TRANS[1]*0.1, Z_TRANS*0.1 - 2.0 );
        glMultMatrixf(ROTATION_MATRIX);
    
    
        //Now we render object, using the variables 'DISPLAYMODE'.
        //These are _live_ variables, which are transparently
        //updated by GLUI
    
        switch (DISPLAYMODE)
        {
            case WIREFRAME:
            
                DrawWireframe();
                break;
            case FLATSHADED:
                DrawVoxelShaded();
                //DrawFlatShaded();
                break;
            case SMOOTHSHADED:
                DrawSmoothShaded();
                break;
            case SHADEDMESH:
                DrawShadedMesh();
                break;
            default:
                DrawWireframe();
                break;
        }
    
        glutSwapBuffers();
    }
}


int main(int argc, char * argv[]) {
    
    /****************************************/
    /*   Initialize GLUT and create window  */
    /****************************************/
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( 300, 300 );
    
    MAIN_WINDOW = glutCreateWindow( "SMF_VIEW" );
    glutDisplayFunc( myGlutDisplay );
    glutReshapeFunc( myGlutReshape );
    
    /****************************************/
    /*       Set up OpenGL lights           */
    /****************************************/
    
    GLfloat light0_ambient[] =  {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_diffuse[] =  {0.5f, 0.5f, 0.5f, 0.0f};
    GLfloat light0_position[] = {1.0f, 1.0f, 1.0f, 0.0f};
    
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    /*glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_CULL_FACE);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);*/
    //glDisable(GL_LIGHTING);
    
    /****************************************/
    /*          Enable z-buffering          */
    /****************************************/
    
    glEnable(GL_DEPTH_TEST);
    
    /****************************************/
    /*         Here's the GLUI code         */
    /****************************************/
    
    GLUI *glui = GLUI_Master.create_glui( "GLUI" );
    GLUI_Listbox *listbox = glui->add_listbox("Display Mode", &DISPLAYMODE, -1, SetDisplay);
    
    listbox->add_item(WIREFRAME,"Wireframe");
    listbox->add_item(FLATSHADED,"Flat Shaded");
    listbox->add_item(SMOOTHSHADED,"Smooth Shaded");
    listbox->add_item(SHADEDMESH,"Shaded + Mesh Edges");
    
    ROTATION = glui->add_rotation("Rotate", ROTATION_MATRIX);
    ROTATION->reset();
    
    ZOOM = glui->add_translation("Zoom", GLUI_TRANSLATION_Z, &Z_TRANS);
    TRANSLATION = glui->add_translation("Translate", GLUI_TRANSLATION_XY, XY_TRANS);
    glui->add_separator();
    
    GLUI_Panel * DecimatePanel = glui->add_panel("Decimate Panel");
    DECIMATE = glui->add_button_to_panel(DecimatePanel, "Decimate", DECIMATEBUTTON, glui_cb);
    GLUI_Spinner *N_spinner = glui->add_spinner( "Collapse N", GLUI_SPINNER_INT, &NDecimate );
    GLUI_Spinner *K_spinner = glui->add_spinner( "Choose K", GLUI_SPINNER_INT, &KChoose );

    
    glui->add_separator();
    
    GLUI_Panel * OpenPanel = glui->add_panel("Open Panel");
    //OPEN = glui->add_button_to_panel(OpenPanel, "Open", OPENBUTTON, glui_cb);
    OPENTEXT = glui->add_edittext_to_panel(OpenPanel, "Filename", GLUI_EDITTEXT_TEXT, OPENFILENAME, OPENBUTTON, glui_cb);
    
    GLUI_Panel * SavePanel = glui->add_panel("Save Panel");
    //SAVE = glui->add_button_to_panel(SavePanel, "Save", SAVEBUTTON, glui_cb);
    SAVETEXT = glui->add_edittext_to_panel(SavePanel, "Filename", GLUI_EDITTEXT_TEXT, SAVEFILENAME, SAVEBUTTON, glui_cb);
    
    
    glui->add_separator();
    QUIT = glui->add_button("Quit", QUITBUTTON, glui_cb);
    
    
    glui->set_main_gfx_window( MAIN_WINDOW );
    
    /* We register the idle callback with GLUI, *not* with GLUT */
    GLUI_Master.set_glutIdleFunc( myGlutIdle );
    
    glutMainLoop();
    CleanupLists();
    return 0;
}
