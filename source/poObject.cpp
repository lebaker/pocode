/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

poObject::poObject() 
:	_parent(NULL)
,	_name("")
,	_alpha(1.f)
,	_scale(1.f, 1.f, 1.f)
,	_position(0.f, 0.f, 0.f)
,	_rotation(0.f)
,	_rotation_axis(0.f, 0.f, 1.f)
,	_offset(0.f, 0.f, 0.f)
,	_bounds(0.f, 0.f, 0.f, 0.f)
,	_alignment(PO_ALIGN_TOP_LEFT)
,	_visible(true)
,	events(PO_LAST_EVENT)
,	_matrix_order(PO_MATRIX_ORDER_TRS)
,	_draw_order(0)
,	position_tween(&_position)
,	scale_tween(&_scale)
,	alpha_tween(&_alpha)
,	rotation_tween(&_rotation)
,	true_alpha(1.f)
{}

poObject::poObject(const std::string &name)
:	_parent(NULL)
,	_name(name)
,	_alpha(1.f)
,	_scale(1.f, 1.f, 1.f)
,	_position(0.f, 0.f, 0.f)
,	_rotation(0.f)
,	_rotation_axis(0.f, 0.f, 1.f)
,	_offset(0.f, 0.f, 0.f)
,	_bounds(0.f, 0.f, 0.f, 0.f)
,	_alignment(PO_ALIGN_TOP_LEFT)
,	_visible(true)
,	events(PO_LAST_EVENT)
,	_matrix_order(PO_MATRIX_ORDER_TRS)
,	_draw_order(0)
,	position_tween(&_position)
,	scale_tween(&_scale)
,	alpha_tween(&_alpha)
,	rotation_tween(&_rotation)
,	true_alpha(1.f)
{}

poObject::~poObject() {
	removeAllChildren(true);
}

void poObject::draw() {}
void poObject::update() {}
void poObject::eventHandler(poEvent *event) {}
void poObject::messageHandler(const std::string &msg, const poDictionary& dict) {}

int poObject::addEvent(int eventType, poObject *sink, std::string message, const poDictionary& dict) {
	if(!sink)
		sink = this;
	return poEventCenter::get()->registerForEvent(eventType, this, sink, message, dict);
}

void poObject::removeEvent(int event_id) {
	poEventCenter::get()->removeEvent(event_id);
}

void poObject::preDraw() {}
void poObject::postDraw() {}

void poObject::addChild(poObject* obj) {
	obj->_parent = this;
	children.push_back(obj);
}

void poObject::addChild(poObject *obj, int idx) {
	obj->_parent = this;
	children.insert(children.begin()+idx, obj);
}

bool poObject::removeChild(poObject* obj) {
	poObjectVec::iterator iter = std::find(children.begin(), children.end(), obj);
	bool found = iter != children.end();
	
	if(found) {
		(*iter)->_parent = NULL;
		children.erase(iter);
	}
	
	return found;
}

bool poObject::removeChild(int idx, bool and_delete) {
	if(idx < 0 || idx >= children.size())
		return false;

	children[idx]->_parent = NULL;

	if(and_delete)
		delete children[idx];
	
	children.erase(children.begin()+idx);
	
	return true;
}

void poObject::removeAllChildren(bool and_delete) {
	BOOST_FOREACH(poObject* obj, children) {
		obj->_parent = NULL;
		if(and_delete)
			delete obj;
	}
	children.clear();
}

int poObject::numChildren() const {
	return (int)children.size();
}

poObject* poObject::getChild(int idx) {
	if(idx < 0 || idx >= children.size())
		return NULL;
	return *(children.begin()+idx);
}

poObject* poObject::getChild(const std::string &name) {
	poObjectVec::iterator iter = std::find_if(children.begin(), children.end(), boost::bind(&poObject::name, _1) == name);
	if(iter != children.end())
		return *iter;
	return NULL;
}

