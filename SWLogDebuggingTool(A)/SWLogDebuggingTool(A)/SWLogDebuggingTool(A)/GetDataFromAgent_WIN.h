#include <stdio.h>
#include <windows.h>
#include "AgtInfoList.h"


#pragma once

class GetDataFromAgent_WIN : public AgtInfoList
{
public:
	GetDataFromAgent_WIN(void);
	~GetDataFromAgent_WIN(void);

	WIN32_FIND_DATA FindData;
	HANDLE hFind;
	HANDLE hFile;

	char* GetAgentName(int& iAgtNameSize);
	char* GetAgentIP();
	char* GetAgentFileInfo(char* cAgtLogPathBuf);
	char* GetLogData(char cLogFilePath[256], int& iCurrentPointOutput);
	
	//list<char*> GetAgentFileInfo_List(char* cAgtLogPathBuf);
};
