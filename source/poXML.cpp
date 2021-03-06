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

//
//  File.cpp
//  pocode
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poXML.h"
#include "poHelpers.h"

#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace pugi;

#define LCAST(type,val) boost::lexical_cast<type>(val)

#pragma mark - poXPathResult -
poXPathResult::poXPathResult(xpath_node_set nodes) 
:	nodes(nodes) 
{}

uint poXPathResult::getNumMatches() {
	return nodes.size();
}

poXMLNode poXPathResult::getNode(uint idx) {
	return poXMLNode(nodes[idx].node());
}

std::string poXPathResult::getString(uint idx) {
	return nodes[idx].attribute().value();
}

int poXPathResult::getInt(uint idx) {
	return nodes[idx].attribute().as_int();
}

float poXPathResult::getFloat(uint idx) {
	return nodes[idx].attribute().as_float();
}

#pragma mark - poXMLNode -
poXMLNode::poXMLNode() : node() {}

poXMLNode::poXMLNode(xml_node node)
:	node(node) 
{}

bool poXMLNode::isValid() const {
	return strlen(node.name()) > 0 || node.first_attribute() || !node.empty();
}

std::string poXMLNode::getName() const {
	return node.name();
}

poXMLNode& poXMLNode::setName(const std::string &str) {
	node.set_name(str.c_str());
	return *this;
}

int poXMLNode::getInnerInt() const {
	return LCAST(int, node.child_value());
}

float poXMLNode::getInnerFloat() const {
	return LCAST(float, node.child_value());
}

std::string poXMLNode::getInnerString() const {
	return node.child_value();
}

poXMLNode& poXMLNode::setInnerString(const std::string &str) {
	for(xml_node::iterator iter=node.begin(); iter!=node.end(); iter++) {
		node.remove_child(*iter);
	}
	
	node.append_child(node_pcdata);
	node.first_child().set_value(str.c_str());
	return *this;
}

poXMLNode& poXMLNode::setInnerInt(int i) {
	if(!node.first_child())
		node.append_child(node_pcdata);
	
	node.first_child().set_value(LCAST(std::string,i).c_str());
	return *this;
}

poXMLNode& poXMLNode::setInnerFloat(float f) {
	if(!node.first_child())
		node.append_child(node_pcdata);
	
	node.first_child().set_value(LCAST(std::string,f).c_str());
	return *this;
}

uint poXMLNode::getNumAttributes() const {
	return std::distance(node.attributes_end(), node.attributes_begin());
}

bool poXMLNode::hasAttribute(const std::string &name) const {
	return node.attribute(name.c_str());
}

std::vector<std::string> poXMLNode::getAttributeNames() const {
	std::vector<std::string> response;

	for(xml_attribute_iterator i=node.attributes_begin(); i != node.attributes_end(); ++i) {
		response.push_back(i->name());
	}
	
	return response;
}

int poXMLNode::getIntAttribute(const std::string &name) const {
	return node.attribute(name.c_str()).as_int();
}

float poXMLNode::getFloatAttribute(const std::string &name) const {
	return node.attribute(name.c_str()).as_float();
}

std::string poXMLNode::getStringAttribute(const std::string &name) const {
	return node.attribute(name.c_str()).value();
}

poXMLNode& poXMLNode::setAttribute(const std::string &name, int value) {
	if(!hasAttribute(name))
		node.append_attribute(name.c_str());
	node.attribute(name.c_str()) = value;
	return *this;
}

poXMLNode& poXMLNode::setAttribute(const std::string &name, float value) {
	if(!hasAttribute(name))
		node.append_attribute(name.c_str());
	node.attribute(name.c_str()) = value;
	return *this;
}

poXMLNode& poXMLNode::setAttribute(const std::string &name, const std::string &value) {
	if(!hasAttribute(name))
		node.append_attribute(name.c_str());
	node.attribute(name.c_str()) = value.c_str();
	return *this;
}

poXMLNode& poXMLNode::removeAttribute(const std::string &name) {
	node.remove_attribute(name.c_str());
	return *this;
}

uint poXMLNode::getNumChildren() const {
	int size = 0;
	xml_node::iterator n = node.begin();
	while(n != node.end()) {
		size++;
		++n;
	};
	return size;
}

poXMLNode poXMLNode::addChild(const std::string &name) {
	return poXMLNode(node.append_child(name.c_str()));
}

poXMLNode &poXMLNode::addChild(const poXMLNode &node) {
	this->node.append_copy(node.node);
	return *this;
}

poXMLNode &poXMLNode::removeChild(const std::string &name) {
	node.remove_child(name.c_str());
	return *this;
}

poXMLNode &poXMLNode::removeChild(const poXMLNode &node) {
	this->node.remove_child(node.node);
	return *this;
}

poXMLNode poXMLNode::getChild(uint idx) {
	if(idx > getNumChildren())
		return poXMLNode();
	
	std::vector<poXMLNode> kids = getChildren();
	return kids[idx];
}


poXMLNode poXMLNode::getChild(const std::string &name) {
	return poXMLNode(node.child(name.c_str()));
}

std::vector<poXMLNode> poXMLNode::getChildren() {
	std::vector<poXMLNode> response;
	xml_node n = node.first_child();
	while(n) {
		response.push_back(poXMLNode(n));
		n = n.next_sibling();
	}
	return response;
}

std::vector<poXMLNode> poXMLNode::getChildren(const std::string &name) {
	std::vector<poXMLNode> response;
	xml_node n = node.child(name.c_str());
	while(n) {
		response.push_back(poXMLNode(n));
		n = n.next_sibling(name.c_str());
	}
	return response;
}

poXMLNode poXMLNode::getFirstChild() {
	return poXMLNode(node.first_child());
}

poXMLNode poXMLNode::getLastChild() {
	return poXMLNode(node.last_child());
}

poXMLNode poXMLNode::getNextSibling() {
	return poXMLNode(node.next_sibling());
}

poXMLNode poXMLNode::getNextSibling(const std::string &name) {
	return poXMLNode(node.next_sibling(name.c_str()));
}

poXPathResult poXMLNode::find(const std::string &xpath) {
	return poXPathResult(node.select_nodes(xpath.c_str()));
}

pugi::xml_node poXMLNode::getHandle() const {
	return node;
}

bool operator==(poXMLNode const& n1, poXMLNode const &n2) {
	return n1.getHandle() == n2.getHandle();
}

#pragma mark - poXMLDocument -

poXMLDocument::poXMLDocument() {
	document.reset(new pugi::xml_document);
	document->append_child("root");
}

poXMLDocument::poXMLDocument(const std::string &url) {
	document.reset(new pugi::xml_document);
	read(url);
}

bool poXMLDocument::isValid() const {
	return document->first_child() != NULL;
}

poXMLNode poXMLDocument::getRootNode() const {
    if(!isValid())
        return poXMLNode();
    
	return poXMLNode(document->first_child());
}

poXMLNode poXMLDocument::resetRootNode() {
	document.reset(new pugi::xml_document);
	return poXMLNode(document->append_child());
}

bool poXMLDocument::read(const std::string &url) {
	document.reset(new pugi::xml_document);
	xml_parse_result result = document->load_file(url.c_str(), parse_default, encoding_utf8);
    if(result.status == status_ok) {
        return true;
    }
    
    log("poXML: parse error (file: %s) (error: %s)", url.c_str(), result.description());
    return false;
}

bool poXMLDocument::readStr(const std::string &str) {
	document.reset(new pugi::xml_document);
	xml_parse_result result = document->load(str.c_str());
    if(result.status == status_ok) {
        return true;
    }
    
    log("poXML: parse error (error: %s)", result.description());
    return false;
}

bool poXMLDocument::write(const std::string &url) {
	return document->save_file(url.c_str());
}

void poXMLDocument::print() const {
	document->print(std::cout);
}

pugi::xml_document &poXMLDocument::getHandle() const {
	return *document.get();
}

poXMLDocument poXMLDocument::copy() {
	poXMLDocument doc;
	doc.document.reset(new pugi::xml_document);
	doc.document->reset(*document);
	return doc;
}
