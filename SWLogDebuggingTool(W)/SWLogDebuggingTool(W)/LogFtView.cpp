// LogFtView.cpp : implementation file
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "LogFtView.h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W)Doc.h"



// LogFtView

IMPLEMENT_DYNCREATE(LogFtView, CScrollView)

LogFtView::LogFtView()
{
	
}

LogFtView::~LogFtView()
{
}


BEGIN_MESSAGE_MAP(LogFtView, CScrollView)
END_MESSAGE_MAP()


// LogFtView drawing

void LogFtView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;

	m_ButtonSearch.Create(TEXT("BUTTON"), TEXT("Search"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(300,0,450,20),this,1233);
	m_EditSearch.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(150,0,300,20), this, 1232);

	m_ComboBox.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN, CRect(0,0,150,140), this, 123);
	m_ComboBox.AddString("1. ErrorLevel");
	m_ComboBox.AddString("2. Time");
	m_ComboBox.AddString("3. Path");
	m_ComboBox.AddString("4. LineNumber");
	m_ComboBox.AddString("5. Description");
	m_ComboBox.AddString("6. Total");

	SetScrollSizes(MM_TEXT, sizeTotal);

}

void LogFtView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	FillFtView(m_strFilteredData, pDC);
	
}

BOOL LogFtView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(wParam == 1233){

		int Category = m_ComboBox.GetCurSel() + 1;
		CString WantedLog1;
		GetDlgItemTextA(1232,WantedLog1);
		CT2CA pszConvertedAnsiString(WantedLog1);
		string WantedLog(pszConvertedAnsiString);

		string Title= mFilter.CreatingTime(WantedLog);
		m_strFilteredData.clear();
		m_strFilteredData = mFilter.DoFilter(Category, WantedLog, Title, m_strViewPath);
		this->Invalidate(TRUE);

	}
	///////////////////////////////////////////////////////////////////////////////////////////////////여기까지

	return CView::OnCommand(wParam, lParam);
}

void LogFtView::SetScrollView(int x, int y)
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

void LogFtView::FillFtView(list<CString> input, CDC* pdc)
{
	int C = 30;
	int i = 0;
	if (input.size() >0 )
	{
		SetScrollView(m_textsize.cx * 8, C + m_textsize.cy*20);
		for (list<CString>::iterator iterPos = input.begin(); iterPos != input.end(); ++iterPos, ++i)
		{
			pdc->TextOut(0, C + i*20, *iterPos);
		}
	}

}


// LogFtView diagnostics

#ifdef _DEBUG
void LogFtView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void LogFtView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// LogFtView message handlers
