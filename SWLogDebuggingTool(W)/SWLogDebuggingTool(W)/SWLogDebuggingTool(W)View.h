#include "TextManager.h"
#include "Filter.h"


// SWLogDebuggingTool(W)View.h : CSWLogDebuggingToolWView Ŭ������ �������̽�
//

#pragma once


class CSWLogDebuggingToolWView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CSWLogDebuggingToolWView();
	DECLARE_DYNCREATE(CSWLogDebuggingToolWView)

	// Ư���Դϴ�.
public:
	CSWLogDebuggingToolWDoc* GetDocument() const;

	// �۾��Դϴ�.
public:

	
	Filter mFilter;
	CWnd m_EditSearch;
	CWnd m_ButtonSearch;
	CComboBox m_ComboBox;

	list<CString> m_strView;
	CString m_strViewPath;
	CSize m_textsize;

	list<CString> m_strFilteredData;
	
	
	BOOL m_bView;

private:
	void SetScrollView(int x, int y);

	// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// �����Դϴ�.
public:
	virtual ~CSWLogDebuggingToolWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // SWLogDebuggingTool(W)View.cpp�� ����� ����
inline CSWLogDebuggingToolWDoc* CSWLogDebuggingToolWView::GetDocument() const
{ return reinterpret_cast<CSWLogDebuggingToolWDoc*>(m_pDocument); }
#endif

