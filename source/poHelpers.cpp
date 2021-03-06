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

#include "poHelpers.h"
#include <cstdarg>
#include <cstdio>
#include <deque>
#include <fstream>
#include <sstream>
#include <stdarg.h>
#include <ctime>
#include <utf8.h>

#include "poObject.h"
#include "poShape2D.h"
#include "poWindow.h"
#include "poApplication.h"
#include "poOpenGLState.h"

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::gregorian;
using namespace boost::posix_time;

fs::path currentPath() {
	return fs::current_path();
}

bool pathToFolder(const std::string &folder_name, fs::path *path) {
	fs::path response = currentPath();
	while(!fs::exists(response / folder_name) && !fs::is_directory(response / folder_name) && response != "/") {
		response = response.parent_path();
	}
	
	if(response == "/") {
		*path = "";
		return false;
	}
	
	*path = response;
	return true;
}

bool lookUpAndSetPath(const std::string &folder_name) {
	fs::path path;
	if(pathToFolder(folder_name, &path)) {
		setCurrentPath(path/folder_name);
		return true;
	}
	return false;
}

bool lookUpAndSetPathNextTo(const std::string &folder_name) {
	fs::path path;
	if(pathToFolder(folder_name, &path)) {
		setCurrentPath(path);
		return true;
	}
	return false;
}

std::string getApplicationSupportDirectory() {
    return applicationGetSupportDirectory();
}

unsigned int getNumCpus() {
	return boost::thread::hardware_concurrency();
}

#ifdef __APPLE__
	#include <mach/mach_time.h>
	#include <sys/param.h>
	#include <sys/sysctl.h>
	#include <Foundation/Foundation.h>

	#if defined(POTION_MAC)
		#include <Cocoa/Cocoa.h>

		poPoint deviceResolution() {
			NSWindow *window = (NSWindow*)applicationCurrentWindow()->getWindowHandle();
			NSScreen *screen = [window screen];
			
			NSSize size = [[[screen deviceDescription] objectForKey:NSDeviceResolution] sizeValue];
			return poPoint(size.width, size.height);
		}

	#else // IPHONE OR SIMULATOR
		#include <UIKit/UIKit.h>

		poPoint deviceResolution() {
			return poPoint(72,72);
		}

	#endif

	float poGetElapsedTime() {
		static uint64_t start = 0.0;
		static mach_timebase_info_data_t info;
		if(info.denom == 0) {
			mach_timebase_info(&info);
			start = mach_absolute_time();
		}
		
		uint64_t duration = mach_absolute_time() - start;
		return ((duration * info.numer) / (double)info.denom) * 1.0e-9;
	}

    int poGetElapsedTimeMillis() {
        return poGetElapsedTime() * 1000.0f;
    }

	void setCurrentPath(const fs::path &path) {
		NSString *nsstr = [NSString stringWithCString:path.c_str() encoding:NSUTF8StringEncoding];
		[[NSFileManager defaultManager] changeCurrentDirectoryPath:nsstr];
	}

#elif defined(POTION_WIN32)

	poPoint deviceResolution() {
		return poPoint(72, 72);
	}

	float poGetElapsedTime() {
		static __int64 freq=0L, start;

		if(freq == 0) {
			// start
			QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
			QueryPerformanceCounter((LARGE_INTEGER*)&start);
		}

		// end
		__int64 end;
		QueryPerformanceCounter((LARGE_INTEGER*)&end);
		double diff = (end - start) / (double)freq;
		return diff;
	}

    int poGetElapsedTimeMillis() {
        return poGetElapsedTime() * 1000.0f;
    }

	void setCurrentPath(const fs::path &path) {
		boost::system::error_code ec;
		fs::current_path(path, ec);
		if(ec)
			printf("error setting current path: %s\n", ec.message().c_str());
	}

#endif

poTime poGetCurrentTime() {
	date today(day_clock::local_day());
        
	ptime now = second_clock::local_time();
        
	poTime t;
	t.hours      = now.time_of_day().hours();
	t.minutes    = now.time_of_day().minutes();
	t.seconds    = now.time_of_day().seconds();
        
	//Am/PM
	t.amPmHours   = t.hours;
        
	t.amPm = t.amPmHours < 12 ? "AM" : "PM";
        
	if(t.amPmHours > 12) t.amPmHours -= 12;
	if(t.amPmHours == 0) t.amPmHours = 12;
        
	return t;
}


