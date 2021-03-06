#include "poObjectTestApp.h"

//#include "poMask.h"
#include "poCamera.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"

poObject *createObjectForID(uint uid) {
	return new poObjectTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "poObjectTest", 100, 100, 800, 800);
}

void cleanupApplication() {
}

poObjectTestApp::poObjectTestApp() {
	addModifier(new poCamera2D());
	
	
//	poShape2D *shape;
//	
//	for(int i=0; i<10; i++) {
//		shape = addChild(new poRectShape(100,100));
//		shape->fillColor = hashPointerForColor(shape);
//		shape->alignment(PO_ALIGN_CENTER_CENTER);
//		shape->position = randomPointInRect(getWindowBounds().inset(poPoint(50,50)));
//		shape->addEvent(PO_MOUSE_UP_INSIDE_EVENT, this, (boost::format("obj%d")%i).str());
//	}
//	
//	shape = new poOvalShape(800,800,140);
//	shape->position = getWindowCenter();
//	
//	addModifier(new poGeometryMask(shape));
	
	poFont *font = getFont("Helvetica");
	std::vector<std::string> encodings = font->getEncodings();
	for(std::vector<std::string>::iterator iter = encodings.begin();
		iter != encodings.end();
		iter++)
	{
		printf("%s\n", iter->c_str());
	}
}

poObjectTestApp::~poObjectTestApp() {
}

void poObjectTestApp::draw() {
	
}

void poObjectTestApp::eventHandler(poEvent *event) {
	if(event->type == PO_MOUSE_UP_INSIDE_EVENT) {
		std::cout << event->message << std::endl;
	}
}

void poObjectTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
