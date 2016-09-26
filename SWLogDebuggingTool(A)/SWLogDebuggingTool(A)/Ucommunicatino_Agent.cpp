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

//소켓통신을 위한 초기화 작업
//주소설정, WSA사용 설정, 소켓 생성 등..
void Ucommunicatino_Agent::InitSocket_Ag(){
	
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		fputs("WSAStartup() error\n", stderr);

	//소켓생성
	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hRecvSock == INVALID_SOCKET)
		fputs("Rsocket() error\n", stderr);
	
	hSndSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hSndSock == INVALID_SOCKET)
		fputs("Ssocket() error\n", stderr);

	multiTTL=10;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);

	if(bind(hRecvSock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		fputs("bind() error\n", stderr);

	//ip_mreq구조체에 가입할 그룹의 주소지정
	Rcv_Mreq.imr_multiaddr.s_addr = inet_addr(SERVER);           //클레스 D의 멀티캐스트 주소
	Rcv_Mreq.imr_interface.s_addr = htonl(INADDR_ANY); //로컬주소

	//IP_ADD_MEMBERSHIP : 멀티캐스트 그룹에 가입
	state = setsockopt(hRecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&Rcv_Mreq, sizeof(Rcv_Mreq));
	if(state == SOCKET_ERROR)
		fputs("Rsetsockopt() error\n", stderr);
}

//수신한 메시지가 에이전트가 실행중인 노드의 IP, 로그파일 경로, 파일목록 등을 요구하는 메시지 인지,
//로그파일에서 해당하는 데이터를 전송해달라는 메시지인지 판별(DiSCRiminator)하는 함수
int Ucommunicatino_Agent::RcvMsgDscr()
{
	memset(&cFileBuf, 0, sizeof(cFileBuf));

	printf("Watcher의 요청을 대기합니다...\n");
	recvfrom(hRecvSock, cFileBuf, BUFSIZE, 0, NULL, 0);
	sHeader = "";

	for(int i=0;i<6;i++)
	{
		sHeader+=cFileBuf[i];
	}

	//와처가 현재 에이전트가 실행중인 노드에 노드정보를 와처로 전송해달라는 메시지의 경우 메시지 앞에 WATIP/를 붙여 송신 (WATcher IP address)
	if(sHeader=="WATIP/")
	{
		memset(&cWatcherIP, 0, sizeof(cWatcherIP));

		iTempNum1 = 0;
		iTempNum2 =6;
		
		//실제 메시지를 수신된 데이터로부터 분리하는 부분
		//앞 6개의 문자는 WATIP/ 또는 LFREQ/ 와 같은 메시지의 종류를 나타내므로 7번째 문자부터 메시지로 인식하여 다른 버프에 저장
		while(1)
		{
			cWatcherIP[iTempNum1] = cFileBuf[iTempNum2];
			
			iTempNum1++;
			iTempNum2++;
			
			if(cFileBuf[iTempNum2] == NULL)
				break;
		}
		return 0;
	}
	//와처가 에이전트에게 로그 데이터를 요청할 때는 메시지 앞에 LFREQ/를 붙여 송신 (Log File REQuest)
	else if(sHeader=="LFREQ/")
	{
		int nError = gethostname(cHostName, sizeof(cHostName));
	
		if (nError == 0)
		{
			pHostInfo = gethostbyname(cHostName);
			// ip address 파악
			strcpy(cIPAddr, inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
		}

		sFilePath ="";
		iTempNum1 =6;

		//실제 메시지를 수신된 데이터로부터 분리하는 부분
		//앞 6개의 문자는 WATIP/ 또는 LFREQ/ 와 같은 메시지의 종류를 나타내므로 7번째 문자부터 메시지로 인식하여 다른 버프에 저장
		while(1)
		{			
			if(cFileBuf[iTempNum1] == char(10))
			{
				iTempNum1++;
				iTempNum1++;
				break;
			}
			sRcvIP += cFileBuf[iTempNum1];
			
			iTempNum1++;
		}

		while(1)
		{
			if(cFileBuf[iTempNum1] == char(10))
				break;
			
			sFilePath += cFileBuf[iTempNum1];

			iTempNum1++;
		}

		iTempNum1 = 0;

		while(1)
		{		
			if(cIPAddr[iTempNum1] == NULL)
				break;

			sMyIP += cIPAddr[iTempNum1];
			iTempNum1++;
		}

		if(sMyIP == sRcvIP)
		{
			printf("Message for me\n");
			sMyIP = "";
			sRcvIP = "";
			return 1;
		}
		else
		{
			printf("Message for others\n");
			return -1;
		}

	}
	else
	{
	//어떠한 메시지도 아닌 경우는 -1을 리턴하여 오류 처리
		return -1;
	}
}


void Ucommunicatino_Agent::SendAgentInfor()
{
	struct sInfoMsgStruct MyInforMsg;
	
	memset(&SndAddr, 0, sizeof(SndAddr));
	memset(&MyInforMsg, 0, sizeof(MyInforMsg));

	SndAddr.sin_family = AF_INET;
	SndAddr.sin_addr.s_addr = inet_addr(cWatcherIP); //전송받은 와처의 IP를 입력
	SndAddr.sin_port = htons(PORT);
	
	int nError = gethostname(cHostName, sizeof(cHostName));

	//구조체를 사용하는 전송//////////////////
	if (nError == 0)
	{
		pHostInfo = gethostbyname(cHostName);
		// ip address 파악
		// ip address를 메세지 구조체에 입력
		MyInforMsg.cAgtIPAddr = (char*)(inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
		//strcpy(MyInforMsg.cAgtIPAddr, inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
	}

	//에이전트의 이름추가
	MyInforMsg.cAgtIPAddr = cHostName;
	//strcpy(MyInforMsg.cAgtName, cHostName);

	//에이전트에 저장된 로그파일 경로를 FILEDIRECTORY에 입력하여 Char형으로 전환
	strcpy(cPath, FILEDIRECTORY);
	do
	{
		//파일 경로를 추출
		hFind = FindFirstFile(cPath, &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
			fputs("FindFirstFile() error\n", stderr);
	}while(hFind==INVALID_HANDLE_VALUE);

	//파일 경로를 메시지에 추가
	(MyInforMsg.cAgtLogFilePath = cPath;
	//strcat(MyInforMsg.cAgtLogFilePath, cPath);

	//경로내부의 모든 파일이름 나열하여 출력한 다음 메시지에 추가함
	do{
		printf("%s\n", FindData.cFileName);
		strcat(MyInforMsg.cAgtFileList, (const char*)FindData.cFileName);
		strcat(MyInforMsg.cAgtFileList, "\n");
	}while(FindNextFile(hFind, &FindData));

	FindClose(hFind);

	//구조체를 사용하지 않는 전송//////////////
	/*if (nError == 0)
	{
		pHostInfo = gethostbyname(cHostName);
		// ip address 파악
		strcpy(cIPAddr, inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
	}

	
	//메시지는 cSndBuf에 차례대로 쌓아가는 방식으로 생성
	//먼저 에이전트의 IP를 전송하기 위해 메시지 처음에 AGTIP/로 에이전트의 IP임을 표시함
	strcpy(cSndBuf, "AGTIP/");  //AGTIP : AGenT IP address

	//에이전트의 실제 IP주소를 추가
	strcat(cSndBuf, cIPAddr);
	
	//에이전트의 이름추가
	strcat(cSndBuf, "/AGTNA/");  //AGTN : AGenT NAme (PC이름)
	strcat(cSndBuf, cHostName);

	//에이전트에 저장된 로그파일 경로를 FILEDIRECTORY에 입력하여 Char형으로 전환
	strcpy(cPath, FILEDIRECTORY);

	do
	{
		//파일 경로를 추출
		hFind = FindFirstFile(cPath, &FindData);
		

		if(hFind==INVALID_HANDLE_VALUE)
			fputs("FindFirstFile() error\n", stderr);
	}while(hFind==INVALID_HANDLE_VALUE);

	//파일 경로를 메시지에 추가
	strcat(cSndBuf, "/FPATH/");
	strcat(cSndBuf, cPath);
	strcat(cSndBuf,"/FLIST/");
	
	//경로내부의 모든 파일이름 나열하여 출력한 다음 메시지에 추가함
	do{
		printf("%s\n", FindData.cFileName);
		strcat(cSndBuf, (const char*)FindData.cFileName);
		strcat(cSndBuf, "\n");
	}while(FindNextFile(hFind, &FindData));

	FindClose(hFind);

	//만들어진 메시지를 와처로 발송
	if(sendto(hSndSock, cSndBuf, strlen(cSndBuf), 0, (SOCKADDR*)&SndAddr, sizeof(SndAddr))==-1)
		printf("Agent정보 송신 실패\n");
	memset(cSndBuf, '\0', sizeof(cSndBuf));
	memset(&cFileBuf, 0, sizeof(cFileBuf));
	*/
	////////////////////////////////////////////////////////////////////////////
	
	//만들어진 메시지를 와처로 발송 -- 구조체
	if(sendto(hSndSock, (char*)&MyInforMsg, sizeof(MyInforMsg), 0, (SOCKADDR*)&SndAddr, sizeof(SndAddr))==-1)
		printf("Agent정보 송신 실패\n");

	//사용한 구조체 다시 초기화
	memset(&MyInforMsg, 0, sizeof(MyInforMsg));
}

void Ucommunicatino_Agent::SendLogData()
{
	iTotal=0;
	iCurrent=0;
	iRead=0;
	iFSize=0;
	
	memset(cFileBuf, 0, sizeof(cFileBuf));

	//sFilePath은 RcvMsgDscr함수에서 값을 입력받음
	//CreatFile을 통해서 sFilePath과 일치하는 파일경로를 생성 및 연 다음 파일 생성 및 열기
	hFile = CreateFile(sFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(hFile == INVALID_HANDLE_VALUE){
		printf("파일 경로가 아님\n");
	}
	else{
		iFSize = GetFileSize(hFile, &dTemp);//dTemp는 임의로 넣어준 값 함수가 dTemp에도 값을 리턴해 주지만 iFileSize로 값을 받아서 씀
		
		//File의 사이즈를 먼저 알려주기 위한 메시지를 생성
		strcat(cFileBuf, "FSIZE/");
		itoa(iFSize, cPath, 10);
		strcat(cFileBuf, cPath);
		
		//File사이즈에 대한 정보를 담은 메시지 발송
		sendto(hSndSock, cFileBuf, strlen(cFileBuf), 0, (SOCKADDR*)&SndAddr, sizeof(SndAddr));
	}

	iTotal=iFSize;
	iCurrent=0;
	iRead=0;
	iLen = 0;

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

	while(iTotal>iCurrent)
	{
		Sleep(1);
		memset(cFileBuf, '\0', sizeof(cFileBuf));

		SetFilePointer(hFile, 0, NULL, iCurrent);

		iRead = ReadFile(hFile, cFileBuf, BUFSIZE, &iLen, 0);
		if(sendto(hSndSock, cFileBuf, strlen(cFileBuf), 0 ,(SOCKADDR*)&SndAddr, sizeof(SndAddr))==-1)
		{
			printf("전송 실패\n");
		}
		
		iCurrent += strlen(cFileBuf);
		printf("전송성공\n");
	}
	strcpy(cFileBuf, "/MSGFIN");

	sendto(hSndSock, cFileBuf, strlen(cFileBuf), 0 ,(SOCKADDR*)&SndAddr, sizeof(SndAddr));
	printf("전체 파일 전송 완료\n");

	memset(&cIPAddr, 0, sizeof(cIPAddr));
	memset(&cFileBuf, 0, sizeof(cFileBuf));
}
