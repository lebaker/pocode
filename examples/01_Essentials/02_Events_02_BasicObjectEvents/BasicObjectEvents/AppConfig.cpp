/////////////////////////////////////////
//
// pocode Application Configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "BasicObjectEventsApp.h"
#include "poHelpers.h"

poObject *createObjectForID(uint uid) {
	return new BasicObjectEventsApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicObjectEvents", 100, 100, 800, 600);
}

void cleanupApplication() {
}