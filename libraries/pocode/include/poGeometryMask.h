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

#pragma once


//#include "poImage.h"
#include "poShape2D.h"

// CLASS NOTES
//
// poGeometryMask implements a mask based upon a poShape2D object. A geometry mask is used as follows:
//
//      poRectShape* objectToMask = new poRectShape("myImage.jpg");
//      addChild( objectToMask );
//
//      poOvalShape* maskShape = new poOvalShape( 100,100 );
//      maskShape->position( objectToMask->bounds.width()/2, objectToMask->bounds.height()/2, 0 );
//
//      poGeometryMask* geoMask = new poGeometryMask( maskShape );
//      objectToMask->addModifer( geoMask );
//
// Note that the maskShape can have a position. It can also be scaled and rotated.
// The origin of the objectToMask serves as the origin of the maskShape.
// The maskShape should NOT be added to the scene graph.
//

class poGeometryMask : public poObjectModifier {
public:
	poGeometryMask(poShape2D *shape, bool clearsStencil=false, bool inverse=false);
	virtual			~poGeometryMask();
	
	virtual			poObjectModifier *copy();
	
	void            setShape(poShape2D *shape);	
	// this isn't implemented yet
	bool            pointInside(poPoint p);
	
protected:
	void			doSetUp(poObject*);
	void			doSetDown(poObject*);
	
private:
	poShape2D*		shape;
	bool			clearsStencil;
    bool            inverse;
};

/////////////////////////////////////////////////////
//
//	poImageMask is UNDER DEVELOPMENT
//	Check again in future releases of pocode
//
/////////////////////////////////////////////////////

/*
// CLASS NOTES
//
// poImageMask implements a mask based upon a poImage object. A geometry mask is used as follows:
//
//      poRectShape* objectToMask = new poRectShape("myImage.jpg");
//      addChild( objectToMask );
//
//      poImageMask* imageMask = new poImageMask( getImage("myMask.png") );
//      objectToMask->addModifer( imageMask );
//
// The mask image should be a single channel grayscale image. The black areas in the mask
// is where the masked image will be visible.

class poImageMask :	public poBlendState
{
public:
	poImageMask(poImage *img);
	poImageMask(const std::string &str);
	
	bool            pointInside(poPoint p);
	
protected:	
	virtual void    doSetUp( poObject* obj );
	virtual void    doSetDown( poObject* obj );
	
private:
	poObject*       my_obj;
	poImage*        image;
};
*/
