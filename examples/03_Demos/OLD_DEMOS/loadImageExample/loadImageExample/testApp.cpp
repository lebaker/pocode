//
//  Load Image Example
//  poFramework4
//
//  Created by Miguel Bermudez on 6/17/11.
//  Copyright 2011 Potion Design. All rights reserved.
//  
//  Image (c)  Michael Cina 2010 for Ghostly International 
//  http://ghostly.com/wallpapers/cina_verse_2
//

#include "testApp.h"

#include "Helpers.h"
#include "poCamera.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"
#include "poResourceLoader.h"

poObject *createObjectForID(uint uid) {
	return new testApp();
}

void setupApplication() {
	// set the pwd to a directory above the app named resources
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "test", 100, 100, 600, 300);
}

void cleanupApplication() {
}

testApp::testApp() {
	addModifier(new poCamera2D());
    
	/**
	 *	hold an image in the resource store with getImage
	 *	create new poRectShape and pass it the image texture
	 *	rect size will equal texture size
	 *	set alignment to center
	 *	position image in the center of the screen
	 *	add to stage
	 */
	
    poImage img = getImage("images/GH_0002d.jpg");
	poTexture tex(img, poTextureConfig(GL_RGB).setInternalFormat(GL_RGB32F_ARB));
	
	poShape2D *image = new poRectShape(tex);
    image->setAlignment(PO_ALIGN_CENTER_CENTER);
    image->position.set(getWindowWidth()/2, getWindowHeight()/2, 0);
    addChild(image);

	// it will show up as a red square if the image doesn't exist
	
	img = getImage("THIS_IMAGE_DOESNT_EXIST");
	tex = poTexture(img);
	
	image = new poRectShape(tex);
	addChild(image);
}

testApp::~testApp() {}
