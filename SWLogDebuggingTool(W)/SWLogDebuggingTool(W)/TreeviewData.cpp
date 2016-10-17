#include "StdAfx.h"
#include "TreeviewData.h"

TreeviewData::TreeviewData()
{
	
}

TreeviewData::~TreeviewData(void)
{

}

void TreeviewData::setFullDirectory(CString full)
{
	this->csFullDirectory = full;
}
CString TreeviewData::getFullDirectory()
{
	return csFullDirectory;
}

void TreeviewData::setListIndex(int index)
{
	this->iListIndex = index;
}

int TreeviewData::getListIndex()
{
	return iListIndex;
}

