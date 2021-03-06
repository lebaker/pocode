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
//  poTextBox.cpp
//  pocode
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//


#include "poFBO.h"
#include "poTextBox.h"
#include "poSimpleDrawing.h"
#include "poOpenGLState.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"
#include "poApplication.h"

#include <float.h>
#include <boost/foreach.hpp>

poTextBox::poTextBox()
:	poObject()
,	textColor(poColor::white)
,	layout(poPoint())
,	useTextBounds(false)
,   autoAdjustHeight(false)
,	textAlignment(PO_ALIGN_TOP_LEFT)
,	cacheToTexture(false)
,	cached(NULL)
,   fillEnabled(false)
,   strokeWidth(0)
{
	setFont(poFont::defaultFont());
}

poTextBox::poTextBox(int w) 
:	poObject()
,	textColor(poColor::white)
,	layout(poPoint(w,0))
,	useTextBounds(false)
,   autoAdjustHeight(true)
,	textAlignment(PO_ALIGN_TOP_LEFT)
,	cacheToTexture(false)
,	cached(NULL)
,   fillEnabled(false)
,   strokeWidth(0)
{
    layout.size = poPoint( w,0 );
	setFont(poFont::defaultFont());
}

poTextBox::poTextBox(int w, int h) 
:	poObject()
,	textColor(poColor::white)
,	layout(poPoint(w,h))
,	useTextBounds(false)
,   autoAdjustHeight(false)
,	textAlignment(PO_ALIGN_TOP_LEFT)
,	cacheToTexture(false)
,	cached(NULL)
,   fillEnabled(false)
,   strokeWidth(0)
{
    layout.size = poPoint( w,h );
	setFont(poFont::defaultFont());
}

poObject* poTextBox::copy() {
	poTextBox *tb = new poTextBox();
	clone(tb);
	return tb;
}

void poTextBox::clone(poTextBox *tb) {
	tb->fillColor = fillColor;
	tb->fillEnabled = fillEnabled;
	tb->strokeColor = strokeColor;
	tb->strokeWidth = strokeWidth;
	tb->textColor = textColor;
	tb->useTextBounds = useTextBounds;
	tb->cacheToTexture = cacheToTexture;
	tb->layout = layout;
	tb->cached = cached;
	poObject::clone(tb);
}

poTextBox::~poTextBox() {
    if(cached) delete cached; cached = NULL;
}

std::string poTextBox::getText() const {
	return layout.text;
}

void poTextBox::setText(const std::string &str) {
	layout.text = str;
}

uint poTextBox::getTextSize() const {
	return layout.textSize;
}

void poTextBox::setTextSize(uint ptSize) {
	layout.textSize = ptSize;
}

poAlignment poTextBox::getTextAlignment() const {
	return textAlignment;
}

void poTextBox::setTextAlignment(poAlignment align) {
	textAlignment = align;
}

poRect poTextBox::getTextBounds() const {
	return layout.getTextBounds();
}

poRect      poTextBox::getBounds()
{
    if ( useTextBounds )
    {
        poRect R = getTextBounds();
        R.x -= getPaddingLeft();
        R.width += getPaddingLeft() + getPaddingRight();
        R.y -= getPaddingTop();
        R.height += getPaddingTop() + getPaddingBottom();
        return R;
    }
    else
    {
        if ( autoAdjustHeight )
            return poRect( 0, 0, layout.size.x,  getTextBounds().height+getPaddingTop()+getPaddingBottom() );
        else
            return poRect( poPoint(0,0), layout.size );
    }
}

void poTextBox::reshape(int w, int h) {
	layout.size = poPoint(w,h);
}

void poTextBox::reshape(poRect r) {
	layout.size = r.getSize();
}

bool poTextBox::isRichText() const {
	return layout.isRichText;
}

void poTextBox::setRichText(bool b) {
	layout.isRichText = b;
}

void poTextBox::reshape(poPoint p) {
    reshape(p.x,p.y);
}

uint poTextBox::getNumLines() const {
	return layout.getNumLines();
}

uint poTextBox::getNumWordsForLine(uint lineNum) {
	return layout.getLine(lineNum).wordCount;
}

poRect poTextBox::boundsForLine(uint num) const {
	return layout.getBoundsForLine(num);
}

