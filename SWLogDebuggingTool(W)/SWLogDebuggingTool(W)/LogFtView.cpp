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
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void LogFtView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
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
