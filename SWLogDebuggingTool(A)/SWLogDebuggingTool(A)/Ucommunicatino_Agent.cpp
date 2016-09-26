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
void Ucommunicatino_Agent::InitSocket_Ag(){
	
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		fputs("WSAStartup() error\n", stderr);

	//���ϻ���
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

	//ip_mreq����ü�� ������ �׷��� �ּ�����
	Rcv_Mreq.imr_multiaddr.s_addr = inet_addr(SERVER);           //Ŭ���� D�� ��Ƽĳ��Ʈ �ּ�
	Rcv_Mreq.imr_interface.s_addr = htonl(INADDR_ANY); //�����ּ�

	//IP_ADD_MEMBERSHIP : ��Ƽĳ��Ʈ �׷쿡 ����
	state = setsockopt(hRecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&Rcv_Mreq, sizeof(Rcv_Mreq));
	if(state == SOCKET_ERROR)
		fputs("Rsetsockopt() error\n", stderr);
}

//������ �޽����� ������Ʈ�� �������� ����� IP, �α����� ���, ���ϸ�� ���� �䱸�ϴ� �޽��� ����,
//�α����Ͽ��� �ش��ϴ� �����͸� �����ش޶�� �޽������� �Ǻ�(DiSCRiminator)�ϴ� �Լ�
int Ucommunicatino_Agent::RcvMsgDscr()
{
	memset(&cFileBuf, 0, sizeof(cFileBuf));

	printf("Watcher�� ��û�� ����մϴ�...\n");
	recvfrom(hRecvSock, cFileBuf, BUFSIZE, 0, NULL, 0);
	sHeader = "";

	for(int i=0;i<6;i++)
	{
		sHeader+=cFileBuf[i];
	}

	//��ó�� ���� ������Ʈ�� �������� ��忡 ��������� ��ó�� �����ش޶�� �޽����� ��� �޽��� �տ� WATIP/�� �ٿ� �۽� (WATcher IP address)
	if(sHeader=="WATIP/")
	{
		memset(&cWatcherIP, 0, sizeof(cWatcherIP));

		iTempNum1 = 0;
		iTempNum2 =6;
		
		//���� �޽����� ���ŵ� �����ͷκ��� �и��ϴ� �κ�
		//�� 6���� ���ڴ� WATIP/ �Ǵ� LFREQ/ �� ���� �޽����� ������ ��Ÿ���Ƿ� 7��° ���ں��� �޽����� �ν��Ͽ� �ٸ� ������ ����
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
	//��ó�� ������Ʈ���� �α� �����͸� ��û�� ���� �޽��� �տ� LFREQ/�� �ٿ� �۽� (Log File REQuest)
	else if(sHeader=="LFREQ/")
	{
		int nError = gethostname(cHostName, sizeof(cHostName));
	
		if (nError == 0)
		{
			pHostInfo = gethostbyname(cHostName);
			// ip address �ľ�
			strcpy(cIPAddr, inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
		}

		sFilePath ="";
		iTempNum1 =6;

		//���� �޽����� ���ŵ� �����ͷκ��� �и��ϴ� �κ�
		//�� 6���� ���ڴ� WATIP/ �Ǵ� LFREQ/ �� ���� �޽����� ������ ��Ÿ���Ƿ� 7��° ���ں��� �޽����� �ν��Ͽ� �ٸ� ������ ����
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
	//��� �޽����� �ƴ� ���� -1�� �����Ͽ� ���� ó��
		return -1;
	}
}


void Ucommunicatino_Agent::SendAgentInfor()
{
	struct sInfoMsgStruct MyInforMsg;
	
	memset(&SndAddr, 0, sizeof(SndAddr));
	memset(&MyInforMsg, 0, sizeof(MyInforMsg));

	SndAddr.sin_family = AF_INET;
	SndAddr.sin_addr.s_addr = inet_addr(cWatcherIP); //���۹��� ��ó�� IP�� �Է�
	SndAddr.sin_port = htons(PORT);
	
	int nError = gethostname(cHostName, sizeof(cHostName));

	//����ü�� ����ϴ� ����//////////////////
	if (nError == 0)
	{
		pHostInfo = gethostbyname(cHostName);
		// ip address �ľ�
		// ip address�� �޼��� ����ü�� �Է�
		MyInforMsg.cAgtIPAddr = (char*)(inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
		//strcpy(MyInforMsg.cAgtIPAddr, inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
	}

	//������Ʈ�� �̸��߰�
	MyInforMsg.cAgtIPAddr = cHostName;
	//strcpy(MyInforMsg.cAgtName, cHostName);

	//������Ʈ�� ����� �α����� ��θ� FILEDIRECTORY�� �Է��Ͽ� Char������ ��ȯ
	strcpy(cPath, FILEDIRECTORY);
	do
	{
		//���� ��θ� ����
		hFind = FindFirstFile(cPath, &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
			fputs("FindFirstFile() error\n", stderr);
	}while(hFind==INVALID_HANDLE_VALUE);

	//���� ��θ� �޽����� �߰�
	(MyInforMsg.cAgtLogFilePath = cPath;
	//strcat(MyInforMsg.cAgtLogFilePath, cPath);

	//��γ����� ��� �����̸� �����Ͽ� ����� ���� �޽����� �߰���
	do{
		printf("%s\n", FindData.cFileName);
		strcat(MyInforMsg.cAgtFileList, (const char*)FindData.cFileName);
		strcat(MyInforMsg.cAgtFileList, "\n");
	}while(FindNextFile(hFind, &FindData));

	FindClose(hFind);

	//����ü�� ������� �ʴ� ����//////////////
	/*if (nError == 0)
	{
		pHostInfo = gethostbyname(cHostName);
		// ip address �ľ�
		strcpy(cIPAddr, inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
	}

	
	//�޽����� cSndBuf�� ���ʴ�� �׾ư��� ������� ����
	//���� ������Ʈ�� IP�� �����ϱ� ���� �޽��� ó���� AGTIP/�� ������Ʈ�� IP���� ǥ����
	strcpy(cSndBuf, "AGTIP/");  //AGTIP : AGenT IP address

	//������Ʈ�� ���� IP�ּҸ� �߰�
	strcat(cSndBuf, cIPAddr);
	
	//������Ʈ�� �̸��߰�
	strcat(cSndBuf, "/AGTNA/");  //AGTN : AGenT NAme (PC�̸�)
	strcat(cSndBuf, cHostName);

	//������Ʈ�� ����� �α����� ��θ� FILEDIRECTORY�� �Է��Ͽ� Char������ ��ȯ
	strcpy(cPath, FILEDIRECTORY);

	do
	{
		//���� ��θ� ����
		hFind = FindFirstFile(cPath, &FindData);
		

		if(hFind==INVALID_HANDLE_VALUE)
			fputs("FindFirstFile() error\n", stderr);
	}while(hFind==INVALID_HANDLE_VALUE);

	//���� ��θ� �޽����� �߰�
	strcat(cSndBuf, "/FPATH/");
	strcat(cSndBuf, cPath);
	strcat(cSndBuf,"/FLIST/");
	
	//��γ����� ��� �����̸� �����Ͽ� ����� ���� �޽����� �߰���
	do{
		printf("%s\n", FindData.cFileName);
		strcat(cSndBuf, (const char*)FindData.cFileName);
		strcat(cSndBuf, "\n");
	}while(FindNextFile(hFind, &FindData));

	FindClose(hFind);

	//������� �޽����� ��ó�� �߼�
	if(sendto(hSndSock, cSndBuf, strlen(cSndBuf), 0, (SOCKADDR*)&SndAddr, sizeof(SndAddr))==-1)
		printf("Agent���� �۽� ����\n");
	memset(cSndBuf, '\0', sizeof(cSndBuf));
	memset(&cFileBuf, 0, sizeof(cFileBuf));
	*/
	////////////////////////////////////////////////////////////////////////////
	
	//������� �޽����� ��ó�� �߼� -- ����ü
	if(sendto(hSndSock, (char*)&MyInforMsg, sizeof(MyInforMsg), 0, (SOCKADDR*)&SndAddr, sizeof(SndAddr))==-1)
		printf("Agent���� �۽� ����\n");

	//����� ����ü �ٽ� �ʱ�ȭ
	memset(&MyInforMsg, 0, sizeof(MyInforMsg));
}

void Ucommunicatino_Agent::SendLogData()
{
	iTotal=0;
	iCurrent=0;
	iRead=0;
	iFSize=0;
	
	memset(cFileBuf, 0, sizeof(cFileBuf));

	//sFilePath�� RcvMsgDscr�Լ����� ���� �Է¹���
	//CreatFile�� ���ؼ� sFilePath�� ��ġ�ϴ� ���ϰ�θ� ���� �� �� ���� ���� ���� �� ����
	hFile = CreateFile(sFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(hFile == INVALID_HANDLE_VALUE){
		printf("���� ��ΰ� �ƴ�\n");
	}
	else{
		iFSize = GetFileSize(hFile, &dTemp);//dTemp�� ���Ƿ� �־��� �� �Լ��� dTemp���� ���� ������ ������ iFileSize�� ���� �޾Ƽ� ��
		
		//File�� ����� ���� �˷��ֱ� ���� �޽����� ����
		strcat(cFileBuf, "FSIZE/");
		itoa(iFSize, cPath, 10);
		strcat(cFileBuf, cPath);
		
		//File����� ���� ������ ���� �޽��� �߼�
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
			printf("���� ����\n");
		}
		
		iCurrent += strlen(cFileBuf);
		printf("���ۼ���\n");
	}
	strcpy(cFileBuf, "/MSGFIN");

	sendto(hSndSock, cFileBuf, strlen(cFileBuf), 0 ,(SOCKADDR*)&SndAddr, sizeof(SndAddr));
	printf("��ü ���� ���� �Ϸ�\n");

	memset(&cIPAddr, 0, sizeof(cIPAddr));
	memset(&cFileBuf, 0, sizeof(cFileBuf));
}
