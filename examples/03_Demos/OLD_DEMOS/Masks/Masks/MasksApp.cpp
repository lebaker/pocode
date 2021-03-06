#include "MasksApp.h"

#include "poHelpers.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poDictionary.h"
#include "poApplication.h"
#include "poGeometryMask.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"


poObject *createObjectForID(uint uid) {
	return new MasksApp();
}

void setupApplication() {
	// go up the file system from where the app is, set pwd to a folder called resources
	lookUpAndSetPath("resources");
	// read in the common configuration file
	//poCommon::get()->read("common.xml");
	
	// set up the window, (0,0) is lower left of the main screen
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Masks", 0, 200, 800, 800);
}

void cleanupApplication() {
	// write out any changes to the common config file
	//poCommon::get()->write("common.xml");
}

MasksApp::MasksApp() {
	// this 2D camera will also clear the background
	addModifier(new poCamera2D(poColor(.2,.2,.2,1)));
	
	// register to receive key events
	addEvent(PO_KEY_DOWN_EVENT, this);

	setupSurprise();
	setupAperture();
	setupInstructions();
	
	// and start the animation
	start();
}

void MasksApp::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_KEY_DOWN_EVENT:
			// check if the cmd key is down
			if(event->modifiers && PO_KEY_META) {
				if(event->keyChar == 'o') {
					stop();
					open();
				}
				else if(event->keyChar == 'c') {
					close();
					start();
				}
			}
			break;
			
		case PO_MOUSE_DOWN_INSIDE_EVENT:
		case PO_MOUSE_DRAG_EVENT:
			// as you drag the image around make sure to also save it in the config file
			event->source->position = event->globalPosition;
			poXMLDocument common = poXMLDocument("common.xml");
			common.getRootNode().getChild("entry").setInnerString(event->globalPosition.toString());
			common.write("common.xml");
			//poCommon::get()->setPoint("panda position", event->globalPosition);
			//poXMLDocument* doc = new poXMLDocument("common.xml");
			
			break;
	}
}

void MasksApp::setupSurprise() {
	// load an image
	poRectShape *img = new poRectShape("PandaBaby9911.jpeg");
	addChild(img);
	
	
	// add a mask to the thing
	int size = 150.f;
	poShape2D *circle = new poOvalShape(size*1.7f, size*1.7f, 50);
	circle->position.set(200,150,0);
	img->addModifier(new poGeometryMask(circle, true));
	
	// reposition the surprise by clicking and dragging
//	img->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
//	img->addEvent(PO_MOUSE_DRAG_EVENT, this);
	
	// check the settings dictionary for where to put the thing
	poXMLDocument common = poXMLDocument("common.xml");
	std::string pos = common.getRootNode().getChild("entry").getInnerString();
	
	// move the panda to the right spot
	img->position.fromString(pos);
}

void MasksApp::setupAperture() {
	// make a holder for the leaves of the aperture
	holder = new poObject();
	holder->position.set(getWindowWidth()/2,getWindowHeight()/2,0);
	addChild(holder);
	
	// make the elements of the aperture
	int size = 150.f;
	float step = M_2PI / 12.f;
	for(int i=0; i<6; i++) {
		poShape2D *shape = new poShape2D();
		
		poPoint p1 = poPoint(0,0);
		poPoint p2 = poPoint(cosf(-step), sinf(-step)) * size;
		poPoint p3 = poPoint(cosf(step), sinf(step)) * size;
		
		shape->addPoint(p1);
		shape->addPoint(p2);
		shape->addPoint(p3);
		
		// fill out the parameter for this shape
		shape->fillColor = hashPointerForColor(shape);
		shape->fillDrawStyle = GL_TRIANGLES;
			// change the order in which transformations get applied
		shape->matrixOrder = PO_MATRIX_ORDER_RST;
			// this is in degrees
		shape->rotation = rad2deg(i*step*2.f);
		
		// store the triangle in the holder
		holder->addChild(shape);
	}

	// make the masking object from a poShape2D
	poShape2D *circle = new poOvalShape(size*1.7f, size*1.7f, 50);
	poGeometryMask *mask = new poGeometryMask(circle, true);
	// apply the mask
	holder->addModifier(mask);
	

//	holder->addChild(circle);
}

void MasksApp::setupInstructions() {
	poTextBox *text = new poTextBox(200,200);
	addChild(text);
	
	text->setFont(new poFont("Lucida Grande"));
	text->setTextSize(20);
	text->setText("'⌘-o' to open\n'⌘-c' to close\n");
	text->doLayout();
}

void MasksApp::start() {
	// every poObject has a batch of tweens built-in
	holder->rotationTween
		.set(holder->rotation + 360)
		.setTweenFunction(linearFunc)
		.setDuration(5.f)
		.setRepeat(PO_TWEEN_REPEAT_REGULAR)
		.start();
}

void MasksApp::stop() {
	holder->rotationTween.stop();
}

void MasksApp::open() {
	for(int i=0; i<holder->getNumChildren(); i++) {
		poShape2D *shape = dynamic_cast<poShape2D*>(holder->getChild(i));
		shape->positionTween
			.set(poPoint(0,100,0))
			.setTweenFunction(quadInOutFunc)
			.setDuration(.2f)
			.start();
	}
}

void MasksApp::close() {
	for(int i=0; i<holder->getNumChildren(); i++) {
		poShape2D *shape = dynamic_cast<poShape2D*>(holder->getChild(i));
		shape->positionTween
			.set(poPoint(0,0,0))
			.start();
	}
}
