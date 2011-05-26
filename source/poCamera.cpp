//
//  poCamera.cpp
//  poFramework4
//
//  Created by Jared Schiffman on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poCamera.h"
#include "poApplication.h"

poCamera2D::poCamera2D()
{
    clears_background = true;
    background_color.set( 0,0,0 );
}

poCamera2D::poCamera2D(poColor color) {
	clears_background = true;
	background_color = color;
}

void poCamera2D::setUp( poObject* obj ) {
    // setup orthographic camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glPushMatrix();
	glOrtho(0, getWindowWidth(), getWindowHeight(), 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    // disable the depth test
    glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
    // enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // enable anti-aliasing
    glEnable( GL_LINE_SMOOTH );
    
    // clear background
    if ( clears_background )
    {
        glClearColor( background_color.R, background_color.G, background_color.B, 1 );
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void poCamera2D::setDown( poObject* obj ) {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

bool poCamera2D::clearsBackground() const {return clears_background;}
void poCamera2D::clearsBackground(bool b) {clears_background = b;}

poColor poCamera2D::backgroundColor() const {return background_color;}
void poCamera2D::backgroundColor(poColor color) {
	background_color = color;
}

void poCamera2D::backgroundColor(float r, float g, float b, float a) {
	background_color.set(r,g,b,a);
}

