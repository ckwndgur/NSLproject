// SWLogDebuggingTool(W)View.cpp : CSWLogDebuggingToolWView 클래스의 구현
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W)Doc.h"
#include "SWLogDebuggingTool(W)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSWLogDebuggingToolWView

IMPLEMENT_DYNCREATE(CSWLogDebuggingToolWView, CScrollView)

BEGIN_MESSAGE_MAP(CSWLogDebuggingToolWView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSWLogDebuggingToolWView::OnFilePrintPreview)
END_MESSAGE_MAP()

CClassView* pClassView = (CClassView*)AfxGetApp();
// CSWLogDebuggingToolWView 생성/소멸

CSWLogDebuggingToolWView::CSWLogDebuggingToolWView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CSWLogDebuggingToolWView::~CSWLogDebuggingToolWView()
{
	/*delete m_btn;*/
}

BOOL CSWLogDebuggingToolWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	
//	cs.style &= (ES_AUTOHSCROLL|ES_AUTOVSCROLL);
	

	BOOL a = CView::PreCreateWindow(cs);
	//cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL;

	return a;
}

void CSWLogDebuggingToolWView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

// 	m_ButtonSearch.Create(TEXT("BUTTON"), TEXT("Search"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(300,0,450,20),this,1233);
// 	m_EditSearch.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(150,0,300,20), this, 1232);
// 
// 	m_ComboBox.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN, CRect(0,0,150,140), this, 123);
// 	m_ComboBox.AddString("1. ErrorLevel");
// 	m_ComboBox.AddString("2. Time");
// 	m_ComboBox.AddString("3. Path");
// 	m_ComboBox.AddString("4. LineNumber");
// 	m_ComboBox.AddString("5. Description");
// 	m_ComboBox.AddString("6. Total");
	SetScrollView(0, 0);

}

//scrollview size calculate
void CSWLogDebuggingToolWView::SetScrollView(int x, int y)
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

// CSWLogDebuggingToolWView 그리기

void CSWLogDebuggingToolWView::OnDraw(CDC* pDC)
{

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


// CSWLogDebuggingToolWView 인쇄


void CSWLogDebuggingToolWView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CSWLogDebuggingToolWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CSWLogDebuggingToolWView::OnBeginPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CSWLogDebuggingToolWView::OnEndPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
	

}

void CSWLogDebuggingToolWView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);

}

void CSWLogDebuggingToolWView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CSWLogDebuggingToolWView 진단

#ifdef _DEBUG
void CSWLogDebuggingToolWView::AssertValid() const
{
	CView::AssertValid();
}

void CSWLogDebuggingToolWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSWLogDebuggingToolWDoc* CSWLogDebuggingToolWView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSWLogDebuggingToolWDoc)));
	return (CSWLogDebuggingToolWDoc*)m_pDocument;
}
#endif //_DEBUG



BOOL CSWLogDebuggingToolWView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(wParam == 1234){

		int Category = m_ComboBox.GetCurSel() + 1;
		CString WantedLog1;
		GetDlgItemTextA(1232,WantedLog1);
		CT2CA pszConvertedAnsiString(WantedLog1);
		string WantedLog(pszConvertedAnsiString);

		string Title=mFilter.CreatingTime(WantedLog);
		m_strFilteredData = mFilter.DoFilter(Category, WantedLog, Title, m_strViewPath);

	}
	///////////////////////////////////////////////////////////////////////////////////////////////////여기까지

	return CView::OnCommand(wParam, lParam);
}


