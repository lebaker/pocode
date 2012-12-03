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
//  ShapeBasics2D.cpp
//  pocode
//
//  Created by Jared Schiffman on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poHelpers.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"
#include "poSimpleDrawing.h"

namespace po {
    // ----------------------------------------------------------------------------------
    // ================================ Class: RectShape ================================
    #pragma mark - RectShape -

    RectShape::RectShape() {
        construct(100,100,0);
    }

    RectShape::RectShape(float width, float height, float rad) {
        construct(width,height,rad);
    }

    RectShape::RectShape(Texture *tex, TextureFitOption fit, Alignment align) {
        construct(tex->getWidth(), tex->getHeight(), 0);
        placeTexture(tex, fit, align);
    }

    RectShape::RectShape(const FilePath &filePath, TextureFitOption fit, Alignment align) {
//        Texture *tex = GetTexture(filePath);
//        if(tex && tex->isValid()) {
//            construct(tex->getWidth(), tex->getHeight(), 0);
//            placeTexture(tex, fit, align);
//        }
    }

    RectShape::RectShape(float width, float height, Texture *tex, TextureFitOption fit, Alignment align) {
        construct(width, height, 0);
        placeTexture(tex, fit, align);
    }

    
    //------------------------------------------------------------------------
    Object *RectShape::copy() {
        RectShape *shp = new RectShape();
        clone(shp);
        return shp;
    }
    
    
    //------------------------------------------------------------------------
    void RectShape::clone(RectShape *shp) {
        shp->radius = radius;
        Shape2D::clone(shp);
    }
    
    
    //------------------------------------------------------------------------
    void RectShape::reshape( float w, float h, float rad ) {
        clearPoints();
        construct(w, h, rad);
        
        setAlignment(getAlignment());
        if(strokeEnabled)
            generateStroke(getStrokeWidth());
    }
    
    
    //------------------------------------------------------------------------
    void RectShape::reshape( Rect r, float rad ) {
        reshape(r.width, r.height, rad);
    }
    
    
    //------------------------------------------------------------------------
    void RectShape::setCornerRadius(float rad) {
        radius = rad;
    }
    
    
    //------------------------------------------------------------------------
    float RectShape::getCornerRadius() const {
        return radius;
    }
    
    
    //------------------------------------------------------------------------
    void RectShape::construct( float w, float h, float rad ) {
        //setSize(w,h);
        radius = rad;
        
        if(rad > 0.f) {
            addPoints(po::roundedRect(w,h,rad));
        }
        else {
            addPoint( Point(0,0) );
            addPoint( Point(w,0) );
            addPoint( Point(w,h) );
            addPoint( Point(0,h) );
        }
    }

    
    
    
    // ----------------------------------------------------------------------------------
    // ================================ Class: OvalShape ================================
    #pragma mark - OvalShape -

    OvalShape::OvalShape() {
        construct( 100,100,30 );
    }

    OvalShape::OvalShape(float rad) {
        int segs = 10.f * sqrtf(rad);
        construct(rad, rad, segs);
    }

