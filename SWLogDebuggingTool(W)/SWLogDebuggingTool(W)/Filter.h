#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <list>
using namespace std;

class Filter
{
public:
	Filter();
	~Filter();
	list<CString> DoFilter(int Category,string WantedLog,string Title, CString filepath, CString Rfilepath, bool multiflag);
	string CreatingTime(string WantedLog);

	string WantedLog;
	CString LogColumn;
	CString ResultPath;


	list<CString> cslstFilteredData;
};