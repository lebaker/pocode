//
//  potionCode
//
//  Created by Joshua Fisher on 10/11/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poFont.h"
#include "poTexture.h"
#include "poBitmapFont.h"
#include "poResourceStore.h"
#include <boost/functional/hash.hpp>

poFont *poGetFont(const std::string &url, int group) {
	return poGetFont(url, "", group);
}

poFont *poGetFont(const std::string &url, const std::string &style, int group) {
	poResourceStore *store = poResourceStore::get();
	poResourceLocator lookup = store->locatorForFont(url, style, group);
	poResource *found = store->findResource(lookup);
	if(found)
		return (poFont*)found;
	
	poFont *font = new poFont(url, style);
	store->addResource(lookup, font);
	return font;
}

poBitmapFont *poGetBitmapFont(const std::string &url, uint size, int group) {
	return poGetBitmapFont(poGetFont(url,group), size, group);
}

poBitmapFont *poGetBitmapFont(poFont* font, uint size, int group) {
	return poGetBitmapFont(font->getFamilyName(), font->getStyleName(), size, group);
}

poBitmapFont *poGetBitmapFont(const std::string &family, const std::string &style, uint size, int group) {
	poResourceStore *store = poResourceStore::get();
	poResourceLocator lookup = store->locatorForBitmapFont(family, style, size, group);
	poResource *found = store->findResource(lookup);
	if(found)
		return (poBitmapFont*)found;

	poBitmapFont* bmpFont = new poBitmapFont(family, style, size);
	store->addResource(lookup, bmpFont);
	return bmpFont;
}

poTexture *poGetTexture(const std::string &url, int group) {
	poResourceStore *store = poResourceStore::get();
	poResourceLocator lookup = store->locatorForTexture(url);
	poResource *found = store->findResource(lookup);
	if(found)
		return (poTexture*)found;
	
	poTexture *tex = new poTexture(url);
	store->addResource(lookup, tex);
	return tex;
}

void poDeleteResourceGroup(int group);
void poDeleteResourceType(const std::type_info &type);


static boost::hash<std::string> string_hasher;

poResourceStore *poResourceStore::get() {
	static poResourceStore *instance = NULL;
	if(!instance)
		instance = new poResourceStore;
	return instance;
}

poResourceStore::poResourceStore() {
}

poResourceLocator poResourceStore::locatorForFont(const std::string &url, const std::string &style, int group) {
	poResourceLocator lookup(0, group, typeid(poFont));
	lookup.hash = string_hasher(url+style);
	return lookup;
}

poResourceLocator poResourceStore::locatorForBitmapFont(const std::string &url, uint size, int group) {
	return locatorForBitmapFont(poGetFont(url,group), size, group);
}
poResourceLocator poResourceStore::locatorForBitmapFont(poFont* font, uint size, int group) {
	return locatorForBitmapFont(font->getFamilyName(), font->getStyleName(), size, group);
}
poResourceLocator poResourceStore::locatorForBitmapFont(const std::string &url, const std::string &style, uint size, int group) {
	poResourceLocator lookup(0, group, typeid(poBitmapFont));
	boost::hash_combine(lookup.hash, url+style);
	boost::hash_combine(lookup.hash, size);
	return lookup;
}
poResourceLocator poResourceStore::locatorForTexture(const std::string &url, int group) {
	poResourceLocator lookup(0, group, typeid(poTexture));
	lookup.hash = string_hasher(url);
	return lookup;
}

void poResourceStore::deleteResourceGroup(int group) {
	std::map<poResourceLocator,poResource*>::iterator iter = resources.begin();
	while(iter != resources.end()) {
		if(iter->first.group == group) {
			delete iter->second;
			resources.erase(iter++);
		}
		else
			iter++;
	}
}

void poResourceStore::deleteAllResourcesOfType(const std::type_info &type) {
	std::map<poResourceLocator,poResource*>::iterator iter = resources.begin();
	while(iter != resources.end()) {
		if(iter->first.type == type) {
			delete iter->second;
			resources.erase(iter++);
		}
		else
			iter++;
	}
}

poResource *poResourceStore::findResource(poResourceLocator locator) {
	std::map<poResourceLocator, poResource*>::iterator iter = resources.find(locator);
	if(iter != resources.end())
		return iter->second;
	return NULL;
}

void poResourceStore::addResource(poResourceLocator locator, poResource *resource) {
	if(!findResource(locator)) {
		resources[locator] = resource;
	}
}


