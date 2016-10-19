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

IMPLEMENT_DYNCREATE(CSWLogDebuggingToolWView, CView)

BEGIN_MESSAGE_MAP(CSWLogDebuggingToolWView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSWLogDebuggingToolWView::OnFilePrintPreview)
	ON_COMMAND(ID_Log_Req, &CClassView::OnLogReq)
	ON_COMMAND(ID_Info_Req, &CClassView::OnInfoReq)
	ON_COMMAND(ID_Info_Load, &CClassView::OnInfoLoad)
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
	CView::OnInitialUpdate();

	m_ButtonSearch.Create(TEXT("BUTTON"), TEXT("Search"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(300,0,450,20),this,1233);
	m_EditSearch.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(150,0,300,20), this, 1232);

	m_ComboBox.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN, CRect(0,0,150,140), this, 123);
	m_ComboBox.AddString("1. ErrorLevel");
	m_ComboBox.AddString("2. Time");
	m_ComboBox.AddString("3. Path");
	m_ComboBox.AddString("4. LineNumber");
	m_ComboBox.AddString("5. Description");
	m_ComboBox.AddString("6. Total");
// 	m_btn = new CButton();
// 	m_btn->Create("a", BS_DEFPUSHBUTTON, CRect(0,0,200,50), this, 100);

}

// CSWLogDebuggingToolWView 그리기

void CSWLogDebuggingToolWView::OnDraw(CDC* pDC)
{
// 	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
// 	CSWLogDebuggingToolWView *pView = (CSWLogDebuggingToolWView *)pFrame->GetActiveView();
// 	
// 
// 	CSWLogDebuggingToolWDoc* pDoc = GetDocument();
/*	m_btn->ShowWindow(SW_SHOW);*/
	
// 	ASSERT_VALID(pDoc);
// 	if (!pDoc)
// 		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.


// 	if (m_bView == TRUE)
// 	{
// 		pDC->TextOut(0,0,m_strView);
// 		m_bView = FALSE;
// 	}
	if (m_strView.GetLength() >200)
	{
		int a = 200;
		int b = 40;
		for (int i = 0; i<m_strView.GetLength()/a;i++)
		{
			if (i == 0)
			{
				pDC->TextOut(0,b,m_strView.Mid(i*a, (i+1)*a));
			}
			else
				pDC->TextOut(0,b+i*20,m_strView.Mid(i*a, (i+1)*a));
			
		}
	}
	else
	{
		int a = 50;
		int b = 40;
		for (int i = 0; i<m_strView.GetLength()/a;i++)
		{
			if (i == 0)
			{
				pDC->TextOut(0,b + i*20,m_strView.Mid(i*a, (i+1)*a));
			} 
			else
			{
			}
			pDC->TextOut(0,b+i*20,m_strView.Mid(i*a, (i+1)*a));
		}
	}
	
	
	
	//pDC->TextOut(0,15,m_strView);
	
	
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

void CSWLogDebuggingToolWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CSWLogDebuggingToolWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
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
	if(wParam == 1233){

		int Category = m_ComboBox.GetCurSel() + 1;
		CString WantedLog1;
		GetDlgItemTextA(1232,WantedLog1);
		CT2CA pszConvertedAnsiString(WantedLog1);
		string WantedLog(pszConvertedAnsiString);

		string Title=mFilter.CreatingTime(WantedLog);
		mFilter.DoFilter(Category, WantedLog, Title, m_strViewPath);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////여기까지
	return CView::OnCommand(wParam, lParam);
}


