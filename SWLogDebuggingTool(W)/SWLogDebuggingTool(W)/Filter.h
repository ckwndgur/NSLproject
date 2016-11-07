#include <Windows.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <list>
using namespace std;

class Filter{
public:
	Filter();
	~Filter();
	list<CString> DoFilter(int Category,string WantedLog,string Title, CString filepath);
	string CreatingTime(string WantedLog);

	string WantedLog;

	list<CString> cslstFilteredData;
};