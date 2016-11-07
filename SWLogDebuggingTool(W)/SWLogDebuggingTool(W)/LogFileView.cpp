// LogFileView.cpp : implementation file
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "LogFileView.h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W)Doc.h"


// LogFileView

IMPLEMENT_DYNCREATE(LogFileView, CScrollView)

LogFileView::LogFileView()
{

}

LogFileView::~LogFileView()
{
}


BEGIN_MESSAGE_MAP(LogFileView, CScrollView)
END_MESSAGE_MAP()


// LogFileView drawing

void LogFileView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_ButtonSearch.Create(TEXT("BUTTON"), TEXT("Search"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(300,0,450,20),this,1233);
	m_EditSearch.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(150,0,300,20), this, 1232);

	m_ComboBox.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN, CRect(0,0,150,140), this, 123);
	m_ComboBox.AddString("1. ErrorLevel");
	m_ComboBox.AddString("2. Time");
	m_ComboBox.AddString("3. Path");
	m_ComboBox.AddString("4. LineNumber");
	m_ComboBox.AddString("5. Description");
	m_ComboBox.AddString("6. Total");
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
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	if (m_strView.size() >0)
	{
		int C = 40;
		int i = 0;
		SetScrollView(m_textsize.cx * 8, C + m_textsize.cy*20) ;
		//text size * 8 -> wnd x size
		//first line height + total line number * each line height
		for (list<CString>::iterator iterPos = m_strView.begin(); iterPos != m_strView.end(); ++iterPos, ++i)
		{
			pDC->TextOut(0, C+i*20, *iterPos);
		}
	}
}

BOOL LogFileView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(wParam == 1233){

		int Category = m_ComboBox.GetCurSel() + 1;
		CString WantedLog1;
		GetDlgItemTextA(1232,WantedLog1);
		CT2CA pszConvertedAnsiString(WantedLog1);
		string WantedLog(pszConvertedAnsiString);

		string Title=mFilter.CreatingTime(WantedLog);
		m_strFilteredData = mFilter.DoFilter(Category, WantedLog, Title, m_strViewPath);

	}
	///////////////////////////////////////////////////////////////////////////////////////////////////�������

	return CView::OnCommand(wParam, lParam);
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
