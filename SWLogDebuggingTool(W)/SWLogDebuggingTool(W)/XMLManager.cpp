#include "StdAfx.h"
#include "XMLManager.h"

XMLManager::XMLManager(void)
{
}

XMLManager::~XMLManager(void)
{
}

void XMLManager::initXML()
{
	sConfigFileDirectory = mUserConfig.GetExeDirectory() + "Config.xml";

	if (mXMLDocument.LoadFile(sConfigFileDirectory.c_str()) == false)
	{

	}
	else
	{
		string title = "CommonPath";
		string childelement[2] = {"Agent", "Watcher"};
		string contents[2] = {"c:\\Temp\\CoreDebug\\", "c:\\LogDebugging\\"};

		WriteXML(title, childelement, contents, sizeof(contents)/sizeof(contents[0]));
	}
}

bool XMLManager::WriteXML(string NodeTitle, string* ChildElement, string* Contents, int ChildCnt)
{

	mNode = mXMLDocument.NewElement(NodeTitle.c_str());
	mXMLDocument.InsertFirstChild(mNode);

	if (ChildCnt <= 0)
	{

	} 
	else
	{
		for (int i=0; i<ChildCnt; i++)
		{
			mElement = mXMLDocument.NewElement((ChildElement[i]).c_str());
			mElement->SetText((Contents[i]).c_str());
			mNode->InsertEndChild(mElement);
		}
	}

	if (mXMLDocument.SaveFile(sConfigFileDirectory.c_str()) == false)
	{
		return false;
	} 
	else
	{
		return true;
	}

	mXMLDocument.Clear();

}

bool XMLManager::AddXML(string NodeTitle, string* ChildElement, string* Contents, int ChildCnt)
{
	if (mXMLDocument.LoadFile(sConfigFileDirectory.c_str()) == false)
	{
		WriteXML(NodeTitle, ChildElement, Contents, ChildCnt);
		return false;
	} 
	else
	{
		WriteXML(NodeTitle, ChildElement, Contents, ChildCnt);
		return true;
	}
}

bool XMLManager::EditElementXML(string ChildTitle, string ChildElement, string Contents)
{
	if (mXMLDocument.LoadFile(sConfigFileDirectory.c_str()) == false)
	{
		mNode = mXMLDocument.FirstChildElement(ChildTitle.c_str())->FirstChildElement(ChildElement.c_str());
		mNode->ToElement()->SetText(Contents.c_str());

		mXMLDocument.SaveFile(sConfigFileDirectory.c_str());
		mXMLDocument.Clear();
		return false;
	} 
	else
	{

		return true;
	}
}

string XMLManager::ParsingXML(string ChildTitle, string ChildElement)
{
	if (mXMLDocument.LoadFile(sConfigFileDirectory.c_str()) == false)
	{
		mNode = mXMLDocument.FirstChildElement(ChildTitle.c_str())->FirstChildElement(ChildElement.c_str());

		return (string) mNode->ToElement()->GetText();
	} 
	else
	{
		return (string) sConfigFileDirectory.c_str();
	}
}