std::vector<poPoint> roundedRect(float width, float height, float rad) {
	std::vector<poPoint> response;
	std::vector<poPoint> tmp;
	
    poPoint P1( rad,rad );
    poPoint P2( width-rad,rad );
    poPoint P3( width-rad,height-rad );
    poPoint P4( rad,height-rad );
	
//	response.push_back(poPoint(0,rad));
    
    for( int i=180; i>=90; i-=10 )
    {
        float A = i;
        poPoint P = P1 + poPoint( cos_deg(A)*rad, -sin_deg(A)*rad, 0 );
        response.push_back( P );
    }
    
    for( int i=90; i>=0; i-=10 )
    {
        float A = i;
        poPoint P = P2 + poPoint( cos_deg(A)*rad, -sin_deg(A)*rad, 0 );
        response.push_back( P );
    }
    
    for( int i=0; i>=-90; i-=10 )
    {
        float A = i;
        poPoint P = P3 + poPoint( cos_deg(A)*rad, -sin_deg(A)*rad, 0 );
        response.push_back( P );
    }
    
    for( int i=-90; i>=-180; i-=10 )
    {
        float A = i;
        poPoint P = P4 + poPoint( cos_deg(A)*rad, -sin_deg(A)*rad, 0 );
        response.push_back( P );
    }
	
	response.push_back(response[0]);
	return response;
}

std::vector<poPoint> quadTo(poPoint p1, poPoint p2, poPoint control, int resolution) {
	std::vector<poPoint> response;
	for(int i=0; i<resolution; i++) {
		float t = i / float(resolution-1);
		float invt = 1.f - t;
		poPoint pt = invt*invt*p1 + 2*invt*t*control + t*t*p2;
		response.push_back(pt);
	}
	return response;
}

std::vector<poPoint> cubeTo(poPoint p1, poPoint p2, poPoint c1, poPoint c2, int resolution) {
	std::vector<poPoint> response;
	for(int i=0; i<resolution; i++) {
		float t = i / float(resolution-1);
		float invt = 1.f - t;
		poPoint pt = invt*invt*invt*p1 + 3*invt*invt*t*c1 + 3*invt*t*t*c2 + t*t*t*p2;
		response.push_back(pt);
	}
	return response;
}

float curveLength(const std::vector<poPoint> &curve) {
	float len = 0;
	for(int i=0; i<curve.size()-1; i++) {
		len += (curve[i+1] - curve[i]).getLength();
	}
	return len;
}

std::ofstream log_file;

const char *currentTimeStr() {
	static char buffer[80];
	
	time_t now = time(NULL);
	strftime(buffer,80,"%I:%M:%S %p",localtime(&now));
	
	return buffer;
}

void log(const char *format, ...) {
	static char buffer[SHRT_MAX];

	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	
	if(!log_file.is_open())
		log_file.open("log.text");

	std::stringstream ss;
	ss << currentTimeStr() << ": " << buffer << "\n";
	
	log_file << ss.str();
	std::cerr << ss.str();
}

size_t utf8strlen(const std::string &str) {
	using utf8::unchecked::next;
	return utf8::unchecked::distance(str.begin(), str.end());
}

poPoint alignInRect(poPoint max, poRect rect, poAlignment align) {
	poPoint offset;
	switch(align) {
		case PO_ALIGN_TOP_LEFT:
			offset.set(0.f, max.y - 1.f, 0.f);
			break;
		case PO_ALIGN_TOP_CENTER:
			offset.set((max.x - 1.f)/2.f, max.y - 1.f, 0.f);
			break;
		case PO_ALIGN_TOP_RIGHT:
			offset.set(max.x - 1.f, max.y - 1.f, 0.f);
			break;
		case PO_ALIGN_CENTER_LEFT:
			offset.set(0.f, (max.y - 1.f)/2.f, 0.f);
			break;
		case PO_ALIGN_CENTER_CENTER:
			offset.set((max.x - 1.f)/2.f, (max.y - 1.f)/2.f, 0.f);
			break;
		case PO_ALIGN_CENTER_RIGHT:
			offset.set(max.x - 1.f, (max.y - 1.f)/2.f, 0.f);
			break;
		case PO_ALIGN_BOTTOM_LEFT:
			offset.set(0.f, 0.f, 0.f);
			break;
		case PO_ALIGN_BOTTOM_CENTER:
			offset.set((max.x - 1.f)/2.f, 0.f, 0.f);
			break;
		case PO_ALIGN_BOTTOM_RIGHT:
			offset.set(max.x - 1.f, 0.f, 0.f);
			break;
	}
	return offset;
}

static const std::string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
	
	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			
			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}
	
	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';
		
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
		
		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];
		
		while((i++ < 3))
			ret += '=';
		
	}
	
	return ret;
	
}

std::string base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;
	
	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);
			
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
			
			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}
	
	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;
		
		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);
		
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
		
		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}
	
	return ret;
}


std::string poToUpperCase(std::string s) {
    for(int i=0; i<s.length(); i++) {
        s[i] = toupper(s[i]);
    }
    
    return s;
}

std::string poToLowerCase(std::string s) {
    for(int i=0; i<s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    
    return s;
}

std::string poToTitleCase(std::string s) {
    for(int i=0; i<s.length(); i++) {
        if(i==0 || s[i-1] == ' ') {
            s[i] = toupper(s[i]);
        }
    }
    
    return s;
}

int poToInt(std::string s) {
    return atoi(s.c_str());
}
