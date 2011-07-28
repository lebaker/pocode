//
//  poImage.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 4/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poImage.h"
#include "poTexture.h"
#include "poResourceStore.h"
#include <FreeImage.h>

static void loadFreeImageIfNeeded() {
	static bool free_image_loaded = false;
	if(!free_image_loaded) {
		FreeImage_Initialise();
		free_image_loaded = true;
	}
}

poImage::poImage() 
:	bitmap(NULL)
,	tex(NULL)
{
	loadFreeImageIfNeeded();
}

poImage::poImage(const std::string &url)
:	bitmap(NULL)
,	tex(NULL)
{
	loadFreeImageIfNeeded();
	load(url);
}

poImage::poImage(const std::string &url, ImageBitDepth bpp)
:	bitmap(NULL)
,	tex(NULL)
{
	loadFreeImageIfNeeded();
	load(url, bpp);
}

poImage::poImage(uint w, uint h, ImageBitDepth bpp, const ubyte *p) 
:	bitmap(NULL)
,	tex(NULL)
{
	loadFreeImageIfNeeded();
	load(w, h, bpp, p);
}

poImage::poImage(const poImage &img)
:	bitmap(NULL)
,	tex(NULL)
{
	if(img.isValid())
		bitmap = FreeImage_Clone(img.bitmap);
}

poImage::~poImage() {
	FreeImage_Unload(bitmap);
	
	if(tex) {
		tex->image = NULL;
	}
}

poImage *poImage::copy() {
	if(!isValid())
		return getImage();
	
	poImage *response = new poImage();
	response->bitmap = FreeImage_Clone(bitmap);
	return response;
}

bool poImage::isValid() const {
	return bitmap != NULL;
}

uint poImage::width() const {
	return FreeImage_GetWidth(bitmap);
}

uint poImage::height() const {
	return FreeImage_GetHeight(bitmap);
}

poPoint poImage::dimensions() const {
	return poPoint(width(), height());
}

ImageBitDepth poImage::bpp() const {
	return (ImageBitDepth)FreeImage_GetBPP(bitmap);
}

uint poImage::pitch() const {
	return FreeImage_GetPitch(bitmap);
}

uint poImage::storageSize() const {
	return pitch() * height();
}

ubyte const*poImage::pixels() const {
	return FreeImage_GetBits(bitmap);
}

poColor poImage::getPixel(poPoint p) const {
	if(!isValid() || p.x < 0 || p.y < 0 || p.x >= width() || p.y >=height())
		return poColor();

	uint x = p.x;
	uint y = p.y;
	
	BYTE* bits = FreeImage_GetScanLine(bitmap, y);
	poColor ret;
	
	switch(bpp()) {
		case IMAGE_8:
			ret.set255(bits[x], bits[x], bits[x], 255);
			break;
		case IMAGE_16:
			ret.set255(bits[x*2], bits[x*2], bits[x*2], bits[x*2+1]);
			break;
		case IMAGE_24:
			ret.set255(bits[x*3+0], bits[x*3+1], bits[x*3+2], 255);
			break;
		case IMAGE_32:
			ret.set255(bits[x*4+0], bits[x*4+1], bits[x*4+2], bits[x*4+3]);
			break;
	}
	
	return ret;
}

void poImage::setPixel(poPoint p, poColor c) {
	if(p.x < 0 || p.y < 0 || p.x >= width() || p.y >=height())
		return;
	
	uint x = p.x;
	uint y = p.y;
	
	BYTE *bits = FreeImage_GetScanLine(bitmap, y);
	switch(bpp()) {
		case IMAGE_8:
			bits[x] = (((0.21*c.R) + (0.71*c.G) + (0.07*c.B)) * c.A) * 255;
			break;
		case IMAGE_16:
			bits[x*2] = ((0.21*c.R) + (0.71*c.G) + (0.07*c.B)) * 255;
			bits[x*2+1] = c.A * 255;
			break;
		case IMAGE_24:
			bits[x*3] = c.R * c.A * 255;
			bits[x*3+1] = c.G * c.A * 255;
			bits[x*3+2] = c.B * c.A * 255;
			break;
		case IMAGE_32:
			bits[x*4] = c.R * 255;
			bits[x*4+1] = c.G * 255;
			bits[x*4+2] = c.B * 255;
			bits[x*4+3] = c.A * 255;
			break;
	}
}

void poImage::setPixel(poPoint p, poColor c, int stamp_width) {
	for(int y=-stamp_width/2; y<stamp_width/2; y++)
		for(int x=-stamp_width/2; x<stamp_width/2; x++) {
			setPixel(p + poPoint(x,y), c);
		}
}

void poImage::changeBpp(ImageBitDepth bpp) {
	if(bitmap && FreeImage_GetBPP(bitmap) != bpp) {
		FIBITMAP *dst = NULL;
		switch(bpp) {
			case IMAGE_8:	
				dst = FreeImage_ConvertTo8Bits(bitmap); 
				break;
			case IMAGE_16:	
				dst = FreeImage_ConvertTo16Bits565(bitmap); 
				break;
			case IMAGE_24:	
				dst = FreeImage_ConvertTo24Bits(bitmap); 
				break;
			case IMAGE_32:	
				dst = FreeImage_ConvertTo32Bits(bitmap); 
				break;
		}
		FreeImage_Unload(bitmap);
		bitmap = dst;
	}
}

void poImage::composite(poImage *img, poRect into) {
	FreeImage_Paste(bitmap, img->bitmap, into.origin.x, into.origin.y, 256);
}

poColor operator*(const poColor& c, float f) {
	poColor response(c);
	response.R *= f;
	response.G *= f;
	response.B *= f;
	response.A *= f;
	return response;
}

poColor operator/(const poColor& c, float f) {
	poColor response(c);
	response.R /= f;
	response.G /= f;
	response.B /= f;
	response.A /= f;
	return response;
}

poColor operator+(const poColor &a, const poColor &b) {
	poColor response(a);
	response.R = a.R + b.R;
	response.G = a.G + b.G;
	response.B = a.B + b.B;
	response.A = a.A + b.A;
	return response;
}

void poImage::blur(int kernel_size, float sig) {
	float e = 2.718281828f;
	int hk = kernel_size / 2;
	
	float *kernel = new float[kernel_size];
	for(int i=0; i<kernel_size; i++) {
		float contrib = powf(i - hk, 2) / (2 * sig*sig);
		kernel[i] = powf(e, -contrib);
	}
	
	poImage img(*this);
	
	for(int y=0; y<height(); y++)
		for(int x=0; x<width(); x++) {
			poColor sum;
			float k_tot = 0;
			
			for(int i=-hk; i<hk; i++) {
				if(x+i < 0 || x+i >= width())
					continue;
				
				float k = kernel[i+hk];
				k_tot += k;
				
				poColor c = getPixel(poPoint(x+i,y));
				
				sum = sum + (c*k);
			}
			
			img.setPixel(poPoint(x,y), sum/k_tot);
		}
	
	for(int y=0; y<height(); y++)
		for(int x=0; x<width(); x++) {
			poColor sum;
			float k_tot = 0;
			
			for(int i=-hk; i<hk; i++) {
				if(y+i < 0 || y+i >= height())
					continue;
				
				float k = kernel[i+hk];
				k_tot += k;
				
				poColor c = img.getPixel(poPoint(x,y+i));
				
				sum = sum + (c*k);
			}
			
			setPixel(poPoint(x,y), sum/k_tot);
		}
	
	delete [] kernel;
}

void poImage::flip(poOrientation dir) {
	if(dir == PO_VERTICAL)
		FreeImage_FlipVertical(bitmap);
	else
		FreeImage_FlipHorizontal(bitmap);
}

void poImage::resizeMaxDimension(float max_dim) {
	int newW = width();
	int newH = height();
	
	if(newW > newH)
		resizeWidth(max_dim);
	else
		resizeHeight(max_dim);
}

void poImage::resizeMinDimension(float min_dim) {
	int newW = width();
	int newH = height();
	
	if(newW < newH)
		resizeWidth(min_dim);
	else
		resizeHeight(min_dim);
}

void poImage::resizeWidth(float w) {
	float aspect = height() / (float)width();
	resize(w, w * aspect);
}

void poImage::resizeHeight(float h) {
	float aspect = width() / (float)height();
	resize(h * aspect, h);
}

void poImage::resize(float w, float h) {
	FIBITMAP *img = FreeImage_Rescale(bitmap, w, h, FILTER_CATMULLROM);
	FreeImage_Unload(bitmap);
	bitmap = img;
}

poTexture *poImage::texture() {
	if(!tex)
		tex = new poTexture(this);
	return tex;
}

poTexture *poImage::texture(poTextureConfig config) {
	if(!tex)
		tex = new poTexture(this,config);
	return tex;
}

void poImage::clear() {
	ubyte color[] = {0,0,0,0};
	FreeImage_FillBackground(bitmap, &color[0]);
}

FIBITMAP *loadDIB(const std::string &url) {
	// figure out if the file exists
	
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(url.c_str());
	if(fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(url.c_str());
	if(fif == FIF_UNKNOWN) {
		printf("poImage: image isn't a supported file type (%s)\n", url.c_str());
		return NULL;
	}
	
	FIBITMAP *dib = NULL;
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, url.c_str());
	if(!dib) {
		printf("poImage: image file not found (%s)\n", url.c_str());
		return NULL;
	}

	unsigned bpp = FreeImage_GetBPP(dib);
	if(bpp == 24 || bpp == 32) {
		// there has got to be a more efficient way of doing this
		FIBITMAP *red = FreeImage_GetChannel(dib,FICC_RED);
		FIBITMAP *blue = FreeImage_GetChannel(dib,FICC_BLUE);
		
		FreeImage_SetChannel(dib,red,FICC_BLUE);
		FreeImage_SetChannel(dib,blue,FICC_RED);
		
		FreeImage_Unload(red);
		FreeImage_Unload(blue);
	}
	
	FreeImage_FlipVertical(dib);
	return dib;
}

void poImage::load(const std::string &url) {
	bitmap = loadDIB(url);
}

void poImage::load(const std::string &url, ImageBitDepth bpp) {
	bitmap = loadDIB(url);
	changeBpp(bpp);
}

void poImage::load(uint w, uint h, ImageBitDepth bpp, const ubyte *pix) {
	if(pix != NULL)
		bitmap = FreeImage_ConvertFromRawBits(const_cast<ubyte*>(pix), w, h, w*(bpp/8), bpp, 0,0,0);
	else {
		bitmap = FreeImage_Allocate(w, h, bpp);
		char black[] = {0,0,0,0};
		FreeImage_FillBackground(bitmap, black);
	}
	
}

void writeImageToCHeader(const std::string &str, poImage *img) {
	std::string filename = str + "Image.cpp";
	FILE *f = fopen(filename.c_str(), "w+");
	fprintf(f,
			"\n#include \"%sImage.h\"\n\n"
			"static const uint width = %d;\n"
			"static const uint height = %d;\n"
			"static const uint format = %d;\n"
			"static const ubyte bytes[] = {\n",
			str.c_str(), img->width(), img->height(), (int)img->bpp());
	for(int h=0; h<img->height(); h++) {
		fprintf(f,"\t");
		for(int w=0; w<img->width(); w++) {
			poColor color = img->getPixel(poPoint(w,h));
			fprintf(f,"0x%X,0x%X,0x%X,", uint(color.R*255), uint(color.G*255), uint(color.B*255));
		}
		if(h == img->height()-1)
			fseek(f, -1, SEEK_CUR);
		fprintf(f,"\n");
	}
	fprintf(f,
			"};\n"
			"poImage *get%sImage() {\n"
			"	static poImage *img = NULL;\n"
			"	if(!img) {\n"
			"		img = new poImage(width,height,(ImageBitDepth)format,&bytes[0]);\n"
			"	}\n"
			"	return img->copy();\n"
			"}\n\n",
			str.c_str());
	fclose(f);
	
	filename = str + "Image.h";
	f = fopen(filename.c_str(), "w");
	fprintf(f,
			"\n#pragma once\n\n"
			"#include \"poImage.h\"\n\n"
			"poImage *get%sImage();\n",
			str.c_str());
	fclose(f);
}

