//#include <stdio.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//
//#pragma comment(lib,"ws2_32.lib") //Winsock Library
//
//#define SERVER "234.56.78.9"
//#define BUFSIZE 30
//#define PORT 1883
//
//
//int
//main()
//{
// WSAData wsaData;
//
// SOCKET sock;
// struct sockaddr_in addr;
// struct ip_mreq mreq;
//
// char buf[BUFSIZE];
//
// WSAStartup(MAKEWORD(2,0), &wsaData);
//
// sock = socket(AF_INET, SOCK_DGRAM, 0);
//
// addr.sin_family = AF_INET;
// addr.sin_port = htons(PORT);
// addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//
// bind(sock, (struct sockaddr *)&addr, sizeof(addr));
//
// // setsockoptは、bind以降で行う必要あり
// memset(&mreq, 0, sizeof(mreq));
// mreq.imr_interface.S_un.S_addr = htonl(INADDR_ANY);
// mreq.imr_multiaddr.S_un.S_addr = inet_addr(SERVER);
//
// if (setsockopt(sock,
//	IPPROTO_IP,
//	IP_ADD_MEMBERSHIP,
//	(char *)&mreq, sizeof(mreq)) != 0) {
//	printf("setsockopt : %d\n", WSAGetLastError());
//	return 1;
// }
//
// memset(buf, 0, sizeof(buf));
// recv(sock, buf, sizeof(buf), 0);
//
// printf("%s\n", buf);
//
// closesocket(sock);
//
// WSACleanup();
//
// return 0;
//}
//
//
//#include<stdio.h>
//#include<winsock.h>
//
//#pragma comment(lib,"ws2_32.lib") //Winsock Library
//
//#define MULTICAST_GROUP "234.56.78.9" //Multicast Group Addr
//#define LOCAL_ADDR "202.31.137.116" //Local Interface Addr
//
//int main(){
//	SOCKET iSocket;
//	SOCKADDR_IN localif;
//	struct ip_mreq mreq;
//	int BufLen = 512;
//	char Buf[512];
//	int iSLen, iMLen, iRecvLen;
//	unsigned int iYes = 1;
//	WSADATA wsa;
//
//	printf("\nInitialising Winsock...");
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
//		printf("Failed. Error Code : %d", WSAGetLastError());
//		exit(EXIT_FAILURE);
//	}
//	printf("Initialised.\n");
//
//	iSocket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//
//	localif.sin_family = AF_INET;
//	localif.sin_port = htons(1883);
//	localif.sin_addr.S_un.S_addr= htonl(INADDR_ANY);
//	bind(iSocket, (SOCKADDR*)&localif, sizeof(localif));
//	
//	mreq.imr_interface.s_addr = htonl(INADDR_ANY);//inet_addr(LOCAL_ADDR);
//	mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);
//
//	int optval = 128;
//	setsockopt(iSocket, IPPROTO_IP, IP_MULTICAST_TTL,(char*)&optval, sizeof(int));
//
//	//BOOL bOptVal = TRUE;
//	setsockopt(iSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&iYes, sizeof(iYes));
//	/*
//	int iTimeOut = 1000;
//	setsockopt(iSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeOut, sizeof(iTimeOut));
//	*/
//	iSLen = sizeof(localif);
//	iMLen = sizeof(mreq);
//
//	while (1)
//   {
//      printf("Waiting for data...");
//      fflush(stdout);
//
//      //clear the buffer by filling null, it might have previously received data
//      memset(Buf, '\0', BufLen);
//
//      //try to receive some data, this is a blocking call
//      if ((iRecvLen = recvfrom(iSocket, Buf, BufLen, 0, NULL/*(struct sockaddr *)&localif*/, 0/*&iSLen*/) == SOCKET_ERROR))
//      {
//         printf("recvfrom() failed with error code : %d", WSAGetLastError());
//         exit(EXIT_FAILURE);
//      }
//
//      //print details of the client/peer and the data received
//      printf("Received packet from %s:%d\n", inet_ntoa(localif.sin_addr), ntohs(localif.sin_port));
//      printf("Data: %s\n", Buf);
//   }
//
//   closesocket(iSocket);
//   WSACleanup();
//
//   return 0;
//}
//
#include "Ucommunicatino_Agent.h"

#define SERVER "234.56.78.9"
#define BUFSIZE 2048
#define PORT 1883


using namespace std;

Ucommunicatino_Agent mUcommunicatino_Agent = Ucommunicatino_Agent();

int main(void){
	
	mUcommunicatino_Agent.InitSocket_Ag();

	while(1)
	{
		int iKindMsg = mUcommunicatino_Agent.RcvMsgDscr(); //RcvMsgDscr함수를 이용하여 와처로 전송된 메시지의 종류를 판단한다.

		switch(iKindMsg)
		{
		case 0:
			//와처가 에이전트가 실행중인 노드의 정보
			//IP, Agent가 실행중인 PC의 이름, 로그파일이 존재하는 경로, 파일 경로에 존재하는 파일 목록을 요구하는 메시지를 보낸 케이스
			mUcommunicatino_Agent.SendAgentInfor();
			break;
		case 1:
			//와처가 에이전트로부터 특정파일의 전송을 요구한 경우
			mUcommunicatino_Agent.SendLogData();
			break;
		case -1:
			//와처로 부터 잘못된 메시지를 수신한 경우
			printf("메시지 분석 실패 또는 잘못된 메시지 수신\n");
			break;
		}
	}

	closesocket(mUcommunicatino_Agent.hSndSock);
	closesocket(mUcommunicatino_Agent.hRecvSock);
	WSACleanup();
	return 0;

}