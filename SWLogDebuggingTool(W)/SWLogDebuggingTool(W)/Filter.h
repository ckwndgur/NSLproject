#include <Windows.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
using namespace std;

class Filter{
public:
	Filter();
	~Filter();
	void DoFilter(int Category,string WantedLog,string Title, CString filepath);
	string CreatingTime(string WantedLog);

	string WantedLog;
};