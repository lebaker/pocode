#include "GoldenRatioApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "math.h"

const int SEEDSIZE = 8;
const int NUMSIDES = 3;
const int SPACING  = 10;
const float PHI = (1+sqrt(5))/2;

poObject *createObjectForID(uint uid) {
	return new GoldenRatioApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

GoldenRatioApp::GoldenRatioApp() {
	addModifier(new poCamera2D(poColor::black));
    
    poObject *bloom = new poObject();
    bloom->position(getWindowWidth()/2,getWindowHeight()/2,0);
    addChild(bloom);
    
    for(int i=1; i<3000; i++){
        seeds.push_back(new poOvalShape(SEEDSIZE,SEEDSIZE,NUMSIDES));
        seeds.back()->matrixOrder(PO_MATRIX_ORDER_RST)
        .rotation(i*360*PHI)
        .position(sqrt(i)*SPACING,0);
    }
    
    for(int j=0; j<seeds.size(); j++){
        bloom->addChild(seeds[j]);
    }
    
    addEvent(PO_MOUSE_MOVE_EVENT, this, "mouseMove");
}

GoldenRatioApp::~GoldenRatioApp() {
}

void GoldenRatioApp::update(){
    
}

void GoldenRatioApp::preDraw(){
    
}

void GoldenRatioApp::draw(){
    
}

void GoldenRatioApp::postDraw(){
    
}

void GoldenRatioApp::eventHandler(poEvent *event){
    if(event->type == PO_MOUSE_MOVE_EVENT){
        for(int j=0; j<seeds.size(); j++){
            seeds[j]->scale(8*(event->position)/getWindowWidth());
        }
    }
    
}