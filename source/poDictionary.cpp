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

#include "poDictionary.h"
#include <iomanip>
#include <sstream>

using namespace std;
using namespace boost;

namespace {
	
	struct item_printer : public static_visitor<> {
		ostream &out;
		item_printer(ostream &os) : out(os) {}
		
		void operator()(const int& i) const			{out << i;}
		void operator()(const float& d) const		{out << std::fixed << d;}
		void operator()(const string& s) const		{out << "'" << s << "'";}
		void operator()(const poPoint& p) const		{out << p;}
		void operator()(const poColor& c) const		{out << c;}
		void operator()(const void* p) const		{out << "&" << hex << (size_t)p;}
		void operator()(const poDictionary& d) const{out << d;}
	};
	
	struct string_converter : public static_visitor<string> {
		ostringstream ss;
		
		string operator()(int& i)			{ss << i; return ss.str();}
		string operator()(float& d)			{ss << std::fixed << d; return ss.str();}
		string operator()(string& s)		{ss << "'" << s << "'"; return ss.str();}
		string operator()(poPoint& p)		{ss << p; return ss.str();}
		string operator()(poColor& c)		{ss << c; return ss.str();}
		string operator()(void* p)			{ss << "&" << hex << (size_t)p; return ss.str();}
		string operator()(poDictionary& d)	{ss << d; return ss.str();}
	};
	
}

poDictionaryItem::poDictionaryItem()
: item(0)
{}
poDictionaryItem::poDictionaryItem(const poDictionaryItem_t &item) 
: item(item)
{}
bool poDictionaryItem::getBool() const {
	return (bool)boost::get<int>(item);
}
int poDictionaryItem::getInt() const {
	return boost::get<int>(item);
}
float poDictionaryItem::getFloat() const {
	return boost::get<float>(item);
}
string poDictionaryItem::getString() const {
	return boost::get<string>(item);
}
poPoint poDictionaryItem::getPoint() const {
	return boost::get<poPoint>(item);
}
poColor poDictionaryItem::getColor() const {
	return boost::get<poColor>(item);
}
void* poDictionaryItem::getPtr() const {
	return boost::get<void*>(item);
}
poDictionary poDictionaryItem::getDictionary() const {
	return boost::get<poDictionary>(item);
}
poDictionaryType poDictionaryItem::getType() const {
	return (poDictionaryType)item.which();
}
void poDictionaryItem::setBool(const poDictionaryItem_t &i) {
	item = i;
}
string poDictionaryItem::toString() {
	string_converter conv;
	return boost::apply_visitor(conv, item);
}


