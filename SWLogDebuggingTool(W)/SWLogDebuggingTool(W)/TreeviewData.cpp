#include "StdAfx.h"
#include "TreeviewData.h"

TreeviewData::TreeviewData(list<CString> directory, CString filename)
{
	this->cslFullDirectory = directory;
	this->csFileName = filename;
}

TreeviewData::~TreeviewData(void)
{

}

void TreeviewData::setFullDirectory(list<CString> full)
{
	this->cslFullDirectory = full;
}
list<CString> TreeviewData::getFullDirectory()
{
	return cslFullDirectory;
}

void TreeviewData::setListIndex(int index)
{
	this->iListIndex = index;
}

int TreeviewData::getListIndex()
{
	return iListIndex;
}

