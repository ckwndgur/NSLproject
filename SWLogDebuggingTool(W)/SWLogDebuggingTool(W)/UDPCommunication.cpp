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

void UDPCommunication::InitSocket_Wt(int& hRecvSock, int& hRecvUniSock, int& hSndSock)
{
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		fputs("WSAStartup() error\n", stderr);

	//���ϻ���
	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hRecvSock == INVALID_SOCKET)
		fputs("Rsocket() error\n", stderr);
	// 	else
	// 		printf("1st Socket Open Success\n");

	hRecvUniSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hRecvUniSock == INVALID_SOCKET)
		fputs("hRecvUniSock() error\n", stderr);
	// 	else
	// 		printf("2nb Socket Open Success\n");

	hSndSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hSndSock == INVALID_SOCKET)
		fputs("Ssocket() error\n", stderr);
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

	if(bind(iRcvSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		fputs("bind() error\n", stderr);

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

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(cMultiGroup);
	addr.sin_port = htons(iWatcherPort);

	memcpy(&MyDataReqMsg.cWatcherIP, cIPAddr, strlen(cIPAddr));
	MyDataReqMsg.iWatcherPort = iWatcherPort;

	sendto(iSndSock, (char*)&MyDataReqMsg, sizeof(struct DataReqMsgStruct), 0, (SOCKADDR*)&addr, sizeof(addr));
	printf("InfoReq Message Send Complete\n");
}

void UDPCommunication::LogFileRcv(int iRcvSocket, char* cFileDir, char* cFileName)
{
	struct AgtDataMsgStruct MyAgtDataMsg;
	DWORD dLen;

	memset(&MyAgtDataMsg, 0, sizeof(struct AgtDataMsgStruct));

	string sWriteData = "";

	mFolderManager.MakeDirectory(cFileDir, cFileName);

	while(1)
	{
		recvfrom(iRcvSocket, (char*)&MyAgtDataMsg, sizeof(struct AgtDataMsgStruct), 0, NULL, 0);

		sWriteData = "";
		sWriteData = MyAgtDataMsg.cLogData;

		//WriteFile(hFile, sWriteData.c_str(), strlen(sWriteData.c_str()), &dLen, 0);
		mTextManager.WriteText(cFileDir, cFileName, sWriteData);

		if(MyAgtDataMsg.bLastPacket == TRUE)
		{
			printf("File Download Complete.\n");
			break;
		}
	}
}

/*
int UCommunication::MsgHeaderDecision(int iRcvSocket)
{

}
*/

//���� ������Ʈ�� �������� ����� IP/�α����ϰ��/��ο� �����ϴ� ���� ��� �� ��û�ϱ� ���� �Լ�
list<string> UDPCommunication::RcvInfor(int iRcvUniSock, int iTimeout_sec)
{
	char cRcvBuf[4096];
	char cRcvdIP[16];
	//char* cSingleFileName;

	memset(cRcvBuf, 0, sizeof(cRcvBuf));
	memset(cRcvdIP, 0, sizeof(cRcvdIP));

	AgtInfoMsgStruct MyAgtInfoMsg = AgtInfoMsgStruct();
	AgtInfoList MyAgtInfoList = AgtInfoList();

	int iListStartFlag = 0;

	printf("Wating Reponse from Agent\n");
	int res = recvfromTimeOut(iRcvUniSock, iTimeout_sec, 0);

	while(recvfromTimeOut(iRcvUniSock, iTimeout_sec, 0)!=0)
	{
		switch(res)
		{
		case 0:
			Sleep(1);
			printf("=================== Agent ���� ���� Time Out ===================\n");
			break;
		case -1:
			printf("RcvTimeOut Error\n");
			break;
		default:
			recvfrom(iRcvUniSock, cRcvBuf, sizeof(cRcvBuf), 0, NULL, 0);

			memcpy(&MyAgtInfoMsg, cRcvBuf, sizeof(MyAgtInfoMsg));
			cout << "//////////////////////////////////////////////////" <<endl;
			printf("Rcvd Agt IP : ");
			printf(MyAgtInfoMsg.cAgtIPAddr);
			printf("\n");
			printf("Rcvd Agt Name : ");
			printf(MyAgtInfoMsg.cAgtName);
			printf("\n");
			printf("Rcvd Agt File List : ");
			printf("\n");
			printf(MyAgtInfoMsg.cAgtFileList);

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

			cout << "//////////////////////////////////////////////////" <<endl;

			break;
		}
	}

	return MyAgtInfoList.AgtFileList;
}


//Ư�� Agent�� ������ ��û�ϱ� ���� �Լ� �Ǵ� ������Ʈ�� �������� �����ü�� ������ ��û�ϱ� ���� �Լ�

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