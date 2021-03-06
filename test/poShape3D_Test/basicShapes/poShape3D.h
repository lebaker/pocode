//
//  poShape3D.h
//  basicShapes
//
//  Created by Jared Schiffman on 10/26/11.
//  Copyright 2011 Potion. All rights reserved.
//

#ifndef PO_SHAPE_3D_H
#define PO_SHAPE_3D_H

#include "poObject.h"

class poVertex3D
{
public:
    poPoint position;
    poPoint normal;
    poPoint textureCoords;
    poColor color;
};

class poTriangle3D
{
public:
    int     vertexIndexSet[3];
    
    void    set( int A, int B, int C ) { vertexIndexSet[0]=A; vertexIndexSet[1]=B; vertexIndexSet[2]=C; };
};

typedef std::vector<poVertex3D>    poVertex3DVector;
typedef std::vector<poTriangle3D>  poTriangle3DVector;


class poShape3D : public poObject
{
public:
    poShape3D();
    
    virtual void        draw();
    
    int                 addVertex();
    int                 addVertex( poPoint pos );
    int                 addVertex( poPoint pos, poPoint texCoords );
    int                 addVertex( poPoint pos, poPoint texCoords, poPoint normal );
    int                 addVertex( poPoint pos, poPoint texCoords, poPoint normal, poColor color );
    
    int                 numVertices() { return vertexList.size(); };
    poVertex3D&         getVertex( int N );
    
    int                 addTriangle();
    int                 addTriangle( int vertexIndexA, int vertexIndexB, int vertexIndexC );
    
    int                 numTriangles() { return triangleList.size(); };
    poTriangle3D&       getTriangle( int N );
    
    //void              generateTrianglesFromPolygon( std::vector<poPoint> pointList );
    
    bool                useVertexNormals;
    bool                useVertexColors;
    bool                useVertexTextureCoords;
    
    virtual void        calculateNormals();
    
    poTexture				texture;

    
protected:
    poTriangle3DVector  triangleList;
    poVertex3DVector    vertexList;
    
    std::vector<int>    triangleSizeVector;
    
    int                 vertexBufferID, indexBufferID;
    
    static poVertex3D   errorVertex;
    static poTriangle3D errorTriangle;
    
};




#endif
