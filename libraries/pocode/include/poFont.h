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
//  poFont.h
//  pocode
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once


#include "poRect.h"
#include "poImage.h"
#include "poResourceStore.h"

class poShape2D;
struct FT_LibraryRec_;
struct FT_FaceRec_;

typedef FT_FaceRec_* FT_Face;
typedef FT_LibraryRec_* FT_Library;

// CLASS NOTES
//
// A poFont implements general font functionality. A single poFont object represents a single
// font face in a single font size.
//
// You cannot construct a new font directly. You can load a new font as follows:
//
//      poFont* font = getFont("Courier", 20);
//
// Using getFont() ensures that the same font is not loaded and constructed multiple times.
//
// 

struct poFontGlyphMetrics
{
    poRect  glyphBounds;
	poRect  glyphFrame;
	float   glyphDescender;
	poPoint glyphBearing;
	poPoint glyphAdvance;
};

typedef std::vector<poFontGlyphMetrics> glyphMetricsVector;

// http://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#FT_Encoding
unsigned long encodeTag(const char tag[4]);
std::string decodeTag(unsigned long encoded);

bool urlForFontFamilyName(const std::string &family, const std::string &style, poFilePath &response);

class poFont : public poResource
{
	friend std::ostream &operator<<(std::ostream &o, const poFont *f);

public:
	static bool fontExists(const std::string &family_or_url);
	static poFont *defaultFont();
	
	poFont();
	poFont(const poFilePath &filePath, const std::string &style="", unsigned long encoding=encodeTag("unic"));
	virtual ~poFont();
	
    // FONT LOADING
	bool                isValid() const;
	
    // FONT PROPERTIES
	std::string         getFamilyName() const;
	std::string         getStyleName() const;
	poFilePath          getFilePath() const;
	bool                hasKerning() const;
	std::vector<std::string>
						getEncodings() const;
	
	int                 getPointSize() const;
	void                setPointSize(int size);

	float               getLineHeight() const;
	float               getAscender() const;
	float               getDescender() const;
    
    // UNDERLINE
	// maximum bbox for this font face at this size
	float               getUnderlinePosition() const;
	float               getUnderlineThickness() const;

    // CURRENT GLYPH
    // Get and set the current glyph.
	int                 getGlyph() const;
	void                setGlyph(int g);
    
	// These functions (starting with 'glyph') return info about the current codepoint.
	poRect              getGlyphBounds();
	poRect              getGlyphFrame();
	float               getGlyphDescender();
	poPoint             getGlyphBearing();
	poPoint             getGlyphAdvance();
	poImage*			getGlyphImage();
//	poShape2D           *getGlyphOutline() const;

	poPoint             kernGlyphs(int glyph1, int glyph2) const;

	std::string         toString() const;
	std::string			getRequestedFamilyName() const;
	std::string			getRequestedStyleName() const;

    bool                cachedForSizeYet(int fontSize);
    void                cacheGlyphMetrics();

private:
	void                loadGlyph(int g);
    
    glyphMetricsVector                  *currentCache;
    std::map<int,glyphMetricsVector>    cachedGlyphMetricsSet;
    
    poFilePath          filePath;
	std::string			reqFamily, reqStyle;
	int					size;
	int					glyph, loadedGlyph;
	FT_Face				face;
	static FT_Library   lib;
};

