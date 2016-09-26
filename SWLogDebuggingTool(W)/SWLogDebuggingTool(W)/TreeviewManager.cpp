#include "StdAfx.h"
#include "TreeviewManager.h"

TreeviewManager::TreeviewManager(void)
{
	//iDirectoryIndex = 0;
}

TreeviewManager::~TreeviewManager(void)
{
}

list<CString> TreeviewManager::FindDirectory(LPCTSTR pstr)
{
	CFileFind finder;
	CString strWildcard(pstr);
	list<CString> csDirectoryList;

	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) continue;

		if (finder.IsDirectory())
		{
			csDirectoryList.push_back(finder.GetFileTitle());
		} 
		else
		{
		}
	}

	finder.Close();

	return csDirectoryList;
}

int TreeviewManager::GetCharNumber(CString str, char pstr)
{
	return str.Remove(pstr);
}

list<TreeviewData> TreeviewManager::FindLogFile(LPCTSTR pstr)
{
	CFileFind finder;
	CString strWildcard(pstr);
	

	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);
	

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) continue;

		if (finder.IsDirectory()) 
		{
			CString str = finder.GetFilePath();

			FindLogFile(str);
		}//폴더만 있을 경우에 대비해야함 -> getfiletitle == NULL
		else
		{
			CString filepath = finder.GetFilePath();
			int x =filepath.Remove('\\');
			for (int i = 0; i<x-2; i++)
			{
				CString temp;
				AfxExtractSubString(temp, finder.GetFilePath(), i+2, '\\');
				FullDirectory.push_back(temp);
			}

			TreeviewList.push_back(TreeviewData(FullDirectory, finder.GetFileTitle()));
			FullDirectory.clear();
		}
	}
	finder.Close();

	return TreeviewList;
};

// list<CString> TreeviewManager::OrderDirectory(int order)
// {
// 	list<TreeviewData> csLogFileList;
// 	list<CString> orderdirec;
// 	 
//  	if (FindLogFile("C:\\LogDebugging").size() >0 )
//  	{
// 		for (list<TreeviewData>::iterator iterPos = csLogFileList.begin(); iterPos != csLogFileList.end(); ++iterPos)
// 		{
// 			list<CString> fulldirec;
// 			fulldirec = iterPos->getFullDirectory();
// 
// 			for (list<CString>::iterator iterPoslow = fulldirec.begin(); iterPoslow != fulldirec.end(); ++iterPoslow)
// 			{
// 				int i = 1;
// 				if ((order == 0)&&(i == 1))
// 				{
// 					orderdirec.push_back(*iterPos);
// 				} 
// 				else if ((order == 1)&&(i == 2))
// 				{
// 					orderdirec.push_back(*iterPos);
// 				}
// 				if (i==1 ) i = 2;
// 				else i = 1;
// 			}
// 		}
//  	} 
// 	return orderdirec;
// };

// list<TreeviewData> TreeviewManager::SortingList(list<TreeviewData> input)
// {
// 	for (list<TreeviewData>::iterator iterPos = input.begin(); iterPos != input.end(); ++iterPos)
// 	{
// 		list<CString> fulldirec;
// 		fulldirec = iterPos->getFullDirectory();
// 		
// 		for (list<CString>::iterator iterPoslow = fulldirec.begin(); iterPoslow != fulldirec.end(); ++iterPoslow)
// 		{
// 			
// 		}
// 	}
// };


void TreeviewManager::SetupListIndex(list<TreeviewData> input)
{
	int listsize = 0;
	for (list<TreeviewData>::iterator iterPos = input.begin(); iterPos != input.end(); ++iterPos)
	{
		iterPos->setListIndex(listsize);
		listsize ++;
	}
};

// int TreeviewManager::GetListSize(list<TreeviewData> input)
// {
// 	int listsize = 0;
// 	for (list<TreeviewData>::iterator iterPos = input.begin(); iterPos != input.end(); ++iterPos)
// 	{
// 		listsize++;
// 	}
// 	
// 	return listsize;
// };
