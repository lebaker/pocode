//
//  poFont.h
//  poFramework4
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poRect.h"
#include "poImage.h"
#include "poTexture.h"
#include "poResource.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class poShape2D;

static const std::string PO_FONT_REGULAR = "span";
static const std::string PO_FONT_ITALIC = "i";
static const std::string PO_FONT_BOLD = "b";

class poFont;
typedef std::map<std::string, poFont*> poFontMap;

class poFont
:	public poResource
{
public:
	// pass in a family name or a font url
	poFont(const std::string &family_or_url, int pointSize, const std::string &trait=PO_FONT_REGULAR);
	virtual ~poFont();
	
	bool valid() const;
	poFont *copy() const;
	
	std::string familyName() const;
	std::string styleName() const;
	std::string url() const;
	bool hasKerning() const;
	
	int pointSize() const;
	void pointSize(int size);

	float lineHeight() const;
	float ascender() const;
	float descender() const;
	float underlinePosition() const;
	float underlineThickness() const;

	int glyph() const;
	void glyph(int g);
	// uses the previously set glyph
	poRect glyphBounds();
	poPoint glyphBearing();
	poPoint glyphAdvance();
	// you have to delete the results
	poImage *glyphImage();
	poShape2D *glyphOutline();

	poPoint kernGlyphs(int glyph1, int glyph2);
	
	std::string toString() const;

private:
	poFont();
	void init();
	void loadGlyph(int g);
	
	static FT_Library lib;
	boost::shared_ptr<FT_FaceRec_> face;

	std::string _url;
	int size, _glyph;
};

class poBitmapFontAtlas : public poTextureAtlas {
public:
	// if -1, it'll store the current point size of the font
	poBitmapFontAtlas(poFont *font, int pointSize=-1);
	virtual ~poBitmapFontAtlas();
	
	void cacheGlyph(uint glyph);
	// you can't change the font from here
	poFont const *font();
	
private:
	int size;
	poFont *_font;
};

class BitmapFontCache {
public:
	poBitmapFontAtlas *atlasForFont(poFont *font);
	void releaseAtlasForFont(poFont *font);
	
private:
	static boost::unordered_map<poFont,poBitmapFontAtlas*> atlases;
};

std::ostream &operator<<(std::ostream &o, const poFont &f);
