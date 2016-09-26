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

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#pragma once

class UDPCommunication : public DataReqMsg, public AgtInfoList, public AgtInfoMsg, public TextManager, public FolderManager
{
public:
	UDPCommunication(void);
	~UDPCommunication(void);

	int recvfromTimeOut(SOCKET socket, long sec, long usec);
	void InitSocket_Wt(int& hRecvSock, int& hRecvUniSock, int& hSndSock);
	void MultiGroupRcvSet(int iRcvSocket, char* MultiGroupAddr, int MultiGroupPort);
	void InforReq(int iSndSock, int iWatcherPort, char* cMultiGroup);
	list<string> RcvInfor(int iRcvUniSock, int iTimeout_sec);

	void LogFileRcv(int iRcvSocket, char* cFileDir, char* cFileName);
	char* SndDataReq(int iSndSockUni, list<string> lIPandFileList);

	void DevideInfor(char cMessage[2048], char cPartIPIn[20], char cPartNameIn[20], char cPartPathIn[2048], char cPartListIn[2048]);
	int GetDate();
	int GetDate_DAY();

	WSADATA wsaData;
	SOCKADDR_IN addr;
	struct ip_mreq Rcv_Mreq;
	int iState;

	time_t tTimer;
	struct tm tTimer_St;
	int iToday;
	char cToday[10];

	PHOSTENT pHostInfo;
	char cHostName[64], cIPAddr[15];

	string sSlctAgtIP;
};
