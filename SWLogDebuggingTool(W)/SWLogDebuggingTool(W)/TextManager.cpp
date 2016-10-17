#include "StdAfx.h"
#include "TextManager.h"
#include <fstream>
using namespace std;
using std::ifstream;
using std::ofstream;

TextManager::TextManager(void)
{
}

TextManager::~TextManager(void)
{
}

void TextManager::WriteText(char* filedirectory, char* filename, string data)
{
	//ofstream ofile;.
	char ctemp[256] ;

	strcpy(ctemp, mStringManager.AppendChar(filedirectory, filename));

	//ctemp[strlen(ctemp) - 1] = NULL;

	ofstream ofile(ctemp, ios_base::out | ios_base::app);
	ofile<< data <<endl;
	ofile.close();
}


string TextManager::ReadText(char* filedirectory, char* filename)
{
	ifstream infile;
	char input[2048];
	string strOutput = "";

	infile.open(mStringManager.AppendChar(filedirectory, filename));

	while(!infile.eof()){
		infile.getline(input, 2048);
		strOutput.append(input);
		strOutput.append("\n");
	}

	infile.close();

	return strOutput;
}

string TextManager::ReadText(char* fulldirectory)
{
	ifstream infile;
	char input[2048];
	string strOutput = "";

	infile.open(fulldirectory);

	while(!infile.eof()){
		infile.getline(input, 2048);
		strOutput.append(input);
		strOutput.append("\n");
	}

	infile.close();

	return strOutput;
}

int TextManager::GetFileSize(char* filedirectory, char* filename){
	FILE *fpMe;
	long len;

	if((fpMe = fopen(mStringManager.AppendChar(filedirectory, filename),"rb")) == NULL)
	{
		printf("");
	}
	else
	{
		printf("s");
	}

	fseek(fpMe, 0L, SEEK_END);

	len = ftell(fpMe);
	printf("%d byte\n",len);
	fclose(fpMe);

	return (int)len;
}