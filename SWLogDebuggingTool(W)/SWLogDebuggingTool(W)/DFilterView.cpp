// DFilterView.cpp : implementation file
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "DFilterView.h"


// DFilterView

IMPLEMENT_DYNCREATE(DFilterView, CScrollView)

DFilterView::DFilterView()
{

}

DFilterView::~DFilterView()
{
}


BEGIN_MESSAGE_MAP(DFilterView, CScrollView)
END_MESSAGE_MAP()


// DFilterView drawing

void DFilterView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void DFilterView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// DFilterView diagnostics

#ifdef _DEBUG
void DFilterView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void DFilterView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// DFilterView message handlers
