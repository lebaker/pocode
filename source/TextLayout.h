//
//  TextLayout.h
//  poFramework4
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poRect.h"
#include "poFont.h"
#include "AttributedString.h"

struct layout_glyph {
	layout_glyph() : glyph(0), bbox() {}
	uint glyph;
	poRect bbox;
};

struct layout_line {
	layout_line() : word_count(0) {}
	std::vector<layout_glyph> glyphs;
	poRect bounds;
	int word_count;
};

class TextLayout {
public:
	std::string text() const;
	void text(const std::string &str);
	
	void font(poFont *f, const std::string &weight=PO_FONT_REGULAR);
	void font(const poFont &f, const std::string &weight=PO_FONT_REGULAR);
	poFont *const font(const std::string &weight=PO_FONT_REGULAR);
	bool hasFont(const std::string &weight=PO_FONT_REGULAR);
	
	virtual void layout();
	
	uint numLines() const;
	layout_line getLine(uint i) const;

	poRect textBounds() const;

protected:
	virtual void doLayout() = 0;
	po::AttributedString &parsedText();
	void addLine(const layout_line &line);
	void replaceLine(int i, const layout_line &line);
	void recalculateTextBounds();
	
private:
	// 1. strip html
	// 2. store ranges
	// 3. build attributed str
	void prepareText();
	
	poFontMap fonts;
	std::string _text;
	po::AttributedString _parsed;
	std::vector<layout_line> lines;
	poRect text_bounds;
};

class TextBoxLayout : public TextLayout {
public:
	TextBoxLayout(poPoint s);
	
	poPoint size() const;
	void size(poPoint s);

	float tracking() const;
	void tracking(float f);
	
	float leading() const;
	void leading(float f);
	
	float paddingLeft() const;
	float paddingRight() const;
	float paddingTop() const;
	float paddingBottom() const;
	void padding(float f);
	void padding(float h, float v);
	void padding(float l, float r, float t, float b);
	
	poAlignment alignment() const;
	void alignment(poAlignment a);
	
protected:
	virtual void doLayout();
	
private:
	void addGlyphsToLine(std::vector<layout_glyph> &glpyhs, poPoint &size, layout_line &line);
	void breakLine(layout_line &line);
	void alignText();

	poPoint _size;
	float _tracking;
	float _leading;
	float _padding[4];
	poAlignment _alignment;
};