int poObject::getChildIndex(poObject* obj) {
	poObjectVec::iterator iter = std::find(children.begin(), children.end(), obj);
	if(iter != children.end())
		return (int)std::distance(children.begin(), iter);
	return INVALID_INDEX;
}

void poObject::moveChildToFront(poObject* child) {
	removeChild(child);
	addChild(child, 0);
}

void poObject::moveChildToBack(poObject* child) {
	removeChild(child);
	addChild(child);
}

void poObject::moveChildForward(poObject* child) {
	int idx = getChildIndex(child);
	removeChild(child);
	addChild(child, std::max(idx-1, 0));
}

void poObject::moveChildBackward(poObject* child) {
	int idx = getChildIndex(child);
	removeChild(child);
	addChild(child, idx+1);
}


void    poObject::addModifier(poObjectModifier* mod)
{
    modifiers.push_back( mod );
}

bool poObject::removeModifier(poObjectModifier* mod) {
	poObjectModifierVec::iterator found = std::find(modifiers.begin(), modifiers.end(), mod);
	if(found != modifiers.end()) {
		modifiers.erase(found);
		return true;
	}
	return false;
}

void    poObject::removeAllModifiers(bool and_delete)
{
	if(and_delete) {
		BOOST_FOREACH(poObjectModifier* mod, modifiers) {
			delete mod;
		}
	}
    modifiers.clear();
}


bool poObject::pointInside(poPoint point, bool localize) {
	if(!visible())
		return false;
	
	if(localize)
		point = globalToLocal(point);
	
	return bounds().contains(point);
}

bool poObject::pointInside(float x, float y, float z, bool localize) {
	return pointInside(poPoint(x,y,z),localize);
}

poPoint poObject::globalToLocal(poPoint point) const {
	return matrices.globalToLocal(point);
}

poPoint poObject::localToGlobal(poPoint point) const {
	return matrices.localToGlobal(point);
}

poPoint poObject::objectToLocal(poObject* obj, poPoint point) const {
	point = obj->localToGlobal(point);
	return globalToLocal(point);
}

void poObject::setAlignment(poAlignment align) {
	_alignment = align;
}

poRect poObject::calculateBounds(bool include_children) {
	_bounds = poRect(0,0,0,0);
	
	if(include_children) {
		BOOST_FOREACH(poObject* obj, children) {
			obj->calculateBounds(include_children);
			poRect obj_b = obj->bounds();
			
			_bounds.include(objectToLocal(obj, obj_b.bottomLeft()));
			_bounds.include(objectToLocal(obj, obj_b.bottomRight()));
			_bounds.include(objectToLocal(obj, obj_b.topLeft()));
			_bounds.include(objectToLocal(obj, obj_b.topRight()));
		}
	}
	
	return _bounds;
}

poObject*		poObject::parent() {return _parent;}

std::string		poObject::name() const {return _name;}
poObject&		poObject::name(const std::string &str) {_name = str; return *this;}

float			poObject::alpha() const {return _alpha;}
poObject&		poObject::alpha(float f) {_alpha = f; return *this;}

poPoint			poObject::scale() const {return _scale;}
poObject&		poObject::scale(poPoint pt) {_scale = pt; return *this;}
poObject&		poObject::scale(float x, float y, float z) {return scale(poPoint(x,y,z));}

poPoint			poObject::position() const {return _position;}
poObject&		poObject::position(poPoint p) {_position = p; return *this;}
poObject&		poObject::position(float x, float y, float z) {return position(poPoint(x,y,z));}

float			poObject::rotation() const {return _rotation;}
poObject&		poObject::rotation(float f) {_rotation = f; return *this;}

poPoint			poObject::rotationAxis() const {return _rotation_axis;}
poObject&		poObject::rotationAxis(poPoint p) {_rotation_axis = p; return *this;}
poObject&		poObject::rotationAxis(float x, float y, float z) {return rotationAxis(poPoint(x,y,z));}

