#include "StdAfx.h"
#include "TCPCommunication.h"

TCPCommunication::TCPCommunication(void)
{
}

TCPCommunication::~TCPCommunication(void)
{
}


void TCPCommunication::TCPSockInit(int& iSndSock, int& iRcvSock)
{
	iSndSock=socket(PF_INET, SOCK_STREAM, 0);

	if(iSndSock == INVALID_SOCKET)
		AfxMessageBox("iSndSock error");

	iRcvSock=socket(PF_INET, SOCK_STREAM, 0);

	if(iRcvSock == INVALID_SOCKET)
		AfxMessageBox("iRcvSock error");

}

void TCPCommunication::LogFileRcv(int iRcvSocket, char* cFileDir, char* cFileName)
{
	struct AgtDataMsgStruct MyAgtDataMsg;
	DWORD dLen;

	memset(&MyAgtDataMsg, 0, sizeof(struct AgtDataMsgStruct));

	string sWriteData = "";

	mFolderManager.MakeDirectory(cFileDir, cFileName);

	recvfrom(iRcvSocket, (char*)&MyAgtDataMsg, sizeof(struct AgtDataMsgStruct), 0, NULL, 0);

	sWriteData = "";
	sWriteData = MyAgtDataMsg.cLogData;

	//WriteFile(hFile, sWriteData.c_str(), strlen(sWriteData.c_str()), &dLen, 0);
	mTextManager.WriteText(cFileDir, cFileName, sWriteData);

	if(MyAgtDataMsg.bLastPacket == TRUE)
	{
		//printf("File Download Complete.\n");
		AfxMessageBox("File Download Done");
	}
}