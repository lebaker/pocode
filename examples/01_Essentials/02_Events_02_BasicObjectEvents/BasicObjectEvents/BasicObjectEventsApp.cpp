/////////////////////////////////////////
//
// pocode : Basic Object Events
//
/////////////////////////////////////////

#include "BasicObjectEventsApp.h"
#include "poApplication.h"
#include "poCamera.h"


// APP CONSTRUCTOR. Create all objects here.
BasicObjectEventsApp::BasicObjectEventsApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    FILE* F = fopen("bg.jpg", "r");
	if(!F) {
		printf("RESOURCES NOT FOUND!\nPlease open the Xcode menu, click on 'Preferences' and select the 'Locations' tab. Click on 'Advanced' and make sure that the 'Legacy' option is checked. If it's not, check it and try running this example again.");
		exit(0);
	}
	else fclose(F);
	
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// A NOTE ON OBJECT EVENTS ///////////////////////
	// Object events happen within the bounds of an object
	
	
	// A. PO_MOUSE_DOWN_INSIDE_EVENT ///////////////////////
	
	A = new poRectShape(75, 75);						// Draw a rectangle that will change color
	A->fillColor.set(1, 0.85, 0);
	A->position.set(90, 204, 0);
	addChild(A);
	
	A->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);		// Add a "mouse down inside" event to shape A
	
	
	// B. PO_MOUSE_ENTER_EVENT ///////////////////////
	
	B = new poRectShape(75, 75);						// Draw a rectangle that will change color
	B->fillColor.set(1, 0.85, 0);
	B->position.set(270, 204, 0);
	addChild(B);
	
	B->addEvent(PO_MOUSE_ENTER_EVENT, this);			// Add a "mouse enter" event to shape B
	
	
	// C. PO_MOUSE_LEAVE_EVENT ///////////////////////
	
	C = new poRectShape(75, 75);						// Draw a rectangle that will change color
	C->fillColor.set(1, 0.85, 0);
	C->position.set(455, 204, 0);
	addChild(C);
	
	C->addEvent(PO_MOUSE_LEAVE_EVENT, this);			// Add a "mouse leave" event to shape C
	
	
	// D. PO_MOUSE_OVER_EVENT ///////////////////////
	
	D = new poRectShape(75, 75);						// Draw a rectangle that will change color
	D->fillColor.set(1, 0.85, 0);
	D->position.set(640, 204, 0);
	addChild(D);
	
	D->addEvent(PO_MOUSE_OVER_EVENT, this);				// Add a "mouse over" event to shape D
	
	
	addEvent(PO_KEY_DOWN_EVENT, this);					// Add a "key down" event to reset the color
}

// APP DESTRUCTOR. Delete all objects here.
BasicObjectEventsApp::~BasicObjectEventsApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void BasicObjectEventsApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void BasicObjectEventsApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void BasicObjectEventsApp::eventHandler(poEvent *event) {
	
	if(event->type == PO_MOUSE_DOWN_INSIDE_EVENT) {		// Verify if you get an event of type mouse down inside
            A->fillColor = poColor::blue;					// Change the color of rectangle A
	}
	else if(event->type == PO_MOUSE_ENTER_EVENT) {		// Verify if you get an event of type mouse enter
            B->fillColor = poColor::blue;					// Change the color of rectangle B
	}
	else if(event->type == PO_MOUSE_LEAVE_EVENT) {		// Verify if you get an event of type mouse leave
            C->fillColor = poColor::blue;					// Change the color of rectangle C
	}
	else if(event->type == PO_MOUSE_OVER_EVENT) {		// Verify if you get an event of type mouse over
            D->fillColor = poColor::blue;					// Change the color of rectangle D
	}
	else if(event->type == PO_KEY_DOWN_EVENT) {			// Verify if you get an event of type key down
		
		if(event->keyChar == 'r') {						// Verify which key was pressed
														// If the key is 'r' reset the color of each rectangle
			A->fillColor.set(1, 0.85, 0);
			B->fillColor.set(1, 0.85, 0);
			C->fillColor.set(1, 0.85, 0);
			D->fillColor.set(1, 0.85, 0);
		}
	}
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void BasicObjectEventsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
