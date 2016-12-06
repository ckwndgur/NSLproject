#include "TCPCommunication.h"
#include "GetResourceUsage.h"
#include "Ucommunicatino_Agent.h"
#include "AgtRscMsg.h"

#include <process.h>

#define SERVER "234.56.78.9"
#define PORT 18840

using namespace std;

GetResourceUsage mGetResourceUsage = GetResourceUsage();
Ucommunicatino_Agent mUcommunicatino_Agent = Ucommunicatino_Agent();
GetDataFromAgent_WIN mGetDataFromAgent_WIN = GetDataFromAgent_WIN();
DataCollisionAvoidence mDataCollisionAvoidence = DataCollisionAvoidence();
TextManager mTextManager = TextManager();
XMLManager mXMLManager = XMLManager();
TCPCommunication mTCPCommunication = TCPCommunication();

int iUDP_RcvSock;
int iUDP_SndSock;
int iTCP_ServSock;
int iTCP_CltSock;

SOCKADDR_IN UDPaddr;
SOCKADDR_IN TCPaddr;

char cWatcherIP[15];
int iWatcherPort;
char cAgentIP[15];
char cAgentName[64];
int iszAgentName;
char cAgentFileList[2048];

char cRcvBuf[4096];
char* cTempBuf;

string sFileDir ="";
char* cFileDir;
HANDLE hFile;
int iTotalFileSize;
int iCurrentFilePoint;
int iLoadedFileLen;
DWORD dTemp;

float fCpuUsage;
string sDiskFreeSpace;
DWORD dwRAMUsage;

struct AgtInfoMsgStruct MyAgentInfoMsg;
struct DataReqMsgStruct MyDataReqMsg;
struct AgtDataMsgStruct MyAgtDataMsg;
struct AgtRcsMsgStruct MyAgtRcsMsg;
HANDLE hThread[2];

void GetSystemResource()
{
	int icnt = 0;
	while(1)
	{
		fCpuUsage = mGetResourceUsage.GetCPUUsage();
		if(icnt == 2)
		{
			break;
		}
		icnt++;
	}
	sDiskFreeSpace = mGetResourceUsage.GetDiskFreeSpace_All();
	dwRAMUsage = mGetResourceUsage.GetRAMUsage();
}

void UDP_Init()
{
	int iPort = PORT;
	mUcommunicatino_Agent.InitSocket_Ag(iUDP_RcvSock, iUDP_SndSock);
	mUcommunicatino_Agent.MultiGroupRcvSet(iUDP_RcvSock, SERVER, iPort);

	printf("==================== Socket Initialize Complete ===================\n");
	cout << "Agent log directory from XML file" <<endl;

	mXMLManager.initXML();
	sFileDir = mXMLManager.ParsingXML("CommonPath", "Agent");
	cFileDir = &sFileDir[0u];

	cout << cFileDir << endl;

	cout << "===================================================================" << endl;
}

void TCP_Init()
{
	mTCPCommunication.InitWinSock();
	mTCPCommunication.TCPSockInit(iTCP_ServSock);
	mTCPCommunication.TCPSockInit(iTCP_CltSock);

	int iSockBufSize = 1048576*2;

	setsockopt(iTCP_ServSock, SOL_SOCKET, SO_RCVBUF, (const char*) &iSockBufSize, sizeof(iSockBufSize));
	setsockopt(iTCP_CltSock, SOL_SOCKET, SO_RCVBUF, (const char*) &iSockBufSize, sizeof(iSockBufSize));
}

