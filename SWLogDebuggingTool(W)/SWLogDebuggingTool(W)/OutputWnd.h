
#pragma once
/*#include "FolderManager.h"*/
#include "TreeviewManager.h"
#include "TreeviewData.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList â
class COutputList : public CListBox
{
// �����Դϴ�.
public:
	COutputList();

// �����Դϴ�.
public:
	virtual ~COutputList();
	


protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
// �����Դϴ�.
public:
	COutputWnd();

	list<CString> m_strFilteredDataOut;

// Ư���Դϴ�.
protected:
	CFont m_Font;

	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutputBuild;
	COutputList m_wndOutputDebug;
	COutputList m_wndOutputFind;

	

protected:
	

	void AdjustHorzScroll(CListBox& wndListBox);

// �����Դϴ�.
public:
	void FillBuildWindow(list<CString> input);
	void FillDebugWindow(list<CString> input);
	void FillFindWindow(list<CString> input);
	virtual ~COutputWnd();
	/*FolderManager m_FolderManager;*/
	TreeviewManager m_TreeviewManager;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:
	
};