    OvalShape::OvalShape( float _width, float _height, int nPoints ) {
        construct( _width, _height, nPoints );
    }
    
    
    //------------------------------------------------------------------------
    void OvalShape::reshape( float _width, float _height, int nPoints ) {
        // if the number of points is the same as before, just use the existing point
        if ( nPoints == getNumPoints() ) {
            width = _width;
            height = _height;
            
            float dA = M_2PI / (float) nPoints;
            for( int i=0; i<nPoints; i++ )
                setPoint( i, Point(_width/2.0*cos(dA*i), _height/2.0*sin(dA*i), 0) );
            
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
    
    
    //------------------------------------------------------------------------
    void OvalShape::construct( float _width, float _height, int nPoints ) {
        width = _width;
        height = _height;
        
        float dA = M_2PI / (float) nPoints;
        for( int i=0; i<nPoints; i++ ) {
            addPoint( _width/2.0*cos(dA*i), _height/2.0*sin(dA*i) );
        }
    }
    
    
    //------------------------------------------------------------------------
    Object* OvalShape::copy() {
        OvalShape *shp = new OvalShape();
        clone(shp);
        return shp;
    }
    
    
    //------------------------------------------------------------------------
    void OvalShape::clone(OvalShape *shp) {
        shp->width = width;
        shp->height = height;
        Shape2D::clone(shp);
    }
    
    
    
    
    // ----------------------------------------------------------------------------------
    // ================================ Class: LineShape ================================
    #pragma mark - LineShape -
    
    LineShape::LineShape() {
        construct( Point(0,0,0), Point(0,100,0) );
    }

    LineShape::LineShape( Point A, Point B ) {
        construct( A,B );
    }
    
    
    //------------------------------------------------------------------------
    void    LineShape::reshape( Point A, Point B ) {
        setPoint( 0, A );
        setPoint( 1, B );
    }
    
    
    //------------------------------------------------------------------------
    void    LineShape::construct( Point A, Point B ) {
        addPoint( A );
        addPoint( B );
        
        closed = false;
        fillEnabled = false;
        generateStroke(1,po::STROKE_PLACE_CENTER,po::STROKE_JOIN_MITRE,po::STROKE_CAP_ROUND);
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: Star Shape ================================
    #pragma mark - StarShape -

    StarShape::StarShape() {
        construct(100, 5, 40);
    }

    StarShape::StarShape( float _outerRadius, int nPoints, float depth ) {
        construct( _outerRadius, nPoints, depth );
    }

    StarShape::StarShape( float _width, float _height, int nPoints, float depth ) {
        construct( _width, _height, nPoints, depth );
    }
    
    
    //------------------------------------------------------------------------
    Object* StarShape::copy() {
        StarShape *shp = new StarShape();
        clone(shp);
        return shp;
    }
    
    
    //------------------------------------------------------------------------
    void StarShape::clone(StarShape* shp) {
        shp->width = width;
        shp->height = height;
        shp->depth = depth;
        shp->outerRadius = outerRadius;
        shp->innerRadius = innerRadius;
        Shape2D::clone(shp);
    }
    
    
    //------------------------------------------------------------------------
    void StarShape::construct(float _outerRadius, int nPoints, float depth) {
        outerRadius = _outerRadius;
        float apothem = outerRadius*cos(M_PI/nPoints);
        innerRadius = apothem - depth;
        
        addPoint( 0.f, 0.f );
        float dA = M_2PI / (float) (2 * nPoints);
        float origin = M_2PI * 3/4;
        for( int i=0; i<2*nPoints; i++ ) {
            if( i%2 == 0 )
                addPoint( outerRadius*cos(dA*i + origin), outerRadius*sin(dA*i + origin) );
            else
                addPoint( innerRadius*cos(dA*i + origin), innerRadius*sin(dA*i + origin) );
        }
        addPoint( 0.f, -outerRadius );
    }
    
    
    //------------------------------------------------------------------------
    void StarShape::construct(float _width, float _height, int nPoints, float depth) {
        width = _width;
        height = _height;
        
        addPoint( 0.f, 0.f );
        float dA = M_2PI / (float) (2 * nPoints);
        float origin = M_2PI * 3/4;
        for( int i=0; i<2*nPoints; i++ ) {
            if( i%2 == 0 )
                addPoint( width/2.0*cos(dA*i + origin), height/2.0*sin(dA*i + origin) );
            else
                addPoint( (width/2.0 - depth)*cos(dA*i + origin), (height/2.0 - depth)*sin(dA*i + origin) );
        }
        addPoint( 0.f, -height/2.0 );
    }
    
    
    //------------------------------------------------------------------------
    void StarShape::reshape( float _outerRadius, int nPoints, float depth ) {
        // if the number of points is the same as before, just use the existing point
        if ( nPoints == getNumPoints() ) {
            outerRadius = _outerRadius;
            float apothem = outerRadius*cos(M_PI/nPoints);
            innerRadius = apothem - depth;
            
            float dA = M_2PI / (float) (2 * nPoints);
            float origin = M_2PI * 3/4;
            for( int i=0; i<2*nPoints; i++ ) {
                if( i%2 == 0 )
                    setPoint( i+1, Point(outerRadius*cos(dA*i + origin), outerRadius*sin(dA*i + origin), 0) );
                else
                    setPoint( i+1, Point(innerRadius*cos(dA*i + origin), innerRadius*sin(dA*i + origin), 0) );
            }
            setPoint( 2*nPoints+1, Point( 0.f, -outerRadius, 0) );
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
    
    
    //------------------------------------------------------------------------
    void StarShape::reshape( float _width, float _height, int nPoints, float depth ) {
        // if the number of points is the same as before, just use the existing point
        if ( nPoints == getNumPoints() ) {
            width = _width;
            height = _height;
            
            float dA = M_2PI / (float) (2 * nPoints);
            float origin = M_2PI * 3/4;
            for( int i=0; i<2*nPoints; i++ ) {
                if( i%2 == 0 )
                    setPoint( i+1, Point( width/2.0*cos(dA*i + origin), height/2.0*sin(dA*i + origin), 0) );
                else
                    setPoint( i+1, Point((width/2.0 - depth)*cos(dA*i + origin), (height/2.0 - depth)*sin(dA*i + origin), 0) );
            }
            setPoint( 2*nPoints+1, Point( 0.f, -height/2.0, 0) );
        }
        // if not, clear the list and make new points
        else {
            clearPoints();
            construct( _width, _height, nPoints, depth );
        }
        
        if(strokeEnabled)
            generateStroke(getStrokeWidth());
    }
} /* End po namespace */