int     poTextBox::numLettersForLine( int lineIndex ) {
    return layout.getNumGlyphsForLine(lineIndex);
}

poRect  poTextBox::getBoundsForLetterOnLine( int letterIndex, int lineIndex ) {
    return layout.getBoundsForGlyphOnLine( letterIndex, lineIndex );
}

void    poTextBox::setBoundsForLetterOnLine( int letterIndex, int lineIndex, poRect newBounds ) {
	layout.lines[lineIndex].glyphs[letterIndex].bbox = newBounds;
}


void poTextBox::setLineOffset(uint lineNum, poPoint offset)
{    
	layout.shiftLine(lineNum, offset);
}

bool poTextBox::getCacheToTexture() const {
	return cacheToTexture;
}

void poTextBox::setCacheToTexture(bool b) {
	cacheToTexture = b;
    if(!cacheToTexture && cached) delete cached; cached = NULL;
}
void poTextBox::useTextBoundsAsBounds( bool B ) { 
	useTextBounds = B; layout.setUseTextBounds(B); 
};
void poTextBox::useAutoAdjustHeight( bool B ) { 
	autoAdjustHeight = B; 
};

float poTextBox::getLeading() const {
	return layout.leading;}

void poTextBox::setLeading(float f) {
	layout.leading = f; }

float poTextBox::getTracking() const {
	return layout.tracking;}

void poTextBox::setTracking(float f) {
	layout.tracking = f; }

float poTextBox::getPaddingLeft() const {
	return layout.padding[po::TextBoxLayout::PADDING_LEFT];
}

float poTextBox::getPaddingRight() const {
	return layout.padding[po::TextBoxLayout::PADDING_RIGHT];
}

float poTextBox::getPaddingTop() const {
	return layout.padding[po::TextBoxLayout::PADDING_TOP];
}

float poTextBox::getPaddingBottom() const {
	return layout.padding[po::TextBoxLayout::PADDING_BOTTOM];
}

void poTextBox::setPadding(float f) {
	layout.padding[0] = layout.padding[1] = layout.padding[2] = layout.padding[3] = f; 
}

void poTextBox::setPadding(float h, float v) {
	layout.padding[0] = layout.padding[1] = h;
	layout.padding[2] = layout.padding[3] = v;
}

void poTextBox::setPadding(float l, float r, float t, float b) {
	layout.padding[0] = l;
	layout.padding[1] = r;
	layout.padding[2] = t;
	layout.padding[3] = b;
}

void poTextBox::setTabWidth(int tw) {
	layout.tabWidth = tw;
}

int poTextBox::getTabWidth() const {
	return layout.tabWidth;
}

void poTextBox::setFont(poFont *f, const std::string &name) {
	layout.setFont(f,name);
}

poFont* poTextBox::getFont(const std::string &name) {
	return layout.getFont(name);
}

void poTextBox::doLayout() {
	layout.layout();
	
    layout.alignment = getTextAlignment();
	layout.realignText();
    setAlignment(getAlignment());
	
	if(cacheToTexture) {
        generateCachedTexture();
	}
    
    layoutDone = true;
}


void poTextBox::generateCachedTexture() {
	if(cached) {
		delete cached;
		cached = NULL;
	}
    
    poRect bounds = getBounds();
    bounds.include(getTextBounds());
    
    poFBO *fbo = new poFBO(bounds.width * poGetScale(), bounds.height * poGetScale(), poFBOConfig());
    fbo->setUp(this);
    
    // http://stackoverflow.com/questions/2171085/opengl-blending-with-previous-contents-of-framebuffer
	po::saveBlendState();
	po::enableBlendWithFunc(GL_SRC_COLOR, GL_ZERO, GL_ONE, GL_ONE);
    
    poBitmapFont *bmp = poGetBitmapFont(getFont(), layout.textSize);
    
    po::setColor(poColor::white);
    for(uint i=0; i<getNumLines(); i++) {
        BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.lines[i].glyphs) {
            bmp->drawGlyph( glyph.glyph, glyph.bbox.getPosition()); 
        }
    }
    
	po::restoreBlendState();
    
    fbo->setDown(this);
    //	cached = fbo->getColorTexture()->copy();
    cached = fbo->getColorTextureCopy();
    delete fbo;
}


