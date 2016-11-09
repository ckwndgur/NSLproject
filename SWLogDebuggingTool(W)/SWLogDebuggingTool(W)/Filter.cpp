//////////////////////////////////////////////////////////////////////////////////////////////�ϴ� ����
#include "StdAfx.h"
#include "Filter.h"
#include "FolderManager.h"

Filter::Filter()
{
}

Filter::~Filter()
{
}



string Filter::CreatingTime(string WantedLog)
{
	SYSTEMTIME lst;
	GetLocalTime(&lst);
	char buffer[40];
	sprintf_s(buffer,sizeof(buffer),"%04d-%02d-%02d %02dhour%02dmin%02dsec.txt", lst.wYear, lst.wMonth, lst.wDay, lst.wHour, lst.wMinute, lst.wSecond);
	string Title1;
	Title1 = buffer;
	Title1 = "["+WantedLog + "]  "+ Title1;
	char* Title = new char[Title1.size() +1];
	strcpy_s(Title, Title1.size() + 1, Title1.c_str());

	return Title;
}


list<CString> Filter::DoFilter(int Category, string WantedLog, string Title, CString filepath)
{
	string Log;
	string FilteredLog;
	string TargetPart;
	string WantedLogPart[2];
	char divider;
	if(WantedLog.find("&"))
	{
		WantedLogPart[0].assign(WantedLog,0, WantedLog.find("&"));
		WantedLogPart[1].assign(WantedLog,WantedLog.find("&") + 1, WantedLog.length());
		divider = '&';
	}
	if(WantedLog.find("|"))
	{
		WantedLogPart[0].assign(WantedLog,0, WantedLog.find("|"));
		WantedLogPart[1].assign(WantedLog,WantedLog.find("|") + 1, WantedLog.length());
		divider = '|';
	}

	
	CString csfilepath, temp = ""; 
	for (int i = 0; i<4; i++)
	{
		AfxExtractSubString(temp, filepath, i, '\\');
		csfilepath += temp + '\\';
	}
	csfilepath = csfilepath + "Debug" + '\\' + Title.c_str();
	
	FolderManager mFolderManager;
	mFolderManager.MakeDirectory((LPSTR)((LPCTSTR)csfilepath));

	ifstream input((CStringA)filepath, ios::in); // 
	if(input.fail()){
		cout << "������ ���� �� �����߽��ϴ�."<< endl;
	}
	ofstream output(csfilepath, ios::out); 
	if(output.fail()){
		cout << "������ ���� �� �����߽��ϴ�.1"<< endl;
	}
	int LogPartStartPoint= 0, LogPartEndPoint =0;
	
	if((Category == 2)){
		WantedLogPart[0].assign(WantedLog,0, WantedLog.find("~"));
		WantedLogPart[1].assign(WantedLog, WantedLog.find("~")+1, WantedLog.length());
	}
	while(!input.eof()){					
		getline(input, Log);
		switch(Category)
		{
		case 1 :
			LogPartStartPoint = 1;
			LogPartEndPoint = Log.find(")");
			break;
		case 2 :
			LogPartStartPoint = Log.find("[")+1;
			LogPartEndPoint = Log.find("]");
			break;
		case 3 :
			LogPartStartPoint = Log.find("]__") +3;
			LogPartEndPoint = Log.find('.',LogPartStartPoint);
			do
			{
				LogPartEndPoint++;
			}while(Log.at(LogPartEndPoint) != '_');
			break;
		case 4 :
			LogPartStartPoint = Log.find('.');
			do{
				do{
					LogPartStartPoint++;
				}while(Log.at(LogPartStartPoint) != '_');
				LogPartEndPoint = LogPartStartPoint;
				do{
					LogPartEndPoint++;
				}while(Log.at(LogPartEndPoint) != '_');
			}while( !((LogPartEndPoint - LogPartStartPoint >= 1) & (LogPartEndPoint - LogPartStartPoint <= 8)));
			break;
		case 5 :
			LogPartStartPoint = Log.find('.');
			do{
				do{
					LogPartStartPoint++;
				}while(Log.at(LogPartStartPoint) != '_');
				LogPartEndPoint = LogPartStartPoint;
				do{
					LogPartEndPoint++;
				}while(Log.at(LogPartEndPoint) != '_');
			}while( !((LogPartEndPoint - LogPartStartPoint >= 1) & (LogPartEndPoint - LogPartStartPoint <= 8)));
			LogPartStartPoint = LogPartEndPoint+1;
			LogPartEndPoint = Log.length();
			break;
		case 6 :
			LogPartStartPoint = 0;
			LogPartEndPoint = Log.length();
			break;
		default : AfxMessageBox(TEXT("ī�װ� ���ְ� �ƴմϴ�."));
			break;
		}
		TargetPart.assign(Log, LogPartStartPoint, LogPartEndPoint-LogPartStartPoint);
		if(Category == 2){
			if((TargetPart>=WantedLogPart[0]) & (TargetPart<=WantedLogPart[1])){
				FilteredLog = Log;
				output << FilteredLog << endl;
				cslstFilteredData.push_front(FilteredLog.c_str());
			}
		}
		else{
			if(divider == '&')
			{
				if(((TargetPart.find(WantedLogPart[0]) >=0) & (TargetPart.find(WantedLogPart[0]) < TargetPart.length())) & ((TargetPart.find(WantedLogPart[0]) >=0) & (TargetPart.find(WantedLogPart[0]) < TargetPart.length())))
				{
					FilteredLog = Log;
					output << FilteredLog << endl;
					cslstFilteredData.push_front(FilteredLog.c_str());
				}
			}
			else if(divider == '|')
			{
				if(((TargetPart.find(WantedLogPart[0]) >=0) & (TargetPart.find(WantedLogPart[0]) < TargetPart.length())) | ((TargetPart.find(WantedLogPart[0]) >=0) & (TargetPart.find(WantedLogPart[0]) < TargetPart.length())))
				{
					FilteredLog = Log;
					output << FilteredLog << endl;
					cslstFilteredData.push_front(FilteredLog.c_str());
				}
			}
			else
			{
				if(((TargetPart.find(WantedLog) >=0) & (TargetPart.find(WantedLog) < TargetPart.length())))
				{
					FilteredLog = Log;
					output << FilteredLog << endl;
					cslstFilteredData.push_front(FilteredLog.c_str());
				}
			}
		}
	}
	char devider = '0';
	input.close();
	output.close();

	return cslstFilteredData;
}
//(�Է¹�� : YYMMDD_hhmmssmsmsms~YYMMDD_hhmmssmsmsms"