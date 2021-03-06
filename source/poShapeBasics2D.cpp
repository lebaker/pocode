/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  poShapeBasics2D.cpp
//  pocode
//
//  Created by Jared Schiffman on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poHelpers.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"

#pragma mark poRectShape

poRectShape::poRectShape() {
	construct(100,100,0);
}

poRectShape::poRectShape(float width, float height, float rad) {
	construct(width,height,rad);
}

poRectShape::poRectShape(poTexture *tex, poTextureFitOption fit, poAlignment align) {
	construct(tex->getWidth(), tex->getHeight(), 0);
	placeTexture(tex, fit, align);
}

poRectShape::poRectShape(const poFilePath &filePath, poTextureFitOption fit, poAlignment align) {
	poTexture *tex = poGetTexture(filePath);
	if(tex && tex->isValid()) {
		construct(tex->getWidth(), tex->getHeight(), 0);
		placeTexture(tex, fit, align);
	}
}

poRectShape::poRectShape(float width, float height, poTexture *tex, poTextureFitOption fit, poAlignment align) {
	construct(width, height, 0);
	placeTexture(tex, fit, align);
}

poObject *poRectShape::copy() {
	poRectShape *shp = new poRectShape();
	clone(shp);
	return shp;
}

void poRectShape::clone(poRectShape *shp) {
	shp->radius = radius;
	poShape2D::clone(shp);
}

void poRectShape::reshape( float w, float h, float rad ) {
	clearPoints();
	construct(w, h, rad);
	
	setAlignment(getAlignment());
	if(strokeEnabled)
		generateStroke(getStrokeWidth());
}

void poRectShape::reshape( poRect r, float rad ) {
	reshape(r.width, r.height, rad);
}

void poRectShape::setCornerRadius(float rad) {
	radius = rad;
}

float poRectShape::getCornerRadius() const {
	return radius;
}

void poRectShape::construct( float w, float h, float rad ) {
    //setSize(w,h);
	radius = rad;
	
	if(rad > 0.f) {
 		addPoints(roundedRect(w,h,rad));
	}
    else {
		addPoint( poPoint(0,0) );
		addPoint( poPoint(w,0) );
		addPoint( poPoint(w,h) );
		addPoint( poPoint(0,h) );
	}
}

#pragma mark poOvalShape

poOvalShape::poOvalShape() {
    construct( 100,100,30 );
}

poOvalShape::poOvalShape(float rad) {
	int segs = 10.f * sqrtf(rad);
	construct(rad, rad, segs);
}

poOvalShape::poOvalShape( float _width, float _height, int nPoints ) {
    construct( _width, _height, nPoints );
}

void    poOvalShape::reshape( float _width, float _height, int nPoints ) {
    // if the number of points is the same as before, just use the existing point
    if ( nPoints == getNumPoints() ) {
        width = _width;
        height = _height;
        
        float dA = (M_PI*2) / (float) nPoints;
        for( int i=0; i<nPoints; i++ )
            setPoint( i, poPoint(_width/2.0*cos(dA*i), _height/2.0*sin(dA*i), 0) );
        
        setAlignment( getAlignment() );
    }
	// if not, clear the list and make new points
    else {
        clearPoints();
        construct( _width, _height, nPoints );
    }
	
	if(strokeEnabled)
		generateStroke(getStrokeWidth());
}

void poOvalShape::construct( float _width, float _height, int nPoints ) {
    width = _width;
    height = _height;
    
    float dA = (M_PI*2) / (float) nPoints;
    for( int i=0; i<nPoints; i++ ) {
        addPoint( _width/2.0*cos(dA*i), _height/2.0*sin(dA*i) );
    }
}

poObject* poOvalShape::copy() {
	poOvalShape *shp = new poOvalShape();
	clone(shp);
	return shp;
}

void poOvalShape::clone(poOvalShape *shp) {
	shp->width = width;
	shp->height = height;
	poShape2D::clone(shp);
}

// ============== poLineShape ================================

poLineShape::poLineShape() {
    construct( poPoint(0,0,0), poPoint(0,100,0) );
}

poLineShape::poLineShape( poPoint A, poPoint B ) {
    construct( A,B );
}

void    poLineShape::reshape( poPoint A, poPoint B ) {
	clearPoints();
	construct(A,B);
}

void    poLineShape::construct( poPoint A, poPoint B ) {
    addPoint( A );
    addPoint( B );
    
    closed = false;
    fillEnabled = false;
	generateStroke(1,PO_STROKE_PLACE_CENTER,PO_STROKE_JOIN_MITRE,PO_STROKE_CAP_ROUND);
}

