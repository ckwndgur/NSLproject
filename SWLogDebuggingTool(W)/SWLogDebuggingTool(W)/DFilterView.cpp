// DFilterView.cpp : implementation file
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "DFilterView.h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W)Doc.h"
#include "stdlib.h"
#include "string.h"
#include "FileView.h"


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

	//MakeFilterForm(filterform);

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 200;

	m_EditERROR.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,0,250,20), this, 327889);
	m_EditDATE.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,30,250,50), this, 327890);
	m_EditPATH.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,60,250,80), this, 327891);
	m_EditLINE.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,90,250,110), this, 327892);
	m_EditDES.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,120,250,140), this, 327893);
	m_EditTOTAL.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,150,250,170), this, 327894);

	m_StaticERROR.Create(_T("ERROR"),WS_CHILD|WS_VISIBLE, CRect(20,0,120,20), this, 327895);
	m_StaticDATE.Create(_T("DATE"),WS_CHILD|WS_VISIBLE, CRect(20,30,120,50), this, 327896);
	m_StaticPATH.Create(_T("PATH"),WS_CHILD|WS_VISIBLE, CRect(20,60,120,80), this, 327897);
	m_StaticLINE.Create(_T("LINE"),WS_CHILD|WS_VISIBLE, CRect(20,90,120,110), this, 327898);
	m_StaticDES.Create(_T("DESCRIPTION"),WS_CHILD|WS_VISIBLE, CRect(20,120,120,140), this, 327899);
	m_StaticTOTAL.Create(_T("TOTAL"),WS_CHILD|WS_VISIBLE, CRect(20,150,120,170), this, 327900);

	m_ButtonSearch.Create(TEXT("BUTTON"), TEXT("Search"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(50,180,200,200), this, 327901);


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

BOOL DFilterView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(wParam == 221)
	{

		CString Errorstr;
		GetDlgItemTextA(327889,Errorstr);
		CT2CA pszConvertedAnsiString(Errorstr);
		string Wantederror(pszConvertedAnsiString);

		string Title= mFilter.CreatingTime(Wantederror);
		m_strFilteredData.clear();
		m_strFilteredData = mFilter.DoFilter(1, Wantederror, Title, m_filePath);
		this->Invalidate(TRUE);

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////여기까지

	return CView::OnCommand(wParam, lParam);
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
