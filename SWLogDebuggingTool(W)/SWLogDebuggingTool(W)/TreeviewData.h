#pragma once
#include <list>
#include <string>
using namespace std;

class TreeviewData
{
public:
	TreeviewData(list<CString> directory, CString filename);
	~TreeviewData(void);

	void setFullDirectory(list<CString> full);
	list<CString> getFullDirectory();
	void setFileName(CString filename);
	CString getFileName();

	void setListIndex(int index);
	int getListIndex();

private:
	int iListIndex;
	list<CString> cslFullDirectory;
	CString csFileName;
};