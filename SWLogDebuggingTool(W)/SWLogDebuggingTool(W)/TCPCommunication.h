#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <string>
#include <sstream>

#include <vector>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <time.h>

#include "AgtDataMsg.h"
#include "AgtInfoList.h"
#include "AgtInfoMsg.h"
#include "DataReqMsg.h"
#include "TextManager.h"
#include "FolderManager.h"
#include "XMLManager.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#pragma once

class TCPCommunication
{
public:
	TCPCommunication(void);
	~TCPCommunication(void);

	FolderManager mFolderManager;
	TextManager mTextManager;

	void TCPSockInit(int& iSndSock, int& iRcvSock);
	void LogFileRcv(int iRcvSocket, char* cFileDir, char* cFileName);

};
