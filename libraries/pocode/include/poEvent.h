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


#include "poEnums.h"
#include "poDictionary.h"

// PO_EVENT TYPES
// This is a list of all poEvent types. These are used in addEvent() and in the eventHandker implementations.
enum {
	PO_MOUSE_DOWN_EVENT,
	PO_MOUSE_DOWN_INSIDE_EVENT,
	PO_MOUSE_UP_EVENT,
	PO_MOUSE_MOVE_EVENT,
	PO_MOUSE_DRAG_EVENT,
    PO_MOUSE_DRAG_INSIDE_EVENT,
	PO_MOUSE_ENTER_EVENT,
	PO_MOUSE_LEAVE_EVENT,
	PO_MOUSE_OVER_EVENT,
	
	PO_KEY_DOWN_EVENT,
	PO_KEY_UP_EVENT,
	
	PO_TOUCH_BEGAN_EVENT,
	PO_TOUCH_BEGAN_INSIDE_EVENT,
	PO_TOUCH_BEGAN_OUTSIDE_EVENT,
	PO_TOUCH_MOVED_EVENT,
	PO_TOUCH_ENDED_EVENT,
	PO_TOUCH_ENDED_INSIDE_EVENT,
	PO_TOUCH_ENDED_OUTSIDE_EVENT,
	PO_TOUCH_CANCELLED_EVENT,
    
    PO_ACCELEROMETER_EVENT,
	PO_GYROSCOPE_EVENT,
    PO_ROTATION_EVENT,
    
	PO_LAST_EVENT
};

class poObject;
class poEvent;

typedef std::vector<poEvent*> poEventVec;
typedef std::vector<poEventVec> poEventTable;

// check to see if a specific modifier key is being held down
bool isNumericMask(poEvent*);
bool isFunctionMask(poEvent*);
bool isArrowMask(poEvent*);
bool isMetaMask(poEvent*);
bool isAltMask(poEvent*);
bool isShiftMask(poEvent*);
bool isCtrlMask(poEvent*);

// check for a given mouse button
bool isLeftMouseDown(poEvent*);
bool isRightMouseDown(poEvent*);
bool isCenterMouseDown(poEvent*);

// check event type
bool isMouseEvent(int type);
bool isKeyEvent(int type);
bool isTouchEvent(int type);
bool isMotionEvent(int type);

// PO_EVENT SPECIAL KEYS
enum {
	// key event mask
	PO_KEY_CAPS		= 1 << 16,
	PO_KEY_SHIFT	= 1 << 17,
	PO_KEY_CTRL		= 1 << 18,
	PO_KEY_ALT		= 1 << 19,
	PO_KEY_META		= 1 << 20,
	// specific key 
	PO_NUMERIC_KEY_MASK = 1 << 21,
	PO_FUNCTION_KEY_MASK = 1 << 23,
};


// PO_EVENT MOUSE BUTTON LEFT and RIGHT
enum {
	// mouse event mask
	PO_MOUSE_LEFT	= 1,
	PO_MOUSE_MIDDLE	= 1 << 1,
	PO_MOUSE_RIGHT	= 1 << 2,
};

enum {
	// also need to check the modifier flags
	// for arrows, numeric mask and function mask should both be set
	// these numbers are the keyCode
	PO_UP_ARROW = 126,
	PO_DOWN_ARROW = 125,
	PO_LEFT_ARROW = 123,
	PO_RIGHT_ARROW = 124,
	
	PO_RETURN_KEY = 36,
	
	PO_F1 = 122,
	PO_F2 = 120,
	PO_F3 = 99,
	PO_F4 = 118,
	PO_F5 = 96,
	PO_F6 = 97,
	PO_F7 = 98,
	PO_F8 = 100
};

// class poObject;

// CLASS NOTES
//
// poEvent is an object used in pocode for communicating user events to poObjects.
//
// poObjects register for events by calling the poObject's addEvent() method as follows:
//
//      myObject->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this, "message" );
//
// poObjects receive events through the virtual eventHandler() method.
//
// poEvent objects are used as records of event registration (stored by the poEventCenter)
// and as containers for event messages that are propagated to poObjects.
//
// In general, users do not need to construct their own poEvents.
//


class poEvent
{
public:
	poEvent();
	poEvent(int type, poObject* from, std::string _message, const poDictionary& dict=poDictionary());
	
    // EVENT PROPERTIES
    // When you call addEvent(), the object that the event is added to is the source.
	// Source is the poObject that generated the event.
	poObject*       source;

	// The event type.
	int             type;
	// When the event happened.
	float          timestamp;

    // The event message and dictionary are set by the user per event, via the addEvent method.
    std::string     message;
	poDictionary    dictionary;
	
	// Mouse or key modifiers.
	unsigned int    modifiers;
	
	// position is local to the receiver of the event.
    // globalPosition is the mouse or touch position relative to the screen.
	poPoint         localPosition, globalPosition, previousPosition;
    
    //Accelerometer, gyro, etc
    poPoint         motion;

	// The keyCode is the raw key identifier, OS-defined.
	int             keyCode;
	// The keyChar is the key with modifiers applied.
    // In general, the keyChar is used over keyCode
	int             keyChar;
	
	// The touchID stores the identifier of a touch event. All touch events connected
    // to a single touch movement should have the same touchID.
	int             touchID;
    int             uniqueID;
    // Touches have a tap count in iOS, useful for double/triple tap events
    int             tapCount;
};