#include "GetDataFromAgent_WIN.h"

GetDataFromAgent_WIN::GetDataFromAgent_WIN(void)
{
}

GetDataFromAgent_WIN::~GetDataFromAgent_WIN(void)
{
}

char* GetDataFromAgent_WIN::GetAgentName(int& iAgtNameSize)
{
	char cAgtName[64];
	int nError = gethostname(cAgtName, sizeof(cAgtName));
	iAgtNameSize = sizeof(cAgtName);

	return cAgtName;
}
char* GetDataFromAgent_WIN::GetAgentIP()
{
	char AgtNameInfoBuf[64];
	char AgtIPInfoBuf[15];

	int nError = gethostname(AgtNameInfoBuf, sizeof(AgtNameInfoBuf));

	if (nError == 0)
	{
		PHOSTENT pHostInfo = gethostbyname(AgtNameInfoBuf);
		// ip address �ľ�
		strcpy(AgtIPInfoBuf, inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
	}
	return AgtIPInfoBuf;
}

string GetDataFromAgent_WIN::GetAgentFileInfo(char* cAgtLogPathBuf)
{
	string sFileListOutput = "";
	char* cFileListOutput;
	char cFailReturn[2] = "F";

	WIN32_FIND_DATA FindData;
	int iFileFlag = 0;

	do
	{
		//���� ��θ� ����
		hFind = FindFirstFile(cAgtLogPathBuf, &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
		{
			//fputs("FindFirstFile() error\n", stderr);
			iFileFlag = 1;
			break;
		}
	}while(hFind==INVALID_HANDLE_VALUE);

	if(iFileFlag == 1)
		return cFailReturn;

	printf("=================================================================================\n");
	printf("���� ����Ʈ ���\n");
	printf("=================================================================================\n");
	int iDeletPoint = 0;

	do{
		if(iDeletPoint <2)
		{
			//���� ��� ������, �� ó���� �߻��ϴ� . �� .. �׸��� �����ϱ� ���� �ڵ�
			iDeletPoint++;
		}
		else if(iDeletPoint == 2)
		{
			printf("%s\n", FindData.cFileName);
			sFileListOutput += FindData.cFileName;
			sFileListOutput += "\n";
			//strcpy(cFileListOutput, FindData.cFileName);
			//strcat(cFileListOutput, "\n");
			iDeletPoint++;
		}
		else
		{
			printf("%s\n", FindData.cFileName);
			sFileListOutput += FindData.cFileName;
			sFileListOutput += "\n";
			//strcat(cFileListOutput, FindData.cFileName);
			//strcat(cFileListOutput, "\n");
		}
	}while(FindNextFile(hFind, &FindData));

	FindClose(hFind);
	//char* ��ȯ
	//cFileListOutput = (char*)sFileListOutput.c_str();
	//return cFileListOutput;
	
	//string ��ȯ
	return sFileListOutput;
}


//Filelist�� List�� ����
//list<char*>�� char array�� ��ȯ�� �� �ǵ��� ���ã����
/*
list<char*> GetDataFromAgent_WIN::GetAgentFileInfo_List(char* cAgtLogPathBuf)
{
	char cFileListOutput[2048];
	AgtInfoList MyAgtInfoList;

	do
	{
		//���� ��θ� ����
		hFind = FindFirstFile(cAgtLogPathBuf, &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
			fputs("FindFirstFile() error\n", stderr);

	}while(hFind==INVALID_HANDLE_VALUE);

	printf("=================================================================================\n");
	printf("���� ����Ʈ ���\n");
	printf("=================================================================================\n");
	
	int iDeletPoint = 0;
	
	do{
		if(iDeletPoint <2)
		{
			iDeletPoint++;
		}

		else if(iDeletPoint == 2)
		{
			printf("%s\n", FindData.cFileName);
// 			strcpy(cFileListOutput, FindData.cFileName);
// 			strcat(cFileListOutput, "\n");
	
			MyAgtInfoList.AgtFileList.push_front(FindData.cFileName);
			iDeletPoint++;
		}

		else
		{
			printf("%s\n", FindData.cFileName);
			//strcat(cFileListOutput, FindData.cFileName);
			//strcat(cFileListOutput, "\n");

			MyAgtInfoList.AgtFileList.push_back(FindData.cFileName);
		}
	}while(FindNextFile(hFind, &FindData));

	FindClose(hFind);

	return MyAgtInfoList.AgtFileList;
}
*/


char* GetDataFromAgent_WIN::GetLogData(char cLogFilePath[256], int& iCurrentPointOutput)
{
	char cLogDataOutput[8096];
	int iLoaded = 0;
	DWORD iTempNum = 0;

	memset(cLogDataOutput, 0, sizeof(cLogDataOutput));

	hFile = CreateFile(LPCSTR(cLogFilePath), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		printf("��Ȯ�� ���� ��ΰ� �ƴ�\n");
	}
	SetFilePointer(hFile, 0, NULL, iCurrentPointOutput);

	//2048�� �о���� ������

	iLoaded = ReadFile(hFile, cLogDataOutput, 8096, &iTempNum, 0);
	//
	iCurrentPointOutput = iCurrentPointOutput+iLoaded;

	return cLogDataOutput;
}