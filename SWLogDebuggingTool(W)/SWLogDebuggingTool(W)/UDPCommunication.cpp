/*////////////////////////////////////////////////////////////////
Winsock�� ��� ����ϱ� ����
Project -> Property -> Linker -> Input -> Additional Dependence
�� ws2_32.lib�� �߰��Ͽ����ϴ�.

MFC������ printf�� fput���� ����� �Ұ����Ͽ� �ּ�ó�� �Ͽ����ϴ�.
////////////////////////////////////////////////////////////////*/


#include "StdAfx.h"
#include "UDPCommunication.h"

TextManager mTextManager = TextManager();
FolderManager mFolderManager = FolderManager();

UDPCommunication::UDPCommunication(void)
{
}

UDPCommunication::~UDPCommunication(void)
{
}

int UDPCommunication::GetDate()
{
	tTimer = time(NULL);

	localtime_s(&tTimer_St, &tTimer);

	iToday = (tTimer_St.tm_year + 1900) * 10000000000 + (tTimer_St.tm_mon + 1) * 100000000 + (tTimer_St.tm_mday)*1000000+(tTimer_St.tm_hour)*10000+(tTimer_St.tm_min)*100+(tTimer_St.tm_sec);	

	return iToday;
}

int UDPCommunication::GetDate_DAY()
{
	tTimer = time(NULL);

	localtime_s(&tTimer_St, &tTimer);

	iToday = (tTimer_St.tm_year + 1900) * 10000 + (tTimer_St.tm_mon + 1) * 100 + (tTimer_St.tm_mday);	

	return iToday;
}

int UDPCommunication::recvfromTimeOut(SOCKET socket, long sec, long usec)
{
	// Setup timeval variable
	timeval timeout;
	timeout.tv_sec = sec;
	timeout.tv_usec = usec;
	// Setup fd_set structure
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(socket, &fds);
	// Return value:
	// -1: error occured
	// 0: timed out
	// >0: data ready to be read
	return select(0, &fds, 0, 0, &timeout);
}

void UDPCommunication::WSAInit()
{
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	//if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	//	fputs("WSAStartup() error\n", stderr);
}

void UDPCommunication::InitSocket_Wt(int& hRecvSock, int& hRecvUniSock, int& hSndSock)
{
	//���ϻ���
	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hRecvSock == INVALID_SOCKET)
		AfxMessageBox("Sock1 Error");
	// 	else
	// 		printf("1st Socket Open Success\n");

	hRecvUniSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hRecvUniSock == INVALID_SOCKET)
		AfxMessageBox("Sock2 Error");	
	// 	else
	// 		printf("2nb Socket Open Success\n");

	hSndSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hSndSock == INVALID_SOCKET)
		AfxMessageBox("Sock3 Error");	
	// 	else
	// 		printf("3rd Socket Open Success\n");

	//hRecvSock, hSndSock, hRecvUniSock UDP������ ��������
}

void UDPCommunication::MultiGroupRcvSet(int iRcvSocket, char* MultiGroupAddr, int MultiGroupPort)
{
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(MultiGroupPort);
	
	bind(iRcvSocket, (SOCKADDR*)&addr, sizeof(addr));

	//ip_mreq����ü�� ������ �׷��� �ּ�����
	Rcv_Mreq.imr_multiaddr.s_addr = inet_addr(MultiGroupAddr);           //Ŭ���� D�� ��Ƽĳ��Ʈ �ּ�
	Rcv_Mreq.imr_interface.s_addr = htonl(INADDR_ANY); //�����ּ�

	//IP_ADD_MEMBERSHIP : ��Ƽĳ��Ʈ �׷쿡 ����
	iState = setsockopt(iRcvSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&Rcv_Mreq, sizeof(Rcv_Mreq));
	if(iState == SOCKET_ERROR)
		fputs("Rsetsockopt() error\n", stderr);
}


