// SWLogDebuggingTool(W)View.cpp : CSWLogDebuggingToolWView Ŭ������ ����
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
	// ǥ�� �μ� �����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSWLogDebuggingToolWView::OnFilePrintPreview)
	ON_COMMAND(ID_Log_Req, &CClassView::OnLogReq)
	ON_COMMAND(ID_Info_Req, &CClassView::OnInfoReq)
	ON_COMMAND(ID_Info_Load, &CClassView::OnInfoLoad)
END_MESSAGE_MAP()

CClassView* pClassView = (CClassView*)AfxGetApp();
// CSWLogDebuggingToolWView ����/�Ҹ�

CSWLogDebuggingToolWView::CSWLogDebuggingToolWView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CSWLogDebuggingToolWView::~CSWLogDebuggingToolWView()
{
	/*delete m_btn;*/
}

BOOL CSWLogDebuggingToolWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.


	return CView::PreCreateWindow(cs);
}

void CSWLogDebuggingToolWView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

// 	m_btn = new CButton();
// 	m_btn->Create("a", BS_DEFPUSHBUTTON, CRect(0,0,200,50), this, 100);

}

// CSWLogDebuggingToolWView �׸���

void CSWLogDebuggingToolWView::OnDraw(CDC* pDC)
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CSWLogDebuggingToolWView *pView = (CSWLogDebuggingToolWView *)pFrame->GetActiveView();
	

	CSWLogDebuggingToolWDoc* pDoc = GetDocument();
/*	m_btn->ShowWindow(SW_SHOW);*/
	
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	pDC->TextOut(0,0,_T("sdsa"));
	
}


// CSWLogDebuggingToolWView �μ�


void CSWLogDebuggingToolWView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CSWLogDebuggingToolWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CSWLogDebuggingToolWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CSWLogDebuggingToolWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.

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


// CSWLogDebuggingToolWView ����

#ifdef _DEBUG
void CSWLogDebuggingToolWView::AssertValid() const
{
	CView::AssertValid();
}

void CSWLogDebuggingToolWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSWLogDebuggingToolWDoc* CSWLogDebuggingToolWView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSWLogDebuggingToolWDoc)));
	return (CSWLogDebuggingToolWDoc*)m_pDocument;
}
#endif //_DEBUG


// CSWLogDebuggingToolWView �޽��� ó����