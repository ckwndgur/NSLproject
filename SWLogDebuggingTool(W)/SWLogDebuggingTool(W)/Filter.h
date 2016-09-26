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
	int DoFilter();
	void Init();
private:
	string WantedLog;
	string Log;
	string TargetPart;
	string FilteredLog;
	int Category;
};