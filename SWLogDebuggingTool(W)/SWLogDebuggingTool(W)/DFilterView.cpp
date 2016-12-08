// DFilterView.cpp : implementation file
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "DFilterView.h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W)Doc.h"
#include "stdlib.h"
#include "string.h"


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

	Filter *pfilter =  new Filter;
	//filterform = "";
	//filterform = pfilter->LogColumn;


	MakeFilterForm(filterform);

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

void DFilterView::MakeFilterForm(CString columnlist)
{
	int starptr;
	starptr = 0;

	CString columnlist_buf;
	columnlist_buf = "";

	
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
