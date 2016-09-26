#pragma once
#include <string>
#include "tinyxml2.h"
#include "StringManager.h"
#include "UserConfig.h"

using namespace std;

class XMLManager
{
public:
	XMLManager(void);
	~XMLManager(void);


	void initXML();

	bool WriteXML(string NodeTitle, string* ChildElement, string* Contents, int ChildCnt);
	bool AddXML(string NodeTitle, string* ChildElement, string* Contents, int ChildCnt);
	bool EditElementXML(string ChildTitle, string ChildElement, string Contents);

	string ParsingXML(string ChildTitle, string ChildElement);


private:
	string sConfigFileDirectory;
	UserConfig mUserConfig;
	StringManager mStringManager;
	tinyxml2::XMLDocument mXMLDocument;
	tinyxml2::XMLNode *mNode;
	tinyxml2::XMLElement *mElement;
};