// ============== poStarShape ================================

poStarShape::poStarShape() {
    construct(100, 5, 40);
}

poStarShape::poStarShape( float _outerRadius, int nPoints, float depth ) {
    construct( _outerRadius, nPoints, depth );
}

poStarShape::poStarShape( float _width, float _height, int nPoints, float depth ) {
    construct( _width, _height, nPoints, depth );
}

poObject* poStarShape::copy() {
	poStarShape *shp = new poStarShape();
	clone(shp);
	return shp;
}

void poStarShape::clone(poStarShape* shp) {
	shp->width = width;
	shp->height = height;
	shp->depth = depth;
	shp->outerRadius = outerRadius;
	shp->innerRadius = innerRadius;
	poShape2D::clone(shp);
}

void poStarShape::construct(float _outerRadius, int nPoints, float depth) {
    outerRadius = _outerRadius;
    float apothem = outerRadius*cos(M_PI/nPoints);
    innerRadius = apothem - depth;
    
    addPoint( 0.f, 0.f );
    float dA = (M_PI*2) / (float) (2 * nPoints);
    float origin = (M_PI*2) * 3/4;
    for( int i=0; i<2*nPoints; i++ ) {
		if( i%2 == 0 )
			addPoint( outerRadius*cos(dA*i + origin), outerRadius*sin(dA*i + origin) );
		else
			addPoint( innerRadius*cos(dA*i + origin), innerRadius*sin(dA*i + origin) );
    }
    addPoint( 0.f, -outerRadius );
}

void poStarShape::construct(float _width, float _height, int nPoints, float depth) {
    width = _width;
    height = _height;
    
    addPoint( 0.f, 0.f );
    float dA = (M_PI*2) / (float) (2 * nPoints);
    float origin = (M_PI*2) * 3/4;
    for( int i=0; i<2*nPoints; i++ ) {
		if( i%2 == 0 )
            addPoint( width/2.0*cos(dA*i + origin), height/2.0*sin(dA*i + origin) );
        else
            addPoint( (width/2.0 - depth)*cos(dA*i + origin), (height/2.0 - depth)*sin(dA*i + origin) );
    }
    addPoint( 0.f, -height/2.0 );
}

void    poStarShape::reshape( float _outerRadius, int nPoints, float depth ) {
    // if the number of points is the same as before, just use the existing point
    if ( nPoints == getNumPoints() ) {
        outerRadius = _outerRadius;
        float apothem = outerRadius*cos(M_PI/nPoints);
        innerRadius = apothem - depth;
        
        float dA = (M_PI*2) / (float) (2 * nPoints);
        float origin = (M_PI*2) * 3/4;
        for( int i=0; i<2*nPoints; i++ ) {
			if( i%2 == 0 )
				setPoint( i+1, poPoint(outerRadius*cos(dA*i + origin), outerRadius*sin(dA*i + origin), 0) );
			else
				setPoint( i+1, poPoint(innerRadius*cos(dA*i + origin), innerRadius*sin(dA*i + origin), 0) );
        }
        setPoint( 2*nPoints+1, poPoint( 0.f, -outerRadius, 0) );
        setAlignment( getAlignment() );
    }
	// if not, clear the list and make new points
    else {
        clearPoints();
        construct( _outerRadius, nPoints, depth );
    }
	
	if(strokeEnabled)
		generateStroke(getStrokeWidth());
}

void    poStarShape::reshape( float _width, float _height, int nPoints, float depth ) {
    // if the number of points is the same as before, just use the existing point
    if ( nPoints == getNumPoints() ) {
        width = _width;
        height = _height;
        
        float dA = (M_PI*2) / (float) (2 * nPoints);
        float origin = (M_PI*2) * 3/4;
        for( int i=0; i<2*nPoints; i++ ) {
			if( i%2 == 0 )
				setPoint( i+1, poPoint( width/2.0*cos(dA*i + origin), height/2.0*sin(dA*i + origin), 0) );
			else
				setPoint( i+1, poPoint((width/2.0 - depth)*cos(dA*i + origin), (height/2.0 - depth)*sin(dA*i + origin), 0) );
        }
        setPoint( 2*nPoints+1, poPoint( 0.f, -height/2.0, 0) );
    }
	// if not, clear the list and make new points
    else {
        clearPoints();
        construct( _width, _height, nPoints, depth );
    }
	
	if(strokeEnabled)
		generateStroke(getStrokeWidth());
}
