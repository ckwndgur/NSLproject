
// ChildFrm.cpp : CChildFrame Ŭ������ ����
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "LogFileView.h"
#include "LogFtView.h"
#include "DFilterView.h"

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
	if (!m_wndSplitterDown.CreateStatic(&m_wndSplitter, 1, 2, WS_CHILD | WS_VISIBLE | WS_BORDER,m_wndSplitter.IdFromRowCol(1, 0)))
	{
		return FALSE;
	}

	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(LogFileView), CSize(300, 300), pContext);
	m_wndSplitterDown.CreateView(0, 0, RUNTIME_CLASS(LogFtView), CSize(300, 300), pContext);
	m_wndSplitterDown.CreateView(0, 1, RUNTIME_CLASS(DFilterView), CSize(100, 300), pContext);

	/*
	m_wndSplitter.SetRowInfo(0, 300, 300);
	m_wndSplitter.SetRowInfo(1, 300, 300);
	m_wndSplitterDown.SetColumnInfo(0, 200, 0);
	m_wndSplitterDown.SetColumnInfo(1, 100, 0);
*/
	return TRUE;
}

CScrollView* CChildFrame::GetFtViewPane()
{
	CWnd* pWnd = m_wndSplitterDown.GetPane(0, 0);
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


