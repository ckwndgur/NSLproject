#include "Ucommunicatino_Agent.h"

#define SERVER "234.56.78.9"
#define PORT 1883
#define FILEDIRECTORY "c:\\Temp\\CoreDebug\\*"

Ucommunicatino_Agent::Ucommunicatino_Agent(void)
{
}

Ucommunicatino_Agent::~Ucommunicatino_Agent(void)
{
}

//��������� ���� �ʱ�ȭ �۾�
//�ּҼ���, WSA��� ����, ���� ���� ��..
void Ucommunicatino_Agent::InitSocket_Ag(int& hRecvSock, int& hRecvUniSock, int& hSndSock)
{
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		fputs("WSAStartup() error\n", stderr);

	//���ϻ���
	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hRecvSock == INVALID_SOCKET)
		fputs("Rsocket() error\n", stderr);
	else
		printf("1st Socket Open Success\n");

	hRecvUniSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hRecvUniSock == INVALID_SOCKET)
		fputs("hRecvUniSock() error\n", stderr);
	else
		printf("2nb Socket Open Success\n");

	hSndSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hSndSock == INVALID_SOCKET)
		fputs("Ssocket() error\n", stderr);
	else
		printf("3rd Socket Open Success\n");
	//hRecvSock, hSndSock, hRecvUniSock UDP������ ��������
}

void Ucommunicatino_Agent::InitSocket_Ag(int& hRecvSock, int& hSndSock)
{
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		fputs("WSAStartup() error\n", stderr);

	//���ϻ���
	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hRecvSock == INVALID_SOCKET)
		fputs("Rsocket() error\n", stderr);
	else
		printf("RSocket Open Success\n");

	hSndSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hSndSock == INVALID_SOCKET)
		fputs("Ssocket() error\n", stderr);
	else
		printf("SSocket Open Success\n");
}

void Ucommunicatino_Agent::MultiGroupRcvSet(int iRcvSocket, char* MultiGroupAddr, int MultiGroupPort)
{
	int iPort = MultiGroupPort;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(iPort);

	while(1)
	{
		if(bind(iRcvSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			printf("Try with another port...\n");
			iPort++;
			addr.sin_port = htons(iPort);
		}
		else
		{
			printf("Binded with Port Num : %d...\n", iPort);
			break;
		}
	}

	//ip_mreq����ü�� ������ �׷��� �ּ�����
	Rcv_Mreq.imr_multiaddr.s_addr = inet_addr(MultiGroupAddr);           //Ŭ���� D�� ��Ƽĳ��Ʈ �ּ�
	Rcv_Mreq.imr_interface.s_addr = htonl(INADDR_ANY); //�����ּ�

	//IP_ADD_MEMBERSHIP : ��Ƽĳ��Ʈ �׷쿡 ����
	state = setsockopt(iRcvSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&Rcv_Mreq, sizeof(Rcv_Mreq));
	if(state == SOCKET_ERROR)
		fputs("Rsetsockopt() error\n", stderr);
}

int Ucommunicatino_Agent::recvfromTimeOut(SOCKET socket, long sec, long usec)
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

//������ �޽����� ������Ʈ�� �������� ����� IP, �α����� ���, ���ϸ�� ���� �䱸�ϴ� �޽��� ����,
//�α����Ͽ��� �ش��ϴ� �����͸� �����ش޶�� �޽������� �Ǻ�(DiSCRiminator)�ϴ� �Լ�
int Ucommunicatino_Agent::RcvMsgDscr_Fix(int iRcvSock_multi, int iRcvSock_uni, char*& cWatcherIPOutput, int& iWatcherPortOutput)
{
	struct DataReqMsgStruct MyDataReqMsg;
	memset(cMultiRcvBuf, 0, sizeof(cMultiRcvBuf));

	int iMulti_res = recvfromTimeOut(iRcvSock_multi, 1, 0);
	while(recvfromTimeOut(iRcvSock_multi, 1, 0)!=0)
	{
		switch(iMulti_res)
		{
		case 0:
			Sleep(1);
			break;
		case -1:
			printf("RcvTimeOut Error");
			break;
		default:
			recvfrom(iRcvSock_multi, cMultiRcvBuf, sizeof(cMultiRcvBuf), 0, NULL, 0);
			memcpy(&MyDataReqMsg, cMultiRcvBuf, sizeof(MyDataReqMsg));
			//Agent ������ ��û�� ���
			printf("��ûŸ�� : %d\n", MyDataReqMsg.nReqType);
			printf("��ó IP  : ");
			printf(MyDataReqMsg.cWatcherIP);

			memcpy(cWatcherIPOutput, MyDataReqMsg.cWatcherIP, sizeof(MyDataReqMsg.cWatcherIP));

			printf("\n");
			printf("��óPort : %d\n", MyDataReqMsg.iWatcherPort);
			printf("=============================================================\n");
			printf("================ Wating Message From Watcher ================\n");
			printf("=============================================================\n");
			return 0;
			break;
		}
	}

	int iUni_res = recvfromTimeOut(iRcvSock_uni, 1, 0);
	memset(cUniRcvBuf, 0, sizeof(cUniRcvBuf));

	while(recvfromTimeOut(iRcvSock_uni, 1, 0)!=0)
	{
		switch(iUni_res)
		{
		case 0:
			break;
		case -1:
			printf("RcvTimeOut Error");
			break;
		default:
			recvfrom(iRcvSock_uni, cMultiRcvBuf, sizeof(cMultiRcvBuf), 0, NULL, 0);
			memcpy(&MyDataReqMsg, cMultiRcvBuf, sizeof(MyDataReqMsg));
			//Agent�� �α׵����͸� ��û�� ���
			printf("��ûŸ�� : %d\n", MyDataReqMsg.nReqType);
			printf("��ó IP  : ");
			printf(MyDataReqMsg.cWatcherIP);
			printf("\n");
			printf("��óPort : %d\n", MyDataReqMsg.iWatcherPort);
			printf("��û���� �����̸� : ");
			printf(MyDataReqMsg.cReqFileName);
			printf("\n");
			printf("=============================================================\n");
			printf("================ Wating Message From Watcher ================\n");
			printf("=============================================================\n");
			return 1;
			break;
		}
	}
	return -1;
}


void Ucommunicatino_Agent::SndToDst_Uni(int iSndSock, char* cDestIP, int iDestPort, char* cMessage)
{
	memset(&SndAddr, 0, sizeof(SndAddr));
	SndAddr.sin_family = AF_INET;
	SndAddr.sin_addr.s_addr = inet_addr(cDestIP); //���۹��� ��ó�� IP�� �Է�
	SndAddr.sin_port = htons(iDestPort);
	
	if(sendto(iSndSock, cMessage, strlen(cMessage), 0, (SOCKADDR*)&SndAddr, sizeof(SndAddr))==-1)
		printf("Agent Info Trans Failed\n");

	memset(cMessage, 0, sizeof(cMessage));
}

void Ucommunicatino_Agent::GetDate(int& iYear, int& iMonth, int& iDay, int& iHour, int& iMin, int& iSec)
{
	tTimer = time(NULL);

	localtime_s(&tTimer_St, &tTimer);

	iYear = (tTimer_St.tm_year + 1900);
	iMonth = (tTimer_St.tm_mon + 1);
	iDay = (tTimer_St.tm_mday);
	iHour = (tTimer_St.tm_hour);
	iMin = (tTimer_St.tm_min);
	iSec = (tTimer_St.tm_sec);	
}
