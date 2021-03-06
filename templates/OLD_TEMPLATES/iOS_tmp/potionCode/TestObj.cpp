//
//  TestObj.cpp
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "TestObj.h"
#include "poCamera.h"
#include "poSimpleDrawing.h"

poObject *createObjectForID(uint id) {
	return new TestObj();
}

TestObj::TestObj() {
	addModifier(new poCamera2D());
}

void TestObj::draw() {
    po::setColor(poColor::red);
    po::drawFilledRect(poRect(0, 0, 200, 200));
}

void TestObj::eventHandler(poEvent *event) {
}