poPoint			poObject::offset() const {return _offset;}
poObject&		poObject::offset(poPoint p) {_offset = p; return *this;}
poObject&		poObject::offset(float x, float y, float z) {return offset(poPoint(x,y,z));}

poRect			poObject::bounds() const {return _bounds;}
poObject&		poObject::bounds(poRect r) {_bounds = r; return *this;}

poAlignment		poObject::alignment() const {return _alignment;}

bool			poObject::visible() const {return _visible;}
poObject&		poObject::visible(bool b) {_visible = b; return *this;}

poMatrixOrder	poObject::matrixOrder() const {return _matrix_order;}
poObject&		poObject::matrixOrder(poMatrixOrder o) {_matrix_order = o; return *this;}

int				poObject::drawOrder() const {return _draw_order;}

void poObject::_drawTree() {
	if(!visible())
		return;
	
	pushObjectMatrix();

    BOOST_FOREACH(poObjectModifier* mod, modifiers) {
        mod->setUp( this );
    }
    
	preDraw();
	
	draw();
	
	BOOST_FOREACH(poObject* obj, children) {
		obj->_drawTree();
	}

	postDraw();
    
    BOOST_FOREACH(poObjectModifier* mod, modifiers) {
        mod->setDown( this );
    }
	
	popObjectMatrix();
}

void poObject::_updateTree() {
	if(!visible())
		return;
	
	updateAllTweens();
	update();
	
	BOOST_FOREACH(poObject* obj, children) {
		obj->_updateTree();
	}
}

void poObject::_broadcastEvent(poEvent* event) {
	if(!visible())
		return;
	// make sure its even a valid event
	if(event->type < 0 || event->type >= PO_LAST_EVENT)
		return;

	// send it to all our children
	BOOST_REVERSE_FOREACH(poObject* obj, children) {
		obj->_broadcastEvent(event);
	}

	// localize the point only once
	poPoint local_point = globalToLocal(event->position);
	
	// handle every event like this we have
	BOOST_FOREACH(poEvent *e, events[event->type]) {
		localizeEvent(e, event, local_point);
		eventHandler(e);
	}
}

void poObject::updateAllTweens() {
	position_tween.update();
	scale_tween.update();
	alpha_tween.update();
	rotation_tween.update();
}

void poObject::pushObjectMatrix() {
	glPushMatrix();

	if(_parent) {
		true_alpha = _parent->true_alpha * _alpha;
		_draw_order = _parent->_draw_order + 1;
	}
	else {
		true_alpha = _alpha;
	}
	
	// now move depending on the matrix order
	switch(_draw_order) {
		case PO_MATRIX_ORDER_TRS:
			glTranslatef(_position.x, _position.y, _position.z);
			glRotatef(_rotation, _rotation_axis.x, _rotation_axis.y, _rotation_axis.z);
			glScalef(_scale.x, _scale.y, _scale.z);
			break;
			
		case PO_MATRIX_ORDER_RST:
			glRotatef(_rotation, _rotation_axis.x, _rotation_axis.y, _rotation_axis.z);
			glScalef(_scale.x, _scale.y, _scale.z);
			glTranslatef(_position.x, _position.y, _position.z);
			break;
	}
	
	// translate to the offset
	glTranslatef(_offset.x, _offset.y, _offset.z);

	matrices.capture();
}

void poObject::popObjectMatrix() {

	// and reset gl
	glPopMatrix();
}

void poObject::localizeEvent(poEvent *local_event, poEvent *global_event, poPoint localized_pt) {
	local_event->position = global_event->position;
	local_event->local_position = localized_pt;
	local_event->keyChar = global_event->keyChar;
	local_event->keyCode = global_event->keyCode;
	local_event->modifiers = global_event->modifiers;
	local_event->uid = global_event->uid;
	local_event->timestamp = global_event->timestamp;
	local_event->previous_position = global_event->previous_position;
	// don't touch the message or the dictionary
}
