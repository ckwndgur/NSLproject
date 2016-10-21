#include "UDPCommunication.h"
#include "AgtInfoList.h"
#include "TextManager.h"
#include "OutputWnd.h"
#include "XMLManager.h"
#include "GetDataFromAgent_WIN.h"
#include "ClassView.h"
#include "Filter.h"


// SWLogDebuggingTool(W)View.h : CSWLogDebuggingToolWView 클래스의 인터페이스
//

#pragma once


class CSWLogDebuggingToolWView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CSWLogDebuggingToolWView();
	DECLARE_DYNCREATE(CSWLogDebuggingToolWView)

	// 특성입니다.
public:
	CSWLogDebuggingToolWDoc* GetDocument() const;

	// 작업입니다.
public:

	COutputWnd mCOutputWnd;
	Filter mFilter;
	CWnd m_EditSearch;
	CWnd m_ButtonSearch;
	CComboBox m_ComboBox;

	

	list<CString> m_strView;
	CString m_strViewPath;
	CSize m_textsize;
	
	
	BOOL m_bView;

private:
	void SetScrollView(int x, int y);

	// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 구현입니다.
public:
	virtual ~CSWLogDebuggingToolWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // SWLogDebuggingTool(W)View.cpp의 디버그 버전
inline CSWLogDebuggingToolWDoc* CSWLogDebuggingToolWView::GetDocument() const
{ return reinterpret_cast<CSWLogDebuggingToolWDoc*>(m_pDocument); }
#endif

