//
//  poXML_test.cpp
//  potionCode
//
//  Created by filippo on 10/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include "poXML.h"

using namespace pugi;

std::vector<std::string> numeric_names(int start, int end, std::string suffix="");
std::vector<std::string> numeric_names(int start, int end, std::string suffix)
{
	std::vector<std::string> names;
	for(int i=start; i <= end; i++)
	{
		std::string str = (boost::format("%d")%i).str();
		if(!suffix.empty()) str = suffix + str;
		names.push_back(str);
	}
	return names;
}

std::vector<std::string> alphabetic_names(int start, int end, std::string suffix="");
std::vector<std::string> alphabetic_names(int start, int end, std::string suffix)
{
	std::vector<std::string> names;
	for(int i=0; i < (end - start)+1; i++)
	{
		char c[16];
		sprintf(c, "%c", start+i);
//		printf("%c",c);
		std::string str = (boost::format("%c")%c).str();
		if(!suffix.empty()) str = suffix + str;
		names.push_back(str);
	}
	return names;
}

void nestChildNode(poXMLNode parent, std::vector<std::string> names, int index);
void nestChildNode(poXMLNode parent, std::vector<std::string> names, int index)
{
	if(index < names.size())
	{
		poXMLNode child = parent.addChild(names[index]);
		index++;
		nestChildNode(child, names, index);
	}
}

BOOST_AUTO_TEST_CASE( poXMLTest ) {
	
//	poXMLDocument xmlDoc = poXMLDocument();
//	poXMLNode rootNode = xmlDoc.rootNode();
//	
//	poXMLNode invalidNode;
//	pugi::xml_node invalid_node = invalidNode.handle();
//	BOOST_CHECK((bool)invalid_node == false);
////	BOOST_CHECK(invalidNode.isValid() == (bool)invalid_node);
//	
//	poXMLNode noChildNode = rootNode.addChild("noChild");
//	BOOST_CHECK(noChildNode.numChildren() == 0);
//	
//	poXMLNode withChildNode = rootNode.addChild("withChild");
//	poXMLNode childNode = withChildNode.addChild("a child");
//	BOOST_CHECK(withChildNode.numChildren() == 1);
//	BOOST_CHECK(childNode.name() == "a child");
//	BOOST_CHECK(withChildNode.getChild(0).name() == childNode.name());
//	
//	withChildNode.setInnerString("trying to add content to a node with a child");
//	BOOST_CHECK(withChildNode.innerString().empty() == true);
//	
//	poXMLNode withChildAndContentNode = rootNode.addChild("withChildAndContent");
//	withChildAndContentNode.setInnerString("adding content first and then a child");
//	BOOST_CHECK(withChildAndContentNode.innerString() == "adding content first and then a child");
//	
//	poXMLNode anotherChildNode = withChildAndContentNode.addChild("another child");
//	BOOST_CHECK(withChildAndContentNode.innerString() == "adding content first and then a child");
//	BOOST_CHECK(anotherChildNode.name() == "another child");
//	BOOST_CHECK(withChildAndContentNode.numChildren() == 1);
//	BOOST_CHECK(withChildAndContentNode.getChild(0).name() == anotherChildNode.name());
//		
//	poXMLNode numbersNode = rootNode.addChild("numbers");
//	nestChildNode(numbersNode, numeric_names(0, 10, "number"), 0);
//	BOOST_CHECK(numbersNode.getChildren().size() == 1);
//	
//	for(int i=1; i < 10; i++)
//	{
//		char c[16];
//		sprintf(c, "%d", i);
//		numbersNode.addChild(c);
//		BOOST_CHECK(numbersNode.getChild(c).name() == numbersNode.getChild(i).name());
//		BOOST_CHECK(numbersNode.getChild(c).handle() == numbersNode.getChild(i).handle());
//	}
//	BOOST_CHECK(numbersNode.getChildren().size() == 10);
//	
//	poXMLNode lettersNode = rootNode.addChild("letters");
//	nestChildNode(lettersNode, alphabetic_names('A', 'Z'), 0);
//	
//	poXMLNode INode = rootNode.find("//letters/A/B/C/D/E/F/G/H/I").getNode(0);
//	BOOST_CHECK(INode.name() == "I");
//	
//	INode.setName("1");
//	BOOST_CHECK(INode.name() == "1");
//	
//	INode.addAttribute("letter", "no");
//	BOOST_CHECK(INode.stringAttribute("letter") == "no");
//	
//	INode.addAttribute("number", 123);
//	BOOST_CHECK(INode.intAttribute("number") == 123);
//	BOOST_CHECK(INode.stringAttribute("number") == "123");
//	
//	INode.setAttribute("number", 123.999f);
//	BOOST_CHECK(INode.intAttribute("number") == 123);
//	BOOST_CHECK(INode.floatAttribute("number") == 123.999);
//	
//	INode.addChild("J sibling");
//	INode.setInnerFloat(123.999f);
//	
//	poXMLNode JNode = INode.getChild(0);
//	BOOST_CHECK(JNode.name() == "J");
//	
//	poXMLNode JsiblingNode = JNode.nextSibling();
//	BOOST_CHECK(JsiblingNode.name() == "J sibling");
//	
//	poXMLNode sevenNode = numbersNode.getChild("7");
//	sevenNode.addAttribute("value", 7);
//	sevenNode.setInnerString("seven");
//	sevenNode.setInnerInt(7);
//	sevenNode.setInnerFloat(7.f);
	
	
//	xmlDoc.write("test.xml");
//	xmlDoc.print();
}