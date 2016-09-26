#include <stdio.h>
#include <ws2tcpip.h>             //ip_mreq 구조체 정의되어있음
#include <windows.h>
#include <tchar.h>
#include <string>
#include <tchar.h>

#include <iostream>
#include <fstream>
#include <limits.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFSIZE 4096

#pragma once

using namespace std;

class Ucommunicatino_Agent
{
	struct sInfoMsgStruct
	{
		char *cAgtIPAddr;
		char *cAgtName;
		char *cAgtLogFilePath;
		char *cAgtFileList;
	};

public:
	Ucommunicatino_Agent(void);
	~Ucommunicatino_Agent(void);
	
	void InitSocket_Ag();
	int RcvMsgDscr();
	void SendAgentInfor();
	void SendLogData();

	WSADATA wsaData;
	SOCKET hRecvSock, hSndSock;
	SOCKADDR_IN addr, SndAddr;
	int state, strLen;
	struct ip_mreq Rcv_Mreq;
	FILE* fp;
	
	WIN32_FIND_DATA FindData;
	HANDLE hFind;
	HANDLE hFile;

	char cPath[255];
	char cSndBuf[BUFSIZE];
	char cFileBuf[BUFSIZE];

	string sRcvIP;
	string sMyIP;

	string sFilePath;
	string sHeader;

	PHOSTENT pHostInfo;
	char cHostName[50], cIPAddr[50];

	int multiTTL;
	int iTotal, iCurrent, iRead, iLoaded, iTempNum1, iTempNum2;

	DWORD iFSize, iLen, dTemp;

	char cWatcherIP[50];
};