void poTextBox::draw() {
	using namespace std;
	
	if(cached && cached->isValid()) {
		po::saveBlendState();
		po::enableAlphaBlending();
		
		po::setColor(textColor, getAppliedAlpha());
		po::drawTexturedRect(cached, poRect(0,0,cached->getWidth()/poGetScale(), cached->getHeight()/poGetScale()));
		
		po::restoreBlendState();
		return;
	}
	
    // draw fill and stroke
    if ( fillEnabled )
    {
        po::setColor( fillColor, getAppliedAlpha() );
        //po::drawFilledRect( 0, 0, layout.size.x, layout.size.y );
        po::drawFilledRect( getBounds() );
    }
    if ( strokeWidth > 0 )
    {
        glLineWidth( strokeWidth );
        po::setColor( strokeColor, getAppliedAlpha() );
        po::drawStrokedRect( 0, 0, layout.size.x, layout.size.y );
    }
    
	poBitmapFont *bitmapFont = poGetBitmapFont(getFont(), layout.textSize);
	
    if ( layout.isRichText ) {
		int count = 0;
		for(int i=0; i<getNumLines(); i++) {
			BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.getLine(i).glyphs) {
				po::setColor(poColor(textColor, getAppliedAlpha()));
				
				poDictionary dict = layout.getTextPropsAtIndex(count);
				count++;
				
				// see if the user wants anything special
				if(dict.has("color"))
					po::setColor(poColor(dict.getColor("color"), getAppliedAlpha()));
				
				// a new font, perhaps?
				if(dict.has("font")) {
					poFont *theFont = (poFont*)dict.getPtr("font");
					int fontSize = dict.has("fontSize") ? dict.getInt("fontSize") : layout.textSize;
					poBitmapFont* newFont = poGetBitmapFont(theFont, fontSize);
					
					if(newFont != bitmapFont) {
						bitmapFont = newFont;
					}
				}
				else if(bitmapFont->getFont() != layout.getFont()) {
					bitmapFont = poGetBitmapFont(layout.getFont(), layout.textSize);
				}
				
				// very well, now draw it
				bitmapFont->drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
			}
		}
    }
    else {
		po::setColor( poColor(textColor, getAppliedAlpha()) );
		
        for(uint i=0; i<getNumLines(); i++) {
            BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.lines[i].glyphs) {
                bitmapFont->drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
            }
        }
    }
}


void poTextBox::_drawBounds() {
    for(uint i=0; i<getNumLines(); i++) {
        if(drawBounds & PO_TEXT_BOX_STROKE_GLYPHS) {
            po::setColor(poColor::ltGrey, .5f);
            BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.getLine(i).glyphs) {
                po::drawStrokedRect(glyph.bbox);
            }
        }
        
        if(drawBounds & PO_TEXT_BOX_STROKE_LINES) {
            po::setColor(poColor::white, .6f);
            po::drawStrokedRect(boundsForLine(i));
        }
    }
    
    if(drawBounds & PO_TEXT_BOX_STROKE_TEXT_BOUNDS) {
        po::setColor(poColor::grey, .7f);
        po::drawStrokedRect(getTextBounds());
    }
    
    poObject::_drawBounds();
    
    /*po::setColor(poColor::dkGrey, .8f);
     po::drawStrokedRect(getBounds());
     
     po::setColor(poColor::red);
     po::drawFilledRect(poRect(-offset-poPoint(5,5), poPoint(10,10)));*/
}


bool poTextBox::pointInside(poPoint p, bool localize)
{	
    if(!visible)
		return false;
	
    if(localize) {
        p.y = getWindowHeight() - p.y;
        p = globalToLocal(p);
    }
        
    poRect bounds = getBounds();

	// DO POINT INSIDE TEST FOR 2D
    if ( poCamera::getCurrentCameraType() == PO_CAMERA_2D ) {
        return bounds.contains(p.x, p.y);
	}
	else if(poCamera::getCurrentCameraType() == PO_CAMERA_3D) {
        return pointInRect3D( p, getMatrixSet(), bounds );
	}

	return false;
}


int poTextBox::getSizeInMemory() {
    int S = sizeof(poTextBox);
    
    S += layout.getSizeInMemory();
    
    //if ( cached )
    //    S += cached->getSizeInMemory();
    
    return S;
}