void UDPCommunication::InforReq(int iSndSock, int iWatcherPort, char* cMultiGroup)
{
	struct DataReqMsgStruct MyDataReqMsg;
	memset(&MyDataReqMsg, 0, sizeof(struct DataReqMsgStruct));

	int nError = gethostname(cHostName, sizeof(cHostName));

	if (nError == 0)
	{
		pHostInfo = gethostbyname(cHostName);
		// ip address �ľ�
		strcpy(cIPAddr, inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
	}
	int ttl = 15;
	int iyes = 1;
	int state = setsockopt(iSndSock,IPPROTO_IP,IP_MULTICAST_TTL,(char*)&ttl,sizeof(ttl));
	if(state == SOCKET_ERROR)
		fputs("sock reuse error\n", stderr);

	state = setsockopt(iSndSock, SOL_SOCKET,SO_REUSEADDR, (char*)&iyes, sizeof(iyes));
	if(state == SOCKET_ERROR)
		fputs("sock reuse error\n", stderr);

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(cMultiGroup);
	addr.sin_port = htons(iWatcherPort);
	
	MyDataReqMsg.nReqType = 0;
	memcpy(&MyDataReqMsg.cWatcherIP, cIPAddr, strlen(cIPAddr));
	MyDataReqMsg.iWatcherPort = iWatcherPort;

	nError = sendto(iSndSock, (char*)&MyDataReqMsg, sizeof(struct DataReqMsgStruct), 0, (SOCKADDR*)&addr, sizeof(addr));
 }

void UDPCommunication::ResourceReq(int iSndSock, int iWatcherPort, char* cMultiGroup)
{
	struct DataReqMsgStruct MyDataReqMsg;
	memset(&MyDataReqMsg, 0, sizeof(struct DataReqMsgStruct));

	int nError = gethostname(cHostName, sizeof(cHostName));

	if (nError == 0)
	{
		pHostInfo = gethostbyname(cHostName);
		// ip address �ľ�
		strcpy(cIPAddr, inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(cMultiGroup);
	addr.sin_port = htons(iWatcherPort);
	MyDataReqMsg.nReqType = 1;
	memcpy(&MyDataReqMsg.cWatcherIP, cIPAddr, strlen(cIPAddr));
	MyDataReqMsg.iWatcherPort = iWatcherPort;

	sendto(iSndSock, (char*)&MyDataReqMsg, sizeof(struct DataReqMsgStruct), 0, (SOCKADDR*)&addr, sizeof(addr));
}

void UDPCommunication::LogFileRcv(int iRcvSocket, char* cFileDir, char* cFileName)
{
	struct AgtDataMsgStruct MyAgtDataMsg;
	DWORD dLen;

	memset(&MyAgtDataMsg, 0, sizeof(struct AgtDataMsgStruct));

	string sWriteData = "";

	mFolderManager.MakeDirectory(cFileDir, cFileName);

	int res = recvfromTimeOut(iRcvSocket, 5, 0);

	while(recvfromTimeOut(iRcvSocket, 5, 0)!=0)
	{
		switch(res)
		{
		case 0:
			Sleep(1);
			break;
		case -1:
			//printf("RcvTimeOut Error\n");
			break;

		default:
			recvfrom(iRcvSocket, (char*)&MyAgtDataMsg, sizeof(struct AgtDataMsgStruct), 0, NULL, 0);

			sWriteData = "";
			sWriteData = MyAgtDataMsg.cLogData;

			//WriteFile(hFile, sWriteData.c_str(), strlen(sWriteData.c_str()), &dLen, 0);
			mTextManager.WriteText(cFileDir, cFileName, sWriteData, false);

			if(MyAgtDataMsg.bLastPacket == TRUE)
			{
				//printf("File Download Complete.\n");
				AfxMessageBox("File Download Done");
				break;
			}
		}
	}
}

//���� ������Ʈ�� �������� ����� IP/�α����ϰ��/��ο� �����ϴ� ���� ��� �� ��û�ϱ� ���� �Լ�
list<string> UDPCommunication::RcvInfor(int iRcvUniSock, int iTimeout_sec)
{
	char cRcvBuf[4096];
	//char* cSingleFileName;

	memset(cRcvBuf, 0, sizeof(cRcvBuf));

	AgtInfoMsgStruct MyAgtInfoMsg = AgtInfoMsgStruct();
	AgtInfoList MyAgtInfoList = AgtInfoList();

	int iListStartFlag = 0;

	//printf("Wating Reponse from Agent\n");
	int res = recvfromTimeOut(iRcvUniSock, iTimeout_sec, 0);

	while(recvfromTimeOut(iRcvUniSock, iTimeout_sec, 0)!=0)
	{
		switch(res)
		{
		case 0:
			Sleep(1);
			//printf("=================== Agent ���� ���� Time Out ===================\n");
			break;
		case -1:
			//printf("RcvTimeOut Error\n");
			break;
		
		default:
			recvfrom(iRcvUniSock, cRcvBuf, sizeof(cRcvBuf), 0, NULL, 0);

			memcpy(&MyAgtInfoMsg, cRcvBuf, sizeof(MyAgtInfoMsg));

			//Agent�Ѵ�� XML���� �ϳ� ���� �� ������ �Է�
			mXMLManager.CreatXML_AgentInfo(MyAgtInfoMsg.cAgtIPAddr);
			mXMLManager.EditElementXML("AgentInfo", "AgentIP", MyAgtInfoMsg.cAgtIPAddr);
			mXMLManager.EditElementXML("AgentInfo", "AgentName", MyAgtInfoMsg.cAgtName);
			mXMLManager.EditElementXML("AgentInfo", "AgentLogDir", MyAgtInfoMsg.cAgtLogDir);
			mXMLManager.EditElementXML("AgentInfo", "AgentLogFileList", MyAgtInfoMsg.cAgtFileList);

			MyAgtInfoMsg.cAgtFileList[strlen(MyAgtInfoMsg.cAgtFileList)-1] = '\0';

			std::string sAgtFileList(MyAgtInfoMsg.cAgtFileList);
			std::stringstream strmAgtFileList(sAgtFileList);

			std::string sSingleFileName;
			std::string sIPwithFileName;

			strcat(MyAgtInfoMsg.cAgtIPAddr, "/");

			while(strmAgtFileList.good())
			{
				getline(strmAgtFileList, sSingleFileName,'\n');

				sIPwithFileName = MyAgtInfoMsg.cAgtIPAddr;
				sIPwithFileName += sSingleFileName;

				MyAgtInfoList.AgtFileList.push_back(sIPwithFileName);	
			}
			break;
		}
	}

	return MyAgtInfoList.AgtFileList;
}

void UDPCommunication::RcvRsc(int iRcvUniSock, int iTimeout_sec)
{
	char cRcvBuf[6000];
	//char* cSingleFileName;

	memset(cRcvBuf, 0, sizeof(cRcvBuf));

	AgtRcsMsgStruct MyAgtRcsMsg = AgtRcsMsgStruct();

	int iListStartFlag = 0;

	//printf("Wating Reponse from Agent\n");
	int res = recvfromTimeOut(iRcvUniSock, iTimeout_sec, 0);

	while(recvfromTimeOut(iRcvUniSock, iTimeout_sec, 0)!=0)
	{
		switch(res)
		{
		case 0:
			Sleep(1);
			break;
		case -1:
			//RcvTimeOut Error
			break;

		default:
			recvfrom(iRcvUniSock, cRcvBuf, sizeof(cRcvBuf), 0, NULL, 0);
			memcpy(&MyAgtRcsMsg, cRcvBuf, sizeof(MyAgtRcsMsg));

			break;
		}
	}
}

list<string> UDPCommunication::RcvInfor_nonTimeout(int iRcvUniSock, int iTimeout_sec)
{
	char cRcvBuf[4096];
	char cRcvdIP[16];

	memset(cRcvBuf, 0, sizeof(cRcvBuf));
	memset(cRcvdIP, 0, sizeof(cRcvdIP));

	AgtInfoMsgStruct MyAgtInfoMsg = AgtInfoMsgStruct();
	AgtInfoList MyAgtInfoList = AgtInfoList();

	int iListStartFlag = 0;

	switch(recvfromTimeOut(iRcvUniSock, iTimeout_sec, 0))
	{
		case 0:
			Sleep(1);
			break;
		case -1:
			Sleep(1);
			break;

		default:
			recvfrom(iRcvUniSock, cRcvBuf, sizeof(cRcvBuf), 0, NULL, 0);

			memcpy(&MyAgtInfoMsg, cRcvBuf, sizeof(MyAgtInfoMsg));

			//Agent�Ѵ�� XML���� �ϳ� ���� �� ������ �Է�
			mXMLManager.CreatXML_AgentInfo(MyAgtInfoMsg.cAgtIPAddr);
			mXMLManager.EditElementXML("AgentInfo", "AgentIP", MyAgtInfoMsg.cAgtIPAddr);
			mXMLManager.EditElementXML("AgentInfo", "AgentName", MyAgtInfoMsg.cAgtName);
			mXMLManager.EditElementXML("AgentInfo", "AgentLogFileList", MyAgtInfoMsg.cAgtFileList);

			MyAgtInfoMsg.cAgtFileList[strlen(MyAgtInfoMsg.cAgtFileList)-1] = '\0';

			std::string sAgtFileList(MyAgtInfoMsg.cAgtFileList);
			std::stringstream strmAgtFileList(sAgtFileList);

			std::string sSingleFileName;
			std::string sIPwithFileName;

			strcat(MyAgtInfoMsg.cAgtIPAddr, "/");

			while(strmAgtFileList.good())
			{
				getline(strmAgtFileList, sSingleFileName,'\n');

				sIPwithFileName = MyAgtInfoMsg.cAgtIPAddr;
				sIPwithFileName += sSingleFileName;

				MyAgtInfoList.AgtFileList.push_back(sIPwithFileName);	
			}
			break;
	}

	return MyAgtInfoList.AgtFileList;
}


//Ư�� Agent�� ������ ��û�ϱ� ���� �Լ� �Ǵ� ������Ʈ�� �������� �����ü�� ������ ��û�ϱ� ���� �Լ�

BOOL UDPCommunication::SndDataReq_MFC(int iSndSockUni, string sAgtIP, string sReqFileName, int iWtcPort)
{
	char cWtcName[64];
	string sWatcherIP;
	SOCKADDR_IN AgtAddr;

	struct DataReqMsgStruct MyDataReqMsg;

	memset(cWtcName, 0, sizeof(cWtcName));
	memset(&MyDataReqMsg, 0, sizeof(struct DataReqMsgStruct));
	memset(&AgtAddr, 0, sizeof(AgtAddr));
	
	int nError = gethostname(cWtcName, sizeof(cWtcName));
	if (nError == 0)
	{
		pHostInfo = gethostbyname(cWtcName);
		// ip address �ľ�
		sWatcherIP = inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]);
	}
	
	//port 1883;
	memcpy(&MyDataReqMsg.cWatcherIP, sWatcherIP.c_str(), strlen(sWatcherIP.c_str()));
	memcpy(&MyDataReqMsg.cReqFileName, sReqFileName.c_str(), strlen(sReqFileName.c_str()));
	MyDataReqMsg.iWatcherPort = iWtcPort;
	MyDataReqMsg.nReqType = 1;
	
	AgtAddr.sin_family = AF_INET;
	AgtAddr.sin_addr.s_addr = inet_addr(sAgtIP.c_str());
	AgtAddr.sin_port = htons(1883/*agtPort*/);
	
	if(sendto(iSndSockUni, (char*)&MyDataReqMsg, sizeof(struct DataReqMsgStruct), 0, (SOCKADDR*)&AgtAddr, sizeof(AgtAddr)))
		return TRUE;

	return FALSE;
}

char* UDPCommunication::SndDataReq(int iSndSockUni, list<string> lIPandFileList)
{
	int sel = 0;
	int iIndex = 0;
	char cIndex[100];
	char cWtcName[64];
	char cWtcIP[15];
	string sReturnFail = "F";

	SOCKADDR_IN AgtAddr;

	char cFileName[256];
	string sReqFileName = "";

	struct DataReqMsgStruct MyDataReqMsg;

	memset(cFileName, 0, sizeof(cFileName));
	memset(&MyDataReqMsg, 0, sizeof(struct DataReqMsgStruct));
	memset(&AgtAddr, 0, sizeof(AgtAddr));

	cout << " ��û�ϰ��� �ϴ� ������ �����ϼ���. " << endl;		

	list<string>::iterator iterStartList = lIPandFileList.begin();
	list<string>::iterator iterEndList = lIPandFileList.end();

	while(iterStartList != iterEndList)
	{		
		cout<< iIndex << ". " << *iterStartList <<endl;
		iIndex++;
		iterStartList++;
	}

	cout << " ��û�ϰ��� �ϴ� ���� ��ȣ: ";
	cin >> sel;

	if(lIPandFileList.size()<(sel+1))
		return (char*)sReturnFail.c_str();

	iterStartList = lIPandFileList.begin();

	for(int i = 0;i != sel;i++)
	{
		iterStartList ++;
	}

	sReqFileName = *iterStartList;
	std::stringstream strmReqFileInfo(sReqFileName);

	std::string sAgtIP;
	std::string sFileName;

	iIndex = 0;

	//����Ʈ���� IP�� �����̸��� �и��ϴ� �ܰ�
	while(strmReqFileInfo.good())
	{
		if(iIndex == 0)
		{
			getline(strmReqFileInfo, sAgtIP,'/');
			iIndex++;
		}
		else
		{
			getline(strmReqFileInfo, sFileName,'/');
		}
	}
	sSlctAgtIP = sAgtIP;

	strcpy(cFileName, sFileName.c_str());

	int nError = gethostname(cWtcName, sizeof(cWtcName));
	if (nError == 0)
	{
		pHostInfo = gethostbyname(cWtcName);
		// ip address �ľ�
		strcpy(cWtcIP, inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
	}

	//port 1883;
	MyDataReqMsg.iWatcherPort = 1883;
	MyDataReqMsg.nReqType = 1;
	memcpy(&MyDataReqMsg.cWatcherIP, cWtcIP, strlen(cWtcIP));
	memcpy(&MyDataReqMsg.cReqFileName, cFileName, strlen(cFileName));

	AgtAddr.sin_family = AF_INET;
	AgtAddr.sin_addr.s_addr = inet_addr(sAgtIP.c_str());
	AgtAddr.sin_port = htons(1883);

	if(sendto(iSndSockUni, (char*)&MyDataReqMsg, sizeof(struct DataReqMsgStruct), 0, (SOCKADDR*)&AgtAddr, sizeof(AgtAddr))==-1)
		printf("Agent Info Trans Failed\n");

	return cFileName;
}