unsigned int __stdcall UDP_AgentInfoReq_Resp(void*)
{
	while(1)
	{
		memset(cWatcherIP, 0, sizeof(cWatcherIP));
		memset(&MyDataReqMsg, 0, sizeof(struct DataReqMsgStruct));
		memset(&MyAgentInfoMsg, 0, sizeof(struct AgtInfoMsgStruct));

		recvfrom(iUDP_RcvSock, cRcvBuf, sizeof(cRcvBuf), 0, NULL, 0);
		memcpy(&MyDataReqMsg, cRcvBuf, sizeof(struct DataReqMsgStruct));

		if(MyDataReqMsg.nReqType == 0)
		{
			iWatcherPort = MyDataReqMsg.iWatcherPort;
			strcat(cWatcherIP, MyDataReqMsg.cWatcherIP);
			printf("===================================================================\n");
			printf("Watcher의 정보요청 메시지 도착\n");
			printf("Watcher IP : ");
			printf(cWatcherIP);
			printf("\n");
			printf("===================================================================\n");

			cTempBuf = mGetDataFromAgent_WIN.GetAgentName(iszAgentName);
			memcpy(cAgentName, cTempBuf, strlen(cTempBuf));

			cTempBuf = mGetDataFromAgent_WIN.GetAgentIP();
			memcpy(cAgentIP, cTempBuf, strlen(cTempBuf));

			sFileDir = mXMLManager.ParsingXML("CommonPath", "Agent");
			cFileDir = &sFileDir[0u];
			
			memcpy(&MyAgentInfoMsg.cAgtLogDir, &cFileDir, sizeof(MyAgentInfoMsg.cAgtLogDir));
			
			strcat(cFileDir, "*");
			cTempBuf = mGetDataFromAgent_WIN.GetAgentFileInfo(cFileDir);
			if(strlen(cTempBuf)<2)
			{
				cout << " 경로설정이 잘못되었거나 파일이 존재하지 않습니다. " << endl;
				memcpy(cAgentFileList, "No File In Dir\n", strlen("No File In Dir\n"));
			}
			else
			{
				memcpy(cAgentFileList, cTempBuf, strlen(cTempBuf));
			}

			MyAgentInfoMsg.nReqType = 0;
			memcpy(&MyAgentInfoMsg.cAgtIPAddr, cAgentIP, sizeof(MyAgentInfoMsg.cAgtIPAddr));
			memcpy(&MyAgentInfoMsg.cAgtName, cAgentName, sizeof(MyAgentInfoMsg.cAgtName));
			memcpy(&MyAgentInfoMsg.cAgtFileList, cAgentFileList, sizeof(MyAgentInfoMsg.cAgtFileList));

			int iDelay = mDataCollisionAvoidence.CollisionAvoidence(5000);

			printf("===============================================================\n");
			printf("전송 전 임의의 지연 : %d [ms]\n", iDelay);
			printf("===============================================================\n");

			memset(&UDPaddr, 0, sizeof(UDPaddr));
			UDPaddr.sin_family = AF_INET;
			UDPaddr.sin_addr.s_addr = inet_addr(cWatcherIP); //전송받은 와처의 IP를 입력
			UDPaddr.sin_port = htons(iWatcherPort);

			if(sendto(iUDP_SndSock, (char*)&MyAgentInfoMsg, sizeof(struct AgtInfoMsgStruct), 0, (SOCKADDR*)&UDPaddr, sizeof(UDPaddr))==-1)
				printf("Agent Info Trans Failed\n");
		}
		else
		{}
	}
	return 0;
}

