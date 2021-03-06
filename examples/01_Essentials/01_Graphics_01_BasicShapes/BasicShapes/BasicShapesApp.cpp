/////////////////////////////////////////
//
// pocode : Basic Shapes
//
/////////////////////////////////////////

#include "BasicShapesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"				// Include poShapeBasics2D.h to be able to use poShapes

 
// APP CONSTRUCTOR. Create all objects here.
BasicShapesApp::BasicShapesApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
	if(!fs::exists("bg.jpg")) {
		printf("RESOURCES NOT FOUND!\nPlease open the Xcode menu, click on 'Preferences' and select the 'Locations' tab. Click on 'Advanced' and make sure that the 'Legacy' option is checked. If it's not, check it and try running this example again.");
		exit(0);
	}
	
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
    
	// A. poRectShape ///////////////////////
	
	poRectShape* A;
    A = new poRectShape(75, 75);			// Make a new poRectShape with width=75 and height=75
    A->fillColor = poColor::red;			// Set the fill color, red
	A->generateStroke(1);					// Create a 1 pixel stroke around it
	A->strokeColor = poColor::black;		// Set the stroke color
	A->position.set(88, 204, 0);			// Set the position (x, y, z)
	addChild( A );							// Add the object, so it gets drawn on the screen
	
	
	// B. poOvalShape ///////////////////////
	
	poOvalShape* B;
	B = new poOvalShape(100, 65, 50);		// Set width=100, height=65 and number of vertices=50
	B->fillColor = poColor::red;			// Set the fill color, red
	B->generateStroke(1);					// Create a 1 pixel stroke around it
	B->strokeColor = poColor::black;		// Set the stroke color
	B->position.set(310, 240, 0);			// Set the position (x, y, z)
	addChild( B );							// Add the object, so it gets drawn on the screen
	
	
	// C. poStarShape ///////////////////////
	
	poStarShape* C;
	C = new poStarShape(100, 100, 5, 30);	// Set width=100, height=100,
											// number of vertices=5, depth=30
	C->fillColor = poColor::red;			// Set the color, red
	C->position.set(490, 240, 0);			// Set the position (x, y, z)
	addChild( C );							// Add the object, so it gets drawn on the screen
	
	
    // D. poRectShape ///////////////////////
	
	poRectShape* D;
	D = new poRectShape( "apple.jpg" );	// Set the image url
    D->scale.set(0.5, 0.5, 1);				// Scale the object in 3 dimensions (x, y, z)
    D->position.set(615, 190, 0);			// Set the position (x, y, z)
    addChild( D );							// Add the object, so it gets drawn on the screen
}


// APP DESTRUCTOR. Delete all objects here.
BasicShapesApp::~BasicShapesApp() {
}


// UPDATE. Called once per frame. Animate objects here.
void BasicShapesApp::update() {
}

// DRAW. Called once per frame. Draw objects here.
void BasicShapesApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void BasicShapesApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER. Called from within the app. Use for message passing.
void BasicShapesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
