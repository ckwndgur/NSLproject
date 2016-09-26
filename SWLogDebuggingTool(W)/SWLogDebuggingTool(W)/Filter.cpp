#include "StdAfx.h"
#include "Filter.h"

Filter::Filter()
{
}

Filter::~Filter()
{
}

void Filter::Init()
{
	int CategoryPass = 1;
	do{
		cout << "-------------------- 검색할 카테고리를 고르시오. ---------------------------" << endl;
		cout << "   1.ERROR      2.TIME      3.PATH      4.LINE      5.INFORMATION    6.Total" << endl;
		cin >> Category;
		switch(Category){
			case 1:
				cout << "1.ERROR" << endl;
				cout << "검색할 단어 : ";
				cin >> WantedLog;
				CategoryPass = 0;
				break;
			case 2:
				cout << "2.TIME (입력방법 : YYMMDD_hhmmssmsmsms~YYMMDD_hhmmssmsmsms" << endl;
				cout << "검색할 시간 : ";
				cin >> WantedLog;
				CategoryPass = 0;
				break;
			case 3:
				cout << "3.PATH" << endl;
				cout << "검색할 폴더 : ";
				cin >> WantedLog;
				CategoryPass = 0;
				break;
			case 4:
				cout << "4.LINE" << endl;
				cout << "검색할 라인 : ";
				cin >> WantedLog;
				CategoryPass = 0;
				break;
			case 5:
				cout << "5.INFORMATION" << endl;
				cout << "검색할 정보 : ";
				cin >> WantedLog;
				CategoryPass = 0;
				break;
			case 6:
				cout << "6.Total" << endl;
				cout << "검색할 단어 : ";
				cin >> WantedLog;
				CategoryPass = 0;
				break;
			default:
				cout << "1~5인 값을 입력해주세요." << endl;
				break;
		}
	}while (CategoryPass);
}


int Filter::DoFilter()
{
	SYSTEMTIME lst;
	GetLocalTime(&lst);
	char buffer[40];
	sprintf_s(buffer,sizeof(buffer),"%04d-%02d-%02d %02dhour%02dmin%02dsec.txt", lst.wYear, lst.wMonth, lst.wDay, lst.wHour, lst.wMinute, lst.wSecond);
	string Title1;
	Title1 = buffer;
	Title1 = "["+WantedLog + "]  "+ Title1;
	char* Title = new char[Title1.size() +1];
	strcpy_s(Title,Title1.size() + 1, Title1.c_str());
	ifstream input("LogFile.txt", ios::in);
	if(input.fail()){
			cout << "파일을 여는 데 실패했습니다."<< endl;
	}
	ofstream output(Title, ios::out);
	if(output.fail()){
			cout << "파일을 쓰는 데 실패했습니다.1"<< endl;
	}
	int LogPartStartPoint= 0, LogPartEndPoint =0;
	string TimeWantedLog[2];
	if(Category == 2){
		TimeWantedLog[0].assign(WantedLog,0, WantedLog.find("~"));
		TimeWantedLog[1].assign(WantedLog, WantedLog.find("~")+1, WantedLog.length());
	}
	while(!input.eof()){

		getline(input, Log);
		if(Log.length() <= 0){
			return 0;
		}
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
		default : cout << "해당 카테고리는 존재하지 않습니다." << endl;
		}
		TargetPart.assign(Log, LogPartStartPoint, LogPartEndPoint-LogPartStartPoint);
		if(Category == 2){
			if((TargetPart>=TimeWantedLog[0]) & (TargetPart<=TimeWantedLog[1])){
				FilteredLog = Log;
				output << FilteredLog << endl;
			}
		}
		else{
			if(((TargetPart.find(WantedLog) >=0) & (TargetPart.find(WantedLog) < TargetPart.length()))){
				FilteredLog = Log;
				output << FilteredLog << endl;
			}
		}
	}
	input.close();
	output.close();

	return 0;
}


int main()
{
	Filter filter;
	filter.Init();

	LARGE_INTEGER startingTime, endingTime, elapsed;
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startingTime);

	int Result = filter.DoFilter();

	QueryPerformanceCounter(&endingTime);
	elapsed.QuadPart = endingTime.QuadPart - startingTime.QuadPart;
	elapsed.QuadPart *= 1000000;
	elapsed.QuadPart /= frequency.QuadPart;

	cout << "Running Time(us) : " << elapsed.QuadPart << "us" << endl;

	return 0;
}