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

#include <iostream>
#include "poControl.h"
#include "poHelpers.h"
#include "poApplication.h"

int poControl::controlTextSize = 12;

poControl::poControl( string _ID, poObject* _listener ) {
    ID = _ID;
    listener = _listener;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poSliderF::poSliderF( string _ID, float init, float _min, float _max, string _valName ,poObject* _listener ) : poControl( _ID, _listener ) {
	
    type = PO_CONTROL_TYPE_FLOAT;
    
	this->name = ID;
    min = _min;
    max = _max;
    
    valF = poClamp(min, max, init);
    valName = _valName;

    char valString [256];
    sprintf( valString, "%s%.2f", valName.c_str(), valF );
    
    addEvent( PO_MOUSE_DOWN_EVENT, this );
    
    sliderShape = new poRectShape( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
    sliderShape->fillColor = poColor(1,1,1,0.2);
    sliderKnob = new poRectShape( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
    sliderKnob->fillColor = poColor(1,1,1,0.2);
    sliderKnob->strokeColor = poColor(0,0,0,0.2);
    sliderKnob->generateStroke(2);  
    sliderKnob->position.x = poMapf(min, max, valF, 0, PO_CONTROL_SLIDER_WIDTH);
    sliderKnob->addEvent( PO_MOUSE_DRAG_INSIDE_EVENT, this );
    sliderKnob->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this );
    
    addChild( sliderShape );
    sliderShape->addChild( sliderKnob );
    
    shapeData = new poTextBox( PO_CONTROL_SLIDER_WIDTH,PO_CONTROL_SLIDER_HEIGHT );
	shapeData->textColor = poColor::white;
    shapeData->position.set(0,0,0);
    shapeData->setTextSize(controlTextSize);
    shapeData->setText(valString);
	shapeData->doLayout();  
    addChild( shapeData );
    
    shapeLabel = new poTextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
	shapeLabel->setLeading(.6);
	shapeLabel->setTextAlignment(PO_ALIGN_CENTER_LEFT);
    shapeLabel->position.set( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SPACER,0,0);
    shapeLabel->setTextSize(controlTextSize);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();
    addChild( shapeLabel );
}

void poSliderF::eventHandler(poEvent *event) {
    
    if (event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
        
        dragOffset = event->localPosition;
    }

    if (event->type == PO_MOUSE_DRAG_INSIDE_EVENT ) {
        
        poPoint sOffset = sliderShape->localToGlobal( poPoint(0,0) );
        
        float sliderWidth = sliderShape->getWidth() - PO_CONTROL_SLIDER_HEIGHT;
        float knobWidth = sliderKnob->getWidth();
        float xMin = 0;
        float xMax = sliderWidth;
        float posX = event->globalPosition.x - dragOffset.x - sOffset.x;
        
        posX = poClamp( xMin, xMax, posX);
        valF =  sqrt( pow(posX/xMax,2) ); 
        
        float mappedVal = poMapf(0.0f,1, valF, min, max );
		
        sliderKnob->position.x = sliderWidth*valF;
		
        char valString [256];
        sprintf( valString, "%s%.2f", valName.c_str(), mappedVal );
        shapeData->setText( valString );
        shapeData->doLayout();
		
		valF = mappedVal;
		
		poDictionary D;
        D.set("value", valF);
        
        if ( listener == NULL ) {
            //getParent()->messageHandler( ID, D);
        }
        else
            listener->messageHandler( ID, D);
		
		D.set("control", this);
		D.set("valueType", "float");
		getParent()->messageHandler("update_settings", D);
    }
}

void poSliderF::setValue(float val) {
	
	float sliderWidth = sliderShape->getWidth() - PO_CONTROL_SLIDER_HEIGHT;
	float xMin = 0;
	float xMax = sliderWidth;
	
	float mappedVal = poMapf(min, max, val, 0.f, 1.f );
	valF =  mappedVal;
	
	sliderKnob->position.x = sliderWidth*valF;
	
	char valString [32];
	sprintf( valString, "%.2f", val );
	shapeData->setText( valString );
	shapeData->doLayout();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poSliderI::poSliderI( string _ID, int init, int _min, int _max, poObject* _listener ) : poControl( _ID, _listener ) {
    
    type = PO_CONTROL_TYPE_INT;
    
	this->name = ID;
    min = _min;
    max = _max;
	
	valI = poClamp(min, max, init);
    valF = (float)valI;
    
    char valString [256];
    sprintf( valString, "%i", valI );
    
    addEvent( PO_MOUSE_DOWN_EVENT, this );
    
    sliderShape = new poRectShape( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
    sliderShape->fillColor = poColor(1,1,1,0.2);
    sliderKnob = new poRectShape( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
    sliderKnob->fillColor = poColor(1,1,1,0.2);
    sliderKnob->strokeColor = poColor(0,0,0,0.2);
    sliderKnob->generateStroke(2); 
    sliderKnob->position.x = poMapf(min, max, valI, 0, PO_CONTROL_SLIDER_WIDTH);
    sliderKnob->addEvent( PO_MOUSE_DRAG_INSIDE_EVENT, this );
    sliderKnob->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this );
    
    addChild( sliderShape );
    sliderShape->addChild( sliderKnob );
    
    shapeData = new poTextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
	shapeData->textColor = poColor::white;
    shapeData->position.set(0,0,0);
    shapeData->setTextSize(controlTextSize);
    shapeData->setText(valName+valString);
	shapeData->doLayout();  
    addChild( shapeData );
    
    shapeLabel = new poTextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SPACER*4,0,0);
    shapeLabel->setTextSize(controlTextSize);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
}

void poSliderI::eventHandler(poEvent *event) {
    
    
    if (event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
        
        dragOffset = event->localPosition;
    }
    
    
    if (event->type == PO_MOUSE_DRAG_INSIDE_EVENT ) {
        
        poPoint sOffset = sliderShape->localToGlobal( poPoint(0,0) );
        float sliderWidth = sliderShape->getWidth() - PO_CONTROL_SLIDER_HEIGHT;
        float knobWidth = sliderKnob->getWidth();
        float xMin = 0;
        float xMax = sliderWidth;
        float posX = event->globalPosition.x - dragOffset.x - sOffset.x;
        
        posX = poClamp( xMin, xMax, posX);
        
        valF =  sqrt( pow(posX/xMax,2) ); 
        float mappedVal = poMapf(0.0f,1, valF, min, max );
        
        sliderKnob->position.x = sliderWidth*valF;
        poDictionary D;
        
        char valString [256];
        sprintf( valString, "%i", (int)mappedVal );
        shapeData->setText( valString );
        shapeData->doLayout();  
        
        D.set("value", (int)mappedVal );
        valI = (int)mappedVal;

        if ( listener == NULL ) {
            //getParent()->messageHandler( ID, D);
        }
        else
            listener->messageHandler( ID, D);
		
		D.set("control", this);
		D.set("valueType", "int");
		getParent()->messageHandler("update_settings", D);
    }
}

void poSliderI::setValue(int val) {
	
	float sliderWidth = sliderShape->getWidth() - PO_CONTROL_SLIDER_HEIGHT;
	float xMin = 0;
	float xMax = sliderWidth;
		
	float mappedVal = poMapf(min, max, val, 0.f, 1.f );
	valF =  mappedVal;
	
	sliderKnob->position.x = sliderWidth*valF;
	
	char valString [32];
	sprintf( valString, "%i", val );
	shapeData->setText( valString );
	shapeData->doLayout();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poPointSlider::poPointSlider( string _ID, poPoint init, poPoint _min, poPoint _max, poObject* _listener ) : poControl( _ID, _listener ) {
    
    type = PO_CONTROL_TYPE_POINT;
	
	this->name = ID;
    min = _min;
    max = _max;
	
	min.z = 0;
	max.z = 0;
    
    valP.x = poClamp(min.x, max.x, init.x);
	valP.y = poClamp(min.y, max.y, init.y);
	valP.z = 0;
    
    char valString [256];
    sprintf( valString, "%.2lf,%.2lf", valP.x, valP.y );
    
    addEvent( PO_MOUSE_DOWN_EVENT, this );
    
    poRectShape* back = new poRectShape( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT, PO_CONTROL_SLIDER_WIDTH );
    back->fillColor = poColor(1,1,1,0.2);
    addChild( back );
    
    sliderShape = new poRectShape( PO_CONTROL_SLIDER_WIDTH, PO_CONTROL_SLIDER_WIDTH - PO_CONTROL_SLIDER_HEIGHT );
    sliderShape->position = poPoint (PO_CONTROL_SLIDER_HEIGHT/2,PO_CONTROL_SLIDER_HEIGHT/2);
    sliderShape->fillColor = poColor(0,0,0,0);
    sliderPoint = new poRectShape( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
    sliderPoint->position.x = poMapf(min.x, max.x, valP.x, 0, sliderShape->getWidth()); 
    sliderPoint->position.y = poMapf(min.y, max.y, valP.y, 0, sliderShape->getHeight()); 
    sliderPoint->fillColor = poColor(1,0,0,0.5);
    sliderPoint->setAlignment(PO_ALIGN_CENTER_CENTER);
    sliderPoint->addEvent( PO_MOUSE_DRAG_INSIDE_EVENT, this );
    sliderPoint->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this );
    
    addChild( sliderShape );
    sliderShape->addChild( sliderPoint );
    
    shapeData = new poTextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
	shapeData->textColor = poColor::white;
    shapeData->position.set(0,0,0);
    shapeData->setTextSize(controlTextSize);
    shapeData->setText( valString );
	shapeData->doLayout();  
    addChild( shapeData );
    
    shapeLabel = new poTextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SPACER*4,0,0);
    shapeLabel->setTextSize(controlTextSize);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
}

void poPointSlider::eventHandler(poEvent *event) {
    
    if (event->type == PO_MOUSE_DRAG_INSIDE_EVENT ) {
        poPoint P = event->globalPosition;
        P.y = getWindowHeight() - P.y;
        poPoint pos = sliderShape->globalToLocal( P );
        sliderPoint->position = pos; 
              
        float sliderWidth = sliderShape->getWidth();
        float boxWidth = sliderPoint->getWidth();
        float xMin = 0;
        float xMax = sliderWidth+boxWidth/2;

        float sliderHeight = sliderShape->getHeight();
        float yMin = 0;
        float yMax = sliderHeight+boxWidth/2;
 
        pos.x = poClamp( xMin, xMax, pos.x);
        pos.y = poClamp( yMin, yMax, pos.y);

        valP.x =  sqrt( pow( pos.x/xMax,2 ) ); 
        valP.y =  sqrt( pow( pos.y/yMax,2 ) ); 

        sliderPoint->position.x = sliderWidth*valP.x; 
        sliderPoint->position.y = sliderHeight*valP.y;
        
        poPoint mappedVal;
        mappedVal.x = poMapf(0.0f,1, valP.x, min.x, max.x);
        mappedVal.y = poMapf(0.0f,1, valP.y, min.y, max.y);        
                
        char valString [256];
        sprintf( valString, "%.2lf,%.2lf", mappedVal.x, mappedVal.y );
        shapeData->setText( valString );
        shapeData->doLayout();  
        
        poDictionary D;
        D.set("value", mappedVal);
        valP = mappedVal;
        
        if ( listener == NULL ) {
            //getParent()->messageHandler( ID, D );
        }
        else
            listener->messageHandler( ID, D );
		
		D.set("control", this);
		D.set("valueType", "point");
		getParent()->messageHandler("update_settings", D);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poToggleBox::poToggleBox( string _ID, bool init, poObject* _listener ) : poControl( _ID, _listener ) {
    
    type = PO_CONTROL_TYPE_BOOL;
    
    valB = init;
    this->name = ID;
    
    addEvent( PO_MOUSE_DOWN_EVENT, this );
    
    poRectShape* back = new poRectShape( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT);
    back->fillColor = poColor(1,1,1,0.2);
    addChild( back );
    
    toggleShape = new poRectShape( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
    toggleShape->fillColor = poColor((int)valB,0,0,(int)valB);
    toggleShape->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this );
    addChild( toggleShape );
    
    shapeLabel = new poTextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( PO_CONTROL_SLIDER_HEIGHT+PO_CONTROL_SPACER*2,0,0);
    shapeLabel->setTextSize(controlTextSize);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
}

void poToggleBox::eventHandler(poEvent *event) {
 
    if (event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
        valB = !valB;
        toggleShape->fillColor = poColor((int)valB,0,0,(int)valB);
        
        poDictionary D;
        D.set("value", valB);

        if ( listener == NULL ) {
            //getParent()->messageHandler( ID, D);
        }
        else {
            listener->messageHandler( ID, D);
        }
		
		D.set("control", this);
		D.set("valueType", "bool");
		getParent()->messageHandler("update_settings", D);
    }
}

void poToggleBox::setValue(bool val) {
	
	toggleShape->fillColor = poColor((int)val,0,0,(int)val);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poRadio::poRadio( string _ID, int init, vector<string> names, poObject* _listener ) : poControl( _ID,_listener ) {
    
    type = PO_CONTROL_TYPE_INT;
    
	valI = init;
    this->name = ID;
    
    for ( int i=0; i<names.size(); i++ ) {
        bool thisVal;
        if ( valI == i ) thisVal = true; else thisVal = false; 
                
        string uID = ID+"_"+names[i];
        poToggleBox* T = new poToggleBox( uID, thisVal, this );
        T->position = poPoint( 0,i*(PO_CONTROL_SLIDER_HEIGHT+PO_CONTROL_SPACER) );
        T->shapeLabel->setText( names[i] );
        T->shapeLabel->doLayout();
        addChild( T );
        buttons.push_back( T );
    }
    
    shapeLabel = new poTextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SPACER*4,0,0);
    shapeLabel->setTextSize(controlTextSize);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();
    addChild( shapeLabel );
}

void poRadio::eventHandler(poEvent *event) {
}

void poRadio::messageHandler(const std::string &msg, const poDictionary& dict) {
	
	if(msg == "update_settings")
		return;
	else {
		for(int i=0 ; i<buttons.size(); i++) {
			poToggleBox* T = buttons[i];
			if ( T->name == msg ) {
				T->valB = true;
				T->toggleShape->fillColor = poColor(1,0,0,1);
				valI = i;
			}
			else {
				T->valB = false;
				T->toggleShape->fillColor = poColor(0,0,0,0);
			}
		}
		
		poDictionary D;
		D.set( "value",valI ); 
		
		if ( listener == NULL ) {
		}
		else
			listener->messageHandler( ID, D);
		
		D.set("control", this);
		D.set("valueType", "int");
		getParent()->messageHandler("update_settings", D);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poInputTextBox::poInputTextBox( string _ID, string init, poObject* _listener ) : poControl( _ID, _listener ) {
    
    type = PO_CONTROL_TYPE_TEXT;
    
    this->name = ID;
    string temp;
    temp = init;
    //cout << temp << endl;
    temp.erase(temp.find_last_not_of("'")+1);
    temp.erase(0,temp.find_first_not_of("'"));
    valS = temp;
        
    back = new poRectShape( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT, 30 );
    back->fillColor = poColor(1,1,1,0.2);
	back->generateStroke(1);
	back->strokeColor = poColor(1,0,0,1);
	back->strokeEnabled = false;
	back->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
    addChild( back );
    
    shapeData = new poTextBox( PO_CONTROL_SLIDER_WIDTH,30 );
	shapeData->textColor = poColor::white;
    shapeData->position.set(0,0,0);
    shapeData->setTextSize(controlTextSize);
    shapeData->setText( valS );
	shapeData->doLayout();

    addChild( shapeData );
    
    shapeLabel = new poTextBox();
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SPACER*4,0,0);
    shapeLabel->setTextSize(controlTextSize);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
	
	addEvent( PO_KEY_DOWN_EVENT, this );
	
    isActive = false;
    
	resize();
}

void poInputTextBox::eventHandler(poEvent *event) {
	
	if (event->type == PO_MOUSE_DOWN_INSIDE_EVENT) {
		isActive = !isActive;
		back->strokeEnabled = isActive;
	}
    
    if (event->type == PO_KEY_DOWN_EVENT && isActive ) {
        string S;
        if ( event->keyChar != 127 && event->keyChar != 13 ) {
            S = event->keyChar;
            valS = valS+S;
        }

        if ( event->keyChar == 127 && valS.length() > 0 ) {
            valS = valS.erase( valS.length()-1 );
        }
        
        if ( event->keyChar == 13 ) {
            valS = valS + "\n";
        }
        
        shapeData->setText( valS );
        shapeData->doLayout();
		
		resize();
        
        poDictionary D;
        D.set("value", valS);
        
        if ( listener == NULL ) {
            //getParent()->messageHandler( ID, D);
        }
        else
            listener->messageHandler( ID, D);
		
		D.set("control", this);
		D.set("valueType", "string");
		getParent()->messageHandler("update_settings", D);
    }
}

void poInputTextBox::resize() {
	poRect newSize( 0, 0, shapeData->getWidth(), shapeData->getTextBounds().height );
	if(newSize.height < 30) newSize.height = 30;
	shapeData->reshape( newSize );
	back->reshape( back->getWidth(), newSize.height );
	back->generateStroke(1);
	back->strokeEnabled = isActive;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poColorSlider::poColorSlider( string _ID, poColor init, bool _RGB, poObject* _listener ) : poControl( _ID, _listener ) {
    
    type = PO_CONTROL_TYPE_COLOR;
    
    RGB = _RGB;
    valC = init;
    
    float R,G,B,A;
    this->name = ID;
    int values = 4;
    this->position = poPoint(0,PO_CONTROL_SPACER);
    
    for ( int i=0; i<values ; i++ ) {
        float myValue;
        string color;
        
        
        if( i==0 ) {
            myValue = valC.R;
            
            if (RGB) {
                color = ID+"_R";
                poSliderF* S = new poSliderF( color, myValue, 0, 1, "R ", this );
                S->sliderShape->fillColor = poColor(1,1,1,1);
                S->sliderShape->placeTexture( calcTex( poColor( 1,0,0 )) );
                S->position = poPoint( 0,i*PO_CONTROL_SLIDER_HEIGHT );
                S->shapeLabel->visible = false;
                S->valName = "R ";
                addChild( S );
                sliders.push_back( S );
                
            } 
            else {
                color = ID+"_H";
                poSliderF* S = new poSliderF( color, myValue, 0, 1, "H ", this );
                S->sliderShape->fillColor = poColor(1,1,1,1);
                S->sliderShape->placeTexture( calcTex( poColor(-1,valC.G,valC.B) ));
                S->position = poPoint( 0,i*PO_CONTROL_SLIDER_HEIGHT );
                S->shapeLabel->visible = false;
                S->valName = "H ";
                addChild( S );
                sliders.push_back( S );
            }
        }
            
        if( i==1 ) {
            myValue = valC.G;
            
            if (RGB) {
                color = ID+"_G";
                poSliderF* S = new poSliderF( color, myValue, 0, 1,"G ", this );
                S->sliderShape->fillColor = poColor(1,1,1,1);
                S->sliderShape->placeTexture( calcTex( poColor( 0,1,0 )) );
                S->position = poPoint( 0,i*PO_CONTROL_SLIDER_HEIGHT);
                S->shapeLabel->visible = false;
                addChild( S );
                sliders.push_back( S );
                
                
            } 
            else {
                color = ID+"_S";
                poSliderF* S = new poSliderF( color, myValue, 0, 1,"S ", this );
                                
                S->sliderShape->fillColor = poColor(1,1,1,1);
                S->sliderShape->placeTexture( calcTex( poColor( valC.R,-1,valC.B ) ));
                S->position = poPoint( 0,i*PO_CONTROL_SLIDER_HEIGHT );
                S->shapeLabel->visible = false;
                addChild( S );
                sliders.push_back( S );
            }
        }
        
        if( i==2 ) {
            myValue = valC.B;

            if (RGB) {
                color = ID+"_B";
                poSliderF* S = new poSliderF( color, myValue, 0, 1,"B ", this );
                S->sliderShape->fillColor = poColor(1,1,1,1);
                S->sliderShape->placeTexture( calcTex( poColor( 0,0,1 )) );
                S->position = poPoint( 0,i*PO_CONTROL_SLIDER_HEIGHT );
                S->shapeLabel->visible = false;
                addChild( S );
                sliders.push_back( S );
            } 
            else {
                color = ID+"_V";
                poSliderF* S = new poSliderF( color, myValue, 0, 1,"V ", this );
                S->sliderShape->fillColor = poColor(1,1,1,1);
                S->sliderShape->placeTexture( calcTex( poColor( valC.R,valC.G,-1 ) ));
                S->position = poPoint( 0,i*PO_CONTROL_SLIDER_HEIGHT );
                S->shapeLabel->visible = false;
                addChild( S );
                sliders.push_back( S );
            }
        }
        
        
        if( i==3 ) {
            myValue = valC.A;
            color = ID+"_A";
            poSliderF* S = new poSliderF( color, myValue, 0, 1,"A ", this );
            S->sliderShape->fillColor = poColor(1,1,1,1);
            if (!RGB) {
                S->sliderShape->placeTexture( calcTex( poColor( 0,0,1,-1 ) ));
            } else S->sliderShape->placeTexture( calcTex( poColor( 1,1,1,-1 ) ));
            
            S->shapeLabel->visible = false;
            S->position = poPoint( 0,i*PO_CONTROL_SLIDER_HEIGHT );
            addChild( S );
            sliders.push_back( S );
        }
    }
    
    shapeLabel = new poTextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SPACER*4,3*PO_CONTROL_SLIDER_HEIGHT,0);
    shapeLabel->setTextSize(controlTextSize);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
    
    colorBox = new poRectShape( PO_CONTROL_SLIDER_HEIGHT*3,PO_CONTROL_SLIDER_HEIGHT*3 );
    colorBox->position.set( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SPACER*4,0,0);
    
    if (RGB) {
        colorBox->fillColor = valC;
    } else colorBox->fillColor.setHSV(valC.R, valC.G, valC.B);
	
    addChild( colorBox );
}

poTexture* poColorSlider::calcTex ( poColor input ) {
	
    poImage* I = new poImage( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT,4,NULL );
    
    for ( int x=0 ; x<PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT; x++) {
        for ( int y=0 ; y<PO_CONTROL_SLIDER_HEIGHT; y++) {
            float S,H,V,A,VAL;
            poColor C;
            VAL = (float)x/(float)(PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT);
            
            if ( input.R == -1 ) H = VAL; else H = input.R;
            if ( input.G == -1 ) S = VAL; else S = input.G;
            if ( input.B == -1 ) V = VAL; else V = input.B;
            if ( input.A == -1 ) A = VAL; else A = input.A;
         
            C.setHSV( H,S,V );
            C.A = A;
            
            if ( RGB ) {
                C = poColor( H,S,V,VAL);
            }
            
            
            I->setPixel( poPoint(x,y,0), C);
        }
    }
    poTexture *tex = new poTexture(I);
	delete I;
    return tex;
}

void poColorSlider::eventHandler(poEvent *event) {
}

void poColorSlider::messageHandler(const std::string &msg, const poDictionary& dict) {
    
//    cout << msg << endl;
    
    if ( RGB ) {
        if ( msg == ID+"_R" ) {
            valC.R = dict.getFloat( "value" );
            poSliderF* sR = (poSliderF*) getChild( msg );
            sR->sliderShape->placeTexture( calcTex( poColor( 1,0,0 )) );
            //cout << sR->back->fillColor << endl;
        } 
        
        else if ( msg == ID+"_G" ) {
            valC.G = dict.getFloat( "value" );
            poSliderF* sG = (poSliderF*) getChild( msg );
            sG->sliderShape->placeTexture( calcTex( poColor( 0,1,0 )) );
        }
        
        else if ( msg == ID+"_B" ) {
            valC.B = dict.getFloat( "value" );
            poSliderF* sB = (poSliderF*) getChild( msg );
            sB->sliderShape->placeTexture( calcTex( poColor( 0,0,1 )) );
        }
        
        else if ( msg == ID+"_A" ) {
            valC.A = dict.getFloat( "value" );
        }
		
        poSliderF* sA = (poSliderF*) getChild( ID+"_A" );
        sA->sliderShape->placeTexture( calcTex( poColor( 1,1,1,-1 )) );
        
        colorBox->fillColor = valC;
    }
	else {
        
        poSliderF* sH = (poSliderF*) getChild( ID+"_H" );
        poSliderF* sS = (poSliderF*) getChild( ID+"_S" );
        poSliderF* sV = (poSliderF*) getChild( ID+"_V" );
        poSliderF* sA = (poSliderF*) getChild( ID+"_V" );
        
        if ( msg == ID+"_H" ) {
            valC.R = dict.getFloat( "value" );
            valC.R = poClamp( 0.0001f, 0.9999f, valC.R);
        }
        
        if ( msg == ID+"_S" ) {
            valC.G = dict.getFloat( "value" );
        }
        
        if ( msg == ID+"_V" ) {
            valC.B = dict.getFloat( "value" );
        }
        
        else if ( msg == ID+"_A" ) {
            valC.A = dict.getFloat( "value" );
        }
        
        sH->sliderShape->placeTexture( calcTex( poColor( -1,valC.G,valC.B )) ); 
        sS->sliderShape->placeTexture( calcTex( poColor( valC.R,-1,valC.B )) );
        sV->sliderShape->placeTexture( calcTex( poColor( valC.R,valC.G,-1 )) );
        
        poSliderF* Sa = (poSliderF*) getChild( ID+"_A" );
        Sa->sliderShape->placeTexture( calcTex( poColor( 0,0,1,-1 )) );
        colorBox->fillColor.setHSV( valC.R,valC.G,valC.B );
    }
    
    poDictionary D;
    D.set("value", valC);
    
    if ( listener == NULL );
        //getParent()->messageHandler( ID, D);
    else
        listener->messageHandler( ID, D);
	
	D.set("control", this);
	D.set("valueType", "color");
	getParent()->messageHandler("update_settings", D);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poKnob::poKnob( string _ID, float init, float _min, float _max, poObject* _listener ) : poControl( _ID, _listener ) {
    
    type = PO_CONTROL_TYPE_FLOAT;
    
	this->name = ID;
    min = _min;
    max = _max;
	
    valF = poClamp(min, max, init);
    
    char valString [256];
    sprintf( valString, "%.2lf", valF );
    
    addEvent( PO_MOUSE_DOWN_EVENT, this );
    
    sliderShape = new poOvalShape( PO_CONTROL_SLIDER_HEIGHT*3,PO_CONTROL_SLIDER_HEIGHT*3,30 );;
    sliderShape->fillColor = poColor(1,1,1,0.2);
    sliderKnob = new poRectShape( PO_CONTROL_SLIDER_HEIGHT*4/3,3);
    sliderKnob->fillColor = poColor(1,0,0,0.5);
    sliderKnob->position = poPoint(0,0); 
    sliderKnob->setAlignment(PO_ALIGN_CENTER_RIGHT);
    sliderKnob->rotation = valF;
    sliderShape->addEvent( PO_MOUSE_DRAG_INSIDE_EVENT, this );
    sliderShape->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this );
    
    addChild( sliderShape );
    sliderShape->addChild( sliderKnob );
    sliderShape->position = poPoint( sliderShape->getWidth()/2,sliderShape->getWidth()/2 ) ;
	
    shapeData = new poTextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
	shapeData->textColor = poColor::white;
    shapeData->position.set(PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT*3,0);
    shapeData->setTextSize(controlTextSize);
    shapeData->setTextAlignment(PO_ALIGN_CENTER_CENTER);
    shapeData->setText(valString);
	shapeData->doLayout();  
    addChild( shapeData );
    
    shapeLabel = new poTextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SPACER*4,0,0);
    shapeLabel->setTextSize(controlTextSize);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
}

void poKnob::eventHandler(poEvent *event) {
	
    if (event->type == PO_MOUSE_DRAG_INSIDE_EVENT ) {
        poPoint P = event->globalPosition;
        P.y = getWindowHeight() - P.y;
        poPoint pos = sliderShape->globalToLocal( P );
        
        //angle to mouse
        float ang = atan2( pos.y, pos.x ) * 180/3.14159;
        ang = 180+ang;
		
        sliderKnob->rotation = ang;
		
		valF = poMapf(0.f, 360.f, ang, min, max);
		
        char valString [256];
        sprintf( valString, "%.2lf", valF );
        shapeData->setText( valString );
        shapeData->doLayout();  
        
		poDictionary D;
        D.set("value", valF);
        
        if ( listener == NULL ) {
            //getParent()->messageHandler( ID, D);
        }
        else
            listener->messageHandler( ID, D);
		
		D.set("control", this);
		D.set("valueType", "float");
		getParent()->messageHandler("update_settings", D);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poButton::poButton( string _ID, poObject* _listener ) : poControl( _ID, _listener ) {
    
    type = PO_CONTROL_TYPE_MESSAGE;
    
    this->name = ID;
    addEvent( PO_MOUSE_DOWN_EVENT, this );
    
    shapeLabel = new poTextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( 0,0,0);
    shapeLabel->setPadding( PO_CONTROL_SPACER );
    shapeLabel->setTextSize(controlTextSize);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
    
    back = new poRectShape( shapeLabel->getTextBounds().width+PO_CONTROL_SPACER*2,shapeLabel->getTextBounds().height+PO_CONTROL_SPACER*2 );
    back->position = poPoint(PO_CONTROL_SPACER/2,PO_CONTROL_SPACER/2);
    back->fillColor = poColor(1,1,1,0.2);
    back->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this );
    shapeLabel->addChild( back );
    
    poRectShape* space = new poRectShape( back->getWidth(), back->getHeight() );
    space->fillColor = poColor(0,0,0,0);
    addChild( space );
}

void poButton::eventHandler(poEvent *event) {
    
    if (event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
        
        poDictionary D;
        D.set("value", ID);
        
        if ( listener == NULL ) {
            //getParent()->messageHandler( ID, D);
        }
        else
            listener->messageHandler( ID, D);
    }
}
