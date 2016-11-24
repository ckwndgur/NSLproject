// LogFileView.cpp : implementation file
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "LogFileView.h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W)Doc.h"
#include "stdlib.h"
#include "string.h"


#define ERRORITEM	1
#define DATEITEM	2
#define PATHITEM	3
#define LINEITEM	4
#define DESITEM		5


// LogFileView

IMPLEMENT_DYNCREATE(LogFileView, CScrollView)

LogFileView::LogFileView()
{

}

LogFileView::~LogFileView()
{
}


BEGIN_MESSAGE_MAP(LogFileView, CScrollView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// LogFileView drawing

void LogFileView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//ListView�߰�
	m_OriginLoglist.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
						   CRect(0, 0, 1000, 100), this, 1234);
    m_OriginLoglist.ShowScrollBar(SB_BOTH);

	/*
	//�÷� �߰�
	m_OriginLoglist.InsertColumn(0, "No.", LVCFMT_LEFT, 40);
	m_OriginLoglist.InsertColumn(1, "Error Level", LVCFMT_LEFT, 100);
	m_OriginLoglist.InsertColumn(2, "Date", LVCFMT_LEFT, 300);
	m_OriginLoglist.InsertColumn(3, "Path", LVCFMT_LEFT, 100);
	m_OriginLoglist.InsertColumn(4, "Line number", LVCFMT_LEFT, 100);
	m_OriginLoglist.InsertColumn(5, "Description", LVCFMT_LEFT, 200);
*/
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	
	SetScrollView(0, 0);
}

void LogFileView::SetScrollView(int x, int y)
{
	if ((x==0)&&(y==0))
	{
		CRect rc;
		GetClientRect(&rc);
		CSize sizeViewPage;
		sizeViewPage.cx = rc.right-rc.left;
		sizeViewPage.cy = rc.bottom-rc.top;
		SetScrollSizes(MM_TEXT, sizeViewPage);
	} 
	else
	{
		CRect rc;
		GetClientRect(&rc);
		CSize sizeViewPage;

		int check_x = rc.right - rc.left;
		if (x < check_x)
		{
			sizeViewPage.cx = check_x;
		}
		else
		{
			sizeViewPage.cx = x;
		}

		int check_y = rc.bottom - rc.top;
		if (y < check_y)
		{
			sizeViewPage.cy = check_y;
		}
		else
		{
			sizeViewPage.cy = y;
		}


		SetScrollSizes(MM_TEXT, sizeViewPage);
	}

}

void LogFileView::OnDraw(CDC* pDC)
{
	/*
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	if (m_strView.size() >0)
	{
		//int C = 40;
		int i = 0;
		SetScrollView(m_textsize.cx * 8, m_textsize.cy*20);
		//text size * 8 -> wnd x size
		//first line height + total line number * each line height
		for (list<CString>::iterator iterPos = m_strView.begin(); iterPos != m_strView.end(); ++iterPos, ++i)
		{
			pDC->TextOut(0, i*20, *iterPos);
		}
	}
	*/

	if(m_openflag == TRUE)
	{
		//LogtoList(openfilepath);
		DLogtoList(openfilepath);
	}

	m_openflag = FALSE;
}

BOOL LogFileView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
// 	if(wParam == 1233){
// 
// 		int Category = m_ComboBox.GetCurSel() + 1;
// 		CString WantedLog1;
// 		GetDlgItemTextA(1232,WantedLog1);
// 		CT2CA pszConvertedAnsiString(WantedLog1);
// 		string WantedLog(pszConvertedAnsiString);
// 
// 		string Title=mFilter.CreatingTime(WantedLog);
// 		m_strFilteredData = mFilter.DoFilter(Category, WantedLog, Title, m_strViewPath);
// 
// 	}
	///////////////////////////////////////////////////////////////////////////////////////////////////�������

	return CView::OnCommand(wParam, lParam);
}

