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

#include "poMath.h"
#include <cmath>

// FILE NOTES
//
// This file contains all of the tweening functions used by poTween. In general, you don't need to call
// these methods directly. Instead, just pass them into poTween's setTweenFunction().
//
// All of the tween functions have the same signature (i.e. take the same arguments in the same order.)
//
// The arguments are used as follows:
//
//	1. v	= ptr to the float value to be modified
//	2. bv	= start value
//	3. ev	= end value
//	4. t	= time right now
//	5. b    = beginning time
//	6. e    = ending time
//	7. d    = duration
//	8. e1   = extra value for specific easing functions
//	9. e2   = extra value for specific easing functions
//
//	All of the tween function return true if the tween is completed.
//  Otherwise, they return false.
//
//	http://code.google.com/p/kitchensynclib/wiki/Easing
//	+
//	Robert Penner Functions
//	@author modified by Jonathan Bobrow, 2011
//	@author modified by Mims H. Wright, 2007
//	@author (c) 2003 Robert Penner, all rights reserved. - This work is subject to the terms in http://www.robertpenner.com/easing_terms_of_use.html
//	@see http://www.robertpenner.com/easing_terms_of_use.html
//	@see http://www.robertpenner.com/easing/
//	@see EasingUtil


#define PO_TWEEN_GOTO_FUNC goToFunc
#define PO_TWEEN_LINEAR_FUNC linearFunc

#define PO_TWEEN_QUAD_IN_FUNC quadInFunc
#define PO_TWEEN_QUAD_OUT_FUNC quadOutFunc
#define PO_TWEEN_QUAD_INOUT_FUNC quadInOutFunc

#define PO_TWEEN_CUBE_IN_FUNC cubeInFunc
#define PO_TWEEN_CUBE_OUT_FUNC cubeOutFunc
#define PO_TWEEN_CUBE_INOUT_FUNC cubeInOutFunc

#define PO_TWEEN_QUART_IN_FUNC quartInFunc
#define PO_TWEEN_QUART_OUT_FUNC quartOutFunc
#define PO_TWEEN_QUART_INOUT_FUNC quartInOutFunc

#define PO_TWEEN_QUINT_IN_FUNC quintInFunc
#define PO_TWEEN_QUINT_OUT_FUNC quintOutFunc
#define PO_TWEEN_QUINT_INOUT_FUNC quintInOutFunc

#define PO_TWEEN_SINUS_IN_FUNC sinusInFunc
#define PO_TWEEN_SINUS_OUT_FUNC sinusOutFunc
#define PO_TWEEN_SINUS_INOUT_FUNC sinusInOutFunc

#define PO_TWEEN_EXPO_IN_FUNC expoInFunc
#define PO_TWEEN_EXPO_OUT_FUNC expoOutFunc
#define PO_TWEEN_EXPO_INOUT_FUNC expoInOutFunc

#define PO_TWEEN_CIRC_IN_FUNC circInFunc
#define PO_TWEEN_CIRC_OUT_FUNC circOutFunc
#define PO_TWEEN_CIRC_INOUT_FUNC circInOutFunc

#define PO_TWEEN_BACK_IN_FUNC backInFunc
#define PO_TWEEN_BACK_OUT_FUNC backOutFunc
#define PO_TWEEN_BACK_INOUT_FUNC backInOutFunc

// to be completed
#define PO_TWEEN_BOUNCE_IN_FUNC bounceInFunc
#define PO_TWEEN_BOUNCE_OUT_FUNC bounceOutFunc
#define PO_TWEEN_BOUNCE_INOUT_FUNC bounceInOutFunc

#define PO_TWEEN_ELASTIC_IN_FUNC elasticInFunc
#define PO_TWEEN_ELASTIC_OUT_FUNC elasticOutFunc
#define PO_TWEEN_ELASTIC_INOUT_FUNC elasticInOutFunc



inline bool goToFunc(float *v, float bv, float ev, 
					 float t, float b, float e, float d,
					 float e1, float e2)
{
	*v = (*v * e1) + (ev * (1-e1));
	return ::fabs(*v - ev) < e2;
}

inline bool linearFunc(float *v, float bv, float ev, 
					   float t, float b, float e, float d,
					   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2 + bv;
	return (t-b) >= d;
}