unsigned int __stdcall TCP_AgentLogReq_Resp(void*)
{
	char cRcvBuf[16000];
	char cFileDir[256];
	int iRcvdLen = -1;

	if(listen(iTCP_ServSock, 5) == SOCKET_ERROR)
		printf("TCP Listen Error\n");

	int iszTCPaddr = sizeof(TCPaddr);

	iTCP_CltSock = accept(iTCP_ServSock, (SOCKADDR*)&TCPaddr, &iszTCPaddr);
	
	if(iTCP_CltSock == INVALID_SOCKET)
		printf("TCP Accept Error\n");
	
	memset(cWatcherIP, 0, sizeof(cWatcherIP));
	memset(cFileDir, 0, sizeof(cFileDir));
	memset(&MyDataReqMsg, 0, sizeof(struct DataReqMsgStruct));
	
	recv(iTCP_CltSock, cRcvBuf, sizeof(cRcvBuf),0);
	
	memcpy(&MyDataReqMsg, cRcvBuf, sizeof(struct DataReqMsgStruct));
	
	switch(MyDataReqMsg.nReqType)
	{
		case 2:
			strcat(cWatcherIP, MyDataReqMsg.cWatcherIP);
			printf("===================================================================\n");
			printf("Watcher의 로그요청 메시지 도착\n");
			printf("Watcher IP : ");
			printf(cWatcherIP);
			printf("\n");
			printf("요청된 파일 : ");
			printf(MyDataReqMsg.cReqFileName);
			printf("\n");
			printf("===================================================================\n");

			sFileDir = mXMLManager.ParsingXML("CommonPath", "Agent");
			sFileDir += MyDataReqMsg.cReqFileName;
			memcpy(&cFileDir, sFileDir.c_str(), sFileDir.length()-1);
			//sFileDir.substr(0, sFileDir.length()-1);

			hFile = CreateFile(cFileDir, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

			if(hFile == INVALID_HANDLE_VALUE)
			{
				printf("잘못된 파일경로\n");
			}
			else
			{
				MyAgtDataMsg.lTotalDataSize = GetFileSize(hFile, &dTemp);
				iTotalFileSize = MyAgtDataMsg.lTotalDataSize;
				//iTotalFileSize = mTextManager.GetFileSize("d:\\log\\", MyDataReqMsg.cReqFileName);
				//dTemp는 임의로 넣어준 값 함수가 dTemp에도 값을 리턴해 주지만 iFileSize로 값을 받아서 씀
			}

			SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
			MyAgtDataMsg.bLastPacket = FALSE;
			iCurrentFilePoint = 0;

			while(1)
			{
				SetFilePointer(hFile, 0, NULL, iCurrentFilePoint);

				memset(&MyAgtDataMsg.cLogData, 0, 15000/*bufsize*/);

				//cAgtRtrnValBuf = mGetDataFromAgent_WIN.GetLogData(cReqdLogData, iCurrentFilePoint);
				ReadFile(hFile, MyAgtDataMsg.cLogData, 15000, &dTemp, 0);

				iCurrentFilePoint += dTemp;

				if(iTotalFileSize == iCurrentFilePoint)
				{
					MyAgtDataMsg.bLastPacket = TRUE;
				}

				if(send(iTCP_CltSock, (char*)&MyAgtDataMsg, sizeof(struct AgtDataMsgStruct), 0)==-1)
					printf("Agent Log Trans Failed\n");

				Sleep(1);

				if(MyAgtDataMsg.bLastPacket == TRUE)
				{
					printf("File Transmission Complete..\n");
					break;
				}	
			}
		break;

		case 3:
			iWatcherPort = MyDataReqMsg.iWatcherPort;
			strcat(cWatcherIP, MyDataReqMsg.cWatcherIP);
			printf("===================================================================\n");
			printf("Watcher의 시스템 리소스 정보 요청 메시지 도착\n");
			printf("Watcher IP : ");
			printf(cWatcherIP);
			printf("\n");
			printf("===================================================================\n");
			
			GetSystemResource();

			MyAgtRcsMsg.fCPUUsage = fCpuUsage;
			MyAgtRcsMsg.dwRAMUsage = dwRAMUsage;
			memcpy(&MyAgtRcsMsg.cHDDUsage, sDiskFreeSpace.c_str(), strlen(sDiskFreeSpace.c_str()));

			if(send(iTCP_CltSock, (char*)&MyAgtRcsMsg, sizeof(struct AgtRcsMsgStruct), 0)==-1)
				printf("Agent Rsc Trans Failed\n");

			Sleep(1);
		break;

	}
	return 0;
}


int main(void)
{
	TCP_Init();
	UDP_Init();
	
	int iPort = 18830;
	mTCPCommunication.TCPAddrAlloc_Auto(iTCP_ServSock, iPort);

	DWORD retval = WAIT_OBJECT_0 + 10;

	while(1)
	{
		switch (retval)
		{
			case WAIT_OBJECT_0: // hTread[0] 종료
				printf("__Thread Closed\n");
				CloseHandle(hThread[0]);
				printf("__Run Agent Info Req Rcv Thread\n");
				hThread[0] = (HANDLE)_beginthreadex(NULL, 0, UDP_AgentInfoReq_Resp, NULL, NULL, NULL);
				break;

			case WAIT_OBJECT_0+1: // hTread[1] 종료
				CloseHandle(hThread[1]);
				printf("__Run Agent Log Req Rcv Thread\n");
				hThread[1] = (HANDLE)_beginthreadex(NULL, 0, TCP_AgentLogReq_Resp, NULL, NULL, NULL);
				break;
			
			case WAIT_OBJECT_0+10: // hTread[1] 종료
				hThread[0] = (HANDLE)_beginthreadex(NULL, 0, UDP_AgentInfoReq_Resp, NULL, NULL, NULL);
				hThread[1] = (HANDLE)_beginthreadex(NULL, 0, TCP_AgentLogReq_Resp, NULL, NULL, NULL);
				printf("__Thread Initialized\n");
				break;

			case WAIT_FAILED:    // 오류 발생
				printf("Error Thread\n");
				break;
		}
		retval = 0;
		retval = WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
		printf("__Thread End\n");
	}
}