poDictionary::poDictionary() {
}
poDictionary::poDictionary(std::string const k, poDictionaryItem_t const& v) {
	set(k,v);
}
poDictionary poDictionary::copy() {
	poDictionary dict;
	for(poDictionaryItemMap::iterator iter=begin(); iter!=end(); ++iter) {
		if(iter->second.getType() == PO_DICTIONARY_T)
			dict.set(iter->first, iter->second.getDictionary().copy());
		else {
			dict.items[iter->first] = iter->second;
		}
	}
	return dict;
}
bool poDictionary::getBool(const string &s, bool d) const {
	try {
		return items.at(s).getBool();
	}
	catch(...) {
		return d;
	}
}
int poDictionary::getInt(const string &s, int d) const {
	try {
		return items.at(s).getInt();
	} catch(...) {return d;}
}
float poDictionary::getFloat(const string &s, float d) const {
	try {
		return items.at(s).getFloat();
	} catch(...) {return d;}
}
string poDictionary::getString(const string &s, string const& d) const {
	try {
		return items.at(s).getString();
	} catch(...) {return d;}
}
poPoint poDictionary::getPoint(const std::string &s, poPoint d) const {
	try {
		return items.at(s).getPoint();
	} catch(...) {return d;}
}
poColor poDictionary::getColor(const std::string &s, poColor d) const {
	try {
		return items.at(s).getColor();
	} catch(...) {return d;}
}
void* poDictionary::getPtr(const string &s, void* d) const {
	try {
		return items.at(s).getPtr();
	} catch(...) {return d;}
}
poDictionary poDictionary::getDictionary(const string &s, poDictionary d) const {
	try {
		return items.at(s).getDictionary();
	} catch(...) {return d;}
}
poDictionaryType poDictionary::getType(const string &s, poDictionaryType d) const {
	try {
		return items.at(s).getType();
	} catch(...) {return d;}
}
poDictionaryItem poDictionary::get(const string &s, poDictionaryItem d) const {
	try {
		return items.at(s);
	} catch(...) {return d;}
}
poDictionary& poDictionary::set(const string &s, const poDictionaryItem_t &di) {
	items[s] = di;
	return *this;
}
poDictionary& poDictionary::append(const poDictionary &d) {
	poDictionaryItemMap::const_iterator iter = d.begin();
	for(; iter!=d.end(); ++iter) {
		items[iter->first] = iter->second;
	}
	return *this;
}
bool poDictionary::has(const string &s) const {
	return items.count(s) > 0;
}
size_t poDictionary::count() const {
	return items.size();
}
vector<string> poDictionary::keys() const {
	vector<string> response;
	for(poDictionaryItemMap::const_iterator iter=begin(); iter!=end(); iter++) {
		response.push_back(iter->first);
	}
	return response;
}
void poDictionary::write(poXMLNode node) {
	node.setName("item");
	for(poDictionaryItemMap::iterator iter=items.begin(); iter!=items.end(); ++iter) {
		poXMLNode item = node.addChild("item");
		
		item.setAttribute("name", iter->first);
		item.setAttribute("type", iter->second.getType());

		if(iter->second.getType() == PO_DICTIONARY_T) {
			poDictionary dict = iter->second.getDictionary();
			dict.write(item);
		}
		else {
			item.setInnerString(iter->second.toString().c_str());
		}
	}
}
void poDictionary::write(poXMLDocument &doc) {
	poXMLNode root = doc.getRootNode();
	root.setAttribute("name", "root");
	root.setAttribute("type", PO_DICTIONARY_T);
	write(root);
}
void poDictionary::write(const std::string &url) {
	poXMLDocument doc;
	write(doc);
	doc.write(url.c_str());
}
void poDictionary::read(poXMLNode node) {
	int type = node.getIntAttribute("type");
	std::string name = node.getStringAttribute("name");
	
	switch(type) {
		case PO_INT_T:
			set(name, node.getInnerInt());
			break;
		case PO_FLOAT_T:
			set(name, node.getInnerFloat());
			break;
		case PO_STRING_T:
			set(name, node.getInnerString());
			break;
		case PO_POINT_T: {
			poPoint p;
			if(p.fromString(node.getInnerString()))
				set(name, p);
			break;
		}
		case PO_COLOR_T: {
			poColor c;
			if(c.set(node.getInnerString()))
				set(name, c);
			break;
		}
		case PO_VOID_PTR_T:
			printf("reading void ptr from dictionary not supported; inserting address as string, though its basically meaningless\n");
			set(name, node.getInnerString());
			break;
		case PO_DICTIONARY_T: {
			poDictionary dict;
			poXMLNode item = node.getFirstChild();
			while(item.isValid()) {
				dict.read(item);
				item = item.getNextSibling();
			}
			set(node.getStringAttribute("name"), dict);
			break;
		}
	}
}
void poDictionary::read(const std::string &url) {
	poXMLDocument doc(url);
	if(doc.isValid()) {
		poXMLNode item = doc.getRootNode().getFirstChild();
		while(item.isValid()) {
			read(item);
			item = item.getNextSibling();
		}
	}
}
poDictionaryItemMap::iterator poDictionary::begin() {
	return items.begin();
}
poDictionaryItemMap::iterator poDictionary::end() {
	return items.end();
}
poDictionaryItemMap::const_iterator poDictionary::begin() const {
	return items.begin();
}
poDictionaryItemMap::const_iterator poDictionary::end() const {
	return items.end();
}


// stream-out operator for poDictionaryItem
ostream& operator<<(ostream &out, const poDictionaryItem& item) {
	boost::apply_visitor(item_printer(out), item.item);
	return out;
}

// stream-out operator for poDictionary
ostream& operator<<(ostream &out, const poDictionary &d) {
	out << "(";
	poDictionaryItemMap::const_iterator iter=d.begin();
	while(iter!=d.end()) {
		out << "'" << iter->first << "':" << iter->second;
		if(++iter != d.end())
			out << ",";
	}
	return out << ") ";
}