//�������� ����Ʈ�信 �α����� ����
void LogFileView::DLogtoList(CString filepath)
{
	ifstream originfile(filepath); //������ ����� �α������� ���� �����մϴ�.
	string alinelog, alinelog_buf;
	alinelog = alinelog_buf = "";
	CString lineno;
	int valstart, valend, listno, strlength, columnno;
	valstart = 1;
	valend = listno = strlength = columnno = 0;
	string liststr_buf;
	CString liststr;//����Ʈ�� ��������ۿ� �� ���Դϴ�.
	liststr = "";
	bool inputflag;
	inputflag = true;

	Filter *pfilter = new Filter;
	pfilter->LogColumn  = "";

	while(!originfile.eof())
	{
		getline(originfile, alinelog);//1�� ������ ��Ʈ������ �о�ɴϴ�.
		alinelog_buf = alinelog;
		inputflag = true;
		columnno = 0;

		//����Ʈ���� �÷��� ù��° �������� �����մϴ�.
		if(listno == 0)
		{
			m_OriginLoglist.InsertColumn(0, "No.", LVCFMT_LEFT, 40);
			lineno.Format(_T("%d"), listno+1);
			m_OriginLoglist.InsertItem(listno, lineno, 0);

			while(inputflag)
			{
				valstart = alinelog_buf.find("��");
				valend = alinelog_buf.find("��");
				strlength = valend - valstart;
				liststr_buf = alinelog_buf.substr(valstart+2, strlength-2);
				liststr = liststr_buf.c_str();
				m_OriginLoglist.InsertColumn(columnno+1, liststr, LVCFMT_LEFT, 150);

				pfilter->LogColumn  += "*";
				pfilter->LogColumn  += liststr;

				alinelog_buf.erase(valstart,strlength+2);
				valstart = alinelog_buf.find("��");

				liststr_buf = alinelog_buf.substr(0, valstart);
				liststr = liststr_buf.c_str();
				
				m_OriginLoglist.SetItemText(listno, columnno+1, liststr);//����������� �߰��մϴ�.				
				alinelog_buf.erase(0,valstart);
				
				valstart = alinelog_buf.find("��");

				if(valstart == -1)
				{
					inputflag = false;
				}

				else
				{
				}

				columnno++;
			}

			listno++;

		}
		
		//����Ʈ���� ������ �������� �߰��մϴ�.
		else
		{
			//m_OriginLoglist.InsertColumn(0, "No.", LVCFMT_LEFT, 40);
			lineno.Format(_T("%d"), listno+1);
			m_OriginLoglist.InsertItem(listno, lineno, 0);

			while(inputflag)
			{
				valstart = alinelog_buf.find("��");
				valend = alinelog_buf.find("��");
				strlength = valend - valstart;
				liststr_buf = alinelog_buf.substr(valstart+2, strlength-2);
				liststr = liststr_buf.c_str();
				//m_OriginLoglist.InsertColumn(columnno+1, liststr, LVCFMT_LEFT, 150);

				alinelog_buf.erase(valstart,strlength+2);
				valstart = alinelog_buf.find("��");

				liststr_buf = alinelog_buf.substr(0, valstart);
				liststr = liststr_buf.c_str();
				
				m_OriginLoglist.SetItemText(listno, columnno+1, liststr);//����������� �߰��մϴ�.
				alinelog_buf.erase(0,valstart);
				
				valstart = alinelog_buf.find("��");

				if(valstart == -1)
				{
					inputflag = false;
				}

				else
				{
				}

				columnno++;
			}

			listno++;
		}
	}

	listno = 0;
}

void LogFileView::LogtoList(CString filepath)
{
	ifstream originfile(filepath); //������ ����� �α������� ���� �����մϴ�.
	string alinelog;
	alinelog = "";
	CString lineno;
	int valstart, valend, listno, strlength;
	valstart = 1;
	valend = listno = strlength = 0;
	string liststr_buf;
	CString liststr;//����Ʈ�� ��������ۿ� �� ���Դϴ�.
	liststr = "";

	while(!originfile.eof())
	{
		lineno.Format(_T("%d"), listno+1);
		m_OriginLoglist.InsertItem(listno, lineno, 0);//����Ʈ�� ������ �߰��մϴ�.

		getline(originfile, alinelog);//1�� ������ ��Ʈ������ �о�ɴϴ�.

		valend = alinelog.find(")");
		strlength = valend - valstart;
		liststr_buf = alinelog.substr(valstart, strlength);		
		liststr = liststr_buf.c_str();
		m_OriginLoglist.SetItemText(listno, ERRORITEM, liststr); //���������� �߰��մϴ�.

		valstart = alinelog.find("[") + 1;
		valend = alinelog.find("]_");
		strlength = valend - valstart;
		liststr_buf = alinelog.substr(valstart, strlength);
		liststr = liststr_buf.c_str();
		m_OriginLoglist.SetItemText(listno, DATEITEM, liststr); //��¥�� �߰��մϴ�.

		alinelog = "";
		valstart = 1;
		valend = 0;
		listno++;
	}

	listno = 0;
}

//����Ʈ���� �������� �޾ƿɴϴ�.
void ListtoLog()
{
	
}



// LogFileView diagnostics

#ifdef _DEBUG
void LogFileView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void LogFileView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// LogFileView message handlers

void LogFileView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	if (m_OriginLoglist)
	{
		CRect rc;
		GetClientRect(&rc);
		m_OriginLoglist.SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOZORDER|SWP_SHOWWINDOW|SWP_NOACTIVATE);
	}
	// TODO: Add your message handler code here
}
