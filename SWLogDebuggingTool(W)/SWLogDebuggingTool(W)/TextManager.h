#pragma once
#include <string>
#include <windows.h>
#include "StringManager.h"
using namespace std;

class TextManager
{
public:
	TextManager(void);
	~TextManager(void);
	
	void WriteText(char* filedirectory, char* filename, string data, bool lastflag);
	int GetFileSize(char* filedirectory, char* filename);
	void MakeDirectory(char* filedirectory, char* filename);

	string ReadText(char* filedirectory, char* filename);
	
	/*
	void WriteText(char* filedirectory, char* filename, string data);
	int GetFileSize(char* filedirectory, char* filename);

	string ReadText(char* filedirectory, char* filename);
	*/

private:
	StringManager mStringManager;
};