inline bool quadInFunc(float *v, float bv, float ev, 
					   float t, float b, float e, float d,
					   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2 + bv;
	return (t-b) >= d;
}

inline bool quadOutFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv) * t2*(t2-2) + bv;
	return (t-b) >= d;
}

inline bool quadInOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2) 
{
	float t2 = (t - b) / d * 2.f;
	if(t2 < 1.f) 
		*v = (ev-bv)/2.f * t2*t2 + bv;
	else {
		t2--;
		*v = -(ev-bv)/2.f * (t2*(t2-2.f)-1.f) + bv;
	}
	return (t-b) >= d;
}

inline bool cubeInFunc(float *v, float bv, float ev, 
					   float t, float b, float e, float d,
					   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2*t2 + bv;
	return (t-b) >= d;
}

inline bool cubeOutFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = (ev-bv) * (t2*t2*t2+1.f) + bv;
	return (t-b) >= d;
}

inline bool cubeInOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2) 
{
	float t2 = (t - b) / d * 2.f;
	if(t2 < 1.f)
		*v = (ev-bv)/2.f * (t2*t2*t2) + bv;
	else {
		t2 -= 2.f;
		*v = (ev-bv)/2.f * (t2*t2*t2 + 2.f) + bv;
	}
	return (t-b) >= d;
}

inline bool quartInFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2*t2 + bv;
	return (t-b) >= d;
}

inline bool quartOutFunc(float *v, float bv, float ev, 
						 float t, float b, float e, float d,
						 float e1, float e2) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = -(ev-bv) * (t2*t2*t2*t2-1.f) + bv;
	return (t-b) >= d;
}

inline bool quartInOutFunc(float *v, float bv, float ev, 
						   float t, float b, float e, float d,
						   float e1, float e2) 
{
	float t2 = (t - b) / d * 2.f;
	if(t2 < 1.f)
		*v = (ev-bv)/2.f * t2*t2*t2*t2 + bv;
	else {
		t2 -= 2.f;
		*v = -(ev-bv)/2.f * (t2*t2*t2*t2-2.f) + bv;
	}
	return (t-b) >= d;
}

inline bool quintInFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2*t2*t2*t2*t2 + bv;
	return (t-b) >= d;
}


inline bool quintOutFunc(float *v, float bv, float ev, 
						 float t, float b, float e, float d,
						 float e1, float e2) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = (ev-bv) * (t2*t2*t2*t2*t2+1.f) + bv;
	return (t-b) >= d;
}

inline bool quintInOutFunc(float *v, float bv, float ev, 
						   float t, float b, float e, float d,
						   float e1, float e2) 
{
	float t2 = (t - b) / d * 2.f;
	if(t2 < 1)
		*v = (ev-bv)/2.f * t2*t2*t2*t2*t2 + bv;
	else {
		t2 -= 2.f;
		*v = (ev-bv)/2.f * (t2*t2*t2*t2*t2+2.f) + bv;
	}
	return (t-b) >= d;
}

inline bool sinusInFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv) * ::cosf(t2 * M_PI_2) + (ev-bv) + bv;
	return (t-b) >= d;
}

inline bool sinusOutFunc(float *v, float bv, float ev, 
						 float t, float b, float e, float d,
						 float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * ::sin(t2 * M_PI_2) + bv;
	return (t-b) >= d;
}

inline bool sinusInOutFunc(float *v, float bv, float ev, 
						   float t, float b, float e, float d,
						   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv)/2.f * (::cosf(M_PI*t2) - 1.f) + bv;
	return (t-b) >= d;
}

inline bool expoInFunc(float *v, float bv, float ev, 
					   float t, float b, float e, float d,
					   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * ::powf(2, 10*(t2-1.f)) + bv;
	return (t-b) >= d;
}

inline bool expoOutFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * (-::powf(2, -10*t2) + 1.f) + bv;
	return (t-b) >= d;
}

inline bool expoInOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2) 
{
	float t2 = (t - b) / d * 2.f;
	if(t2 < 1.f)
		*v = (ev-bv)/2.f * ::powf(2.f, 10 * (t2-1.f)) + bv;
	else {
		t2--;
		*v = (ev-bv)/2.f * (-::powf(2, -10*t2) + 2.f) + bv;
	}
	return (t-b) >= d;
}

inline bool circInFunc(float *v, float bv, float ev, 
					   float t, float b, float e, float d,
					   float e1, float e2) 
{
	float t2 = (t - b) / d;
	*v = -(ev-bv) * (::sqrtf(1.f - t2*t2) - 1.f) + bv;
	return (t-b) >= d;
}

inline bool circOutFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2) 
{
	float t2 = (t - b) / d;
	t2--;
	*v = (ev-bv) * ::sqrtf(1.f - t2*t2) + bv;
	return (t-b) >= d;
}

inline bool circInOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2) 
{
	float t2 = (t - b) / d * 2.f;
	if(t2 < 1)
		*v = -(ev-bv)/2.f * (::sqrtf(1.f - t2*t2) - 1.f) + bv;
	else {
		t2 -= 2.f;
		*v = (ev-bv)/2.f * (::sqrtf(1.f - t2*t2) + 1.f) + bv;
	}
	return (t-b) >= d;
}


// -------------- Additional Tweens: Back --------------
// e1: overshoot
// e2:
inline bool backInFunc(float *v, float bv, float ev, 
					   float t, float b, float e, float d,
					   float e1, float e2)
{
	float t2 = (t - b) / d;
	*v = (ev-bv) * t2 * t2 * ((e1+1)*t2 - e1) + bv;
	
	return (t-b) >= d;
}

inline bool backOutFunc(float *v, float bv, float ev, 
						float t, float b, float e, float d,
						float e1, float e2)
{
	float t2 = (t - b) / d;
	t2--;
	*v = (ev-bv) * (t2 * t2 * ((e1+1)*t2 + e1) + 1)  + bv;
	
	return (t-b) >= d;
}

inline bool backInOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2)
{
	float t2 = (t - b) / d * 2.f;
	
	if(t2 < 1)
		*v = (ev-bv)/2.f * t2 * t2 * ((e1+1)*t2 - e1) + bv;
	else {
		t2 -= 2.f;
		*v = (ev-bv)/2.f * (t2 * t2 * ((e1+1)*t2 + e1) + 2) + bv;
	}
	
	return (t-b) >= d;
}

// The arguments are used as follows:
//
//	1. v	= ptr to the float value to be modified
//	2. bv	= start value
//	3. ev	= end value
//	4. t	= time right now
//	5. b    = beginning time
//	6. e    = ending time
//	7. d    = duration
//	8. e1   = extra value for specific easing functions
//	9. e2   = extra value for specific easing functions

// -------------- Additional Tweens: Bounce --------------
// e1: 
// e2: 
inline bool bounceInFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2)
{
	float t2 = (t - b) / d;
	
	if (t2 < (1/2.75)) {
		*v = (ev-bv) * (7.5625 * t2 * t2) + bv;
	} 
	else if (t2 < (2/2.75)) {
		*v = (ev-bv) * (7.5625 * (t2-(1.5/2.75)) * (t2-(1.5/2.75)) + .75) + bv;
	}
	else if (t2 < (2.5/2.75)) {
		*v = (ev-bv) * (7.5625 * (t2-(2.25/2.75)) * (t2-(2.25/2.75)) + .9375) + bv;
	}
	else {
		*v = (ev-bv) * (7.5625 * (t2-(2.625/2.75)) * (t2-(2.625/2.75)) + .984375) + bv;
	}
		
	return (t-b) >= d;
}

inline bool bounceOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2)
{
	float t2 = (t - b) / d;
	t2--;
	
	if (t2 < (1/2.75)) {
		*v = (ev-bv) * (1 - (7.5625 * t2 * t2)) + bv;
	} 
	else if (t2 < (2/2.75)) {
		*v = (ev-bv) * (1 - (7.5625 * (t2-(1.5/2.75)) * (t2-(1.5/2.75)) + .75)) + bv;
	}
	else if (t2 < (2.5/2.75)) {
		*v = (ev-bv) * (1 - (7.5625 * (t2-(2.25/2.75)) * (t2-(2.25/2.75)) + .9375)) + bv;
	}
	else {
		*v = (ev-bv) * (1 - (7.5625 * (t2-(2.625/2.75)) * (t2-(2.625/2.75)) + .984375)) + bv;
	}
		
	return (t-b) >= d;
}

