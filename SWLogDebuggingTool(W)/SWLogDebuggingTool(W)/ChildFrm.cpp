
// ChildFrm.cpp : CChildFrame Ŭ������ ����
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "LogFileView.h"
#include "LogFtView.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

// CChildFrame ����/�Ҹ�

CChildFrame::CChildFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	if (!m_wndSplitter.CreateStatic(this, 2, 1))
	{
		return FALSE;
	}

	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(LogFileView), CSize(100, 100), pContext);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(LogFtView), CSize(100, 100), pContext);

	return TRUE;
}

CScrollView* CChildFrame::GetFtViewPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(1, 0);
	CScrollView* pView = DYNAMIC_DOWNCAST(CScrollView, pWnd);
	return pView;
}

CScrollView* CChildFrame::GetFileViewPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CScrollView* pView = DYNAMIC_DOWNCAST(CScrollView, pWnd);
	return pView;
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame ����

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame �޽��� ó����


