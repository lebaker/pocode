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

#define _USE_MATH_DEFINES
#include <math.h>
#include <ctime>
#include <algorithm>

#include "poPoint.h"
#include "poRect.h"
#include "poColor.h"
#include "poMatrixSet.h"

inline bool compare(float f1, float f2, float range=1.0e-6f) {
	return ::fabs(f1-f2) < range;
}

inline float deg2rad(float deg) {
	return deg / 180.f * M_PI;
}

inline float rad2deg(float rad) {
	return rad / M_PI * 180.f;
}

inline float sin_deg(float deg) {
	return ::sinf(deg2rad(deg));
}

inline float cos_deg(float deg) {
	return ::cosf(deg2rad(deg));
}

inline void poRandSeed() {
	::srand((unsigned int)::time(NULL));
}

inline void poRandSeed(int seed) {
	::srand(seed);
}

inline float poRand() {
	return rand() / (float)RAND_MAX;
}

inline float poRand(float low, float high) {
	return poRand() * (high-low) + low;
}

inline poPoint randomPointInRect(poRect r) {
	return poPoint(poRand(r.x,r.x+r.width),
				   poRand(r.y,r.y+r.height));
}

inline poColor randomColor(float alpha=1.f) {
	return poColor(poRand(), poRand(), poRand(), alpha);
}

inline void capToRange(float *f, float low, float high) {
	*f = std::max(*f, low);
	*f = std::min(*f, high);
}

inline void capToRange(poPoint *pt, float low, float high) {
	capToRange(&pt->x, low, high);
	capToRange(&pt->y, low, high);
	capToRange(&pt->z, low, high);
}

// will round -.5 to -1
inline float roundSym(float f) {
	return (f > 0.f) ? floor(f + 0.5f) : ceil(f - 0.5f);
}

inline poPoint roundSym(poPoint p) {
	return poPoint(roundSym(p.x), roundSym(p.y), roundSym(p.z));
}

inline float round(float f) {
	return floor(f + 0.5f);
}

inline poPoint round(poPoint p) {
	return poPoint(round(p.x), round(p.y), round(p.z));
}

inline poPoint floor(poPoint p) {
	return poPoint(floor(p.x), floor(p.y), floor(p.z));
}

template <typename T>
inline T poClamp(T min, T max, T val) {
	return std::max(std::min(val, max), min);
}

inline float poMapf(float min1, float max1, float v, float min2, float max2) {
	float tmp = (v-min1) / (max1-min1);
	return tmp * (max2-min2) + min2;
}

inline float poDist(float x1, float y1, float z1, float x2, float y2, float z2) {
    return sqrtf(powf((x2-x1), 2) + powf((y2-y1), 2) + powf((z2-z1), 2));
}

inline float poDist(poPoint p1, poPoint p2) {
    return poDist(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

struct poRay {
	poPoint origin, dir;
	poRay(poPoint o, poPoint d)
	: origin(o), dir(normalize(d)) {}
};

// angle ABC
float angleBetweenPoints(poPoint a, poPoint b, poPoint c);
// returns false if parallel, p1 = intersection or p1 and p2 = closest point on each
bool rayIntersection(poRay r1, poRay r2, poPoint *p1, poPoint *p2);

bool pointInTriangle( poPoint &P, poPoint &A, poPoint &B, poPoint &C );
bool pointInTriangle3D( poPoint P, poMatrixSet& M, poPoint A, poPoint B, poPoint C );
bool pointInRect3D( poPoint P, poMatrixSet& M, poRect R );

poPoint projectOntoLine(poPoint p, poPoint a, poPoint b);
float distanceToLine(poPoint p, poPoint a, poPoint b);