inline bool bounceInOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2)
{
	float t2 = (t - b) / d * 2.f;
	
	if (t2 < 1){
		if (t2 < (1/2.75)) {
			*v = (ev-bv) * (7.5625 * t2 * t2) + .5f + bv;
		} 
		else if (t2 < (2/2.75)) {
			*v = (ev-bv) * (7.5625 * (t2-(1.5/2.75)) * (t2-(1.5/2.75)) + .75) + .5f + bv;
		}
		else if (t2 < (2.5/2.75)) {
			*v = (ev-bv) * (7.5625 * (t2-(2.25/2.75)) * (t2-(2.25/2.75)) + .9375) + .5f + bv;
		}
		else {
			*v = (ev-bv) * (7.5625 * (t2-(2.625/2.75)) * (t2-(2.625/2.75)) + .984375) + .5f + bv;
		}
	}
	else{
		t2--;

		if (t2 < (1/2.75)) {
			*v = (ev-bv) * (7.5625 * t2 * t2) + bv;
		} 
		else if (t2 < (2/2.75)) {
			*v = (ev-bv) * (7.5625 * (t2-(1.5/2.75)) * (t2-(1.5/2.75)) + .75) + .5f + bv;
		}
		else if (t2 < (2.5/2.75)) {
			*v = (ev-bv) * (7.5625 * (t2-(2.25/2.75)) * (t2-(2.25/2.75)) + .9375) + .5f + bv;
		}
		else {
			*v = (ev-bv) * (7.5625 * (t2-(2.625/2.75)) * (t2-(2.625/2.75)) + .984375) + .5f + bv;
		}
	}
	
	return (t-b) >= d;
}

// -------------- Additional Tweens: Elastic --------------
// e1: amplitude
// e2: period
inline bool elasticInFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2)
{
	t = t - b;
	float c = ev - bv;
	
	if (t <= 0) {
		*v = bv;
	} 
	if ((t/=d) >= 1) {
		*v = ev;
	}
	if (e2 <= 0) {
		e2 = d * .3f;	//default period
	} 
	
	float decay;
	
	if (e1 < ::abs(c)) {
		e1 = c;
		decay = e2 / 4.f; 
	} else
	{
		decay= e2 / (M_PI*2) * ::asin(c/e1);
	}
	*v = -(e1 * powf(2.f, 10.f*(t-=1.f)) * ::sin((t*d-decay)*(M_PI*2)/e2)) + bv;
	
	return (t-b) >= d;
}

inline bool elasticOutFunc(float *v, float bv, float ev, 
						  float t, float b, float e, float d,
						  float e1, float e2)
{
	float t2 = (t - b) / d;
	
	if ((t-b) <= 0) {
		*v = 0;
	} 
	if (t2 >= 1) {
		*v = 1;
	}
	if (e2 == 0) {
		e2 = d * .3f;	//default period
	} 
	
	float decay;
	
	if (e1 < 1) { 
		e1 = 1; 
		decay = e2 / 4.f; 
	} else {
		decay= e2 / (M_PI*2) * ::asin(1/e1);
	}
	*v = (e1 * powf(2.f, 10*(-t2)) * ::sin((((t2-1)*d-decay)*(M_PI*2)/e2) + 1));
	
	return (t-b) >= d;
}

inline bool elasticInOutFunc(float *v, float bv, float ev, 
							  float t, float b, float e, float d,
							  float e1, float e2)
	{
		float t2 = (t - b) / d * 2.f;

		if ((t-b) <= 0) {
			*v = 0;
		} 
		if (t2 >= 2) {
			*v = 1;
		}
		if (e2 == 0) {
			e2 = d * .3f * 1.5f;	//default period
		} 

		float decay;

		if (e1 < 1) { 
			e1 = 1; 
			decay = e2 / 4.f; 
		} else {
			decay= e2 / (M_PI*2) * ::asin(1/e1);
		}
		
		if (t2 < 1)
			*v = -.5f * (e1 * powf(2.f, 10*(t2-1)) * ::sin(((t2-1)*d-decay)*(M_PI*2)/e2));
		else
			*v = .5f * (e1 * powf(2.f, -10*(t2-1)) * ::sin(((t2-1)*d-decay)*(M_PI*2)/e2)) + 1;			

		return (t-b) >= d;
	}