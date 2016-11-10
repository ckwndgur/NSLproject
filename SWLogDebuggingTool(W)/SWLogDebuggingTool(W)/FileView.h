
#pragma once

#include "ViewTree.h"
#include "TreeviewManager.h"
#include "TreeviewData.h"
#include <list>
#include <string>
#include <iostream>
#include <afxwin.h>
#include <Windows.h>

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// �����Դϴ�.
public:
	CFileView();
	
	

	void AdjustLayout();
	void OnChangeVisualStyle();

// Ư���Դϴ�.
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;


protected:
	void FillFileView();

// �����Դϴ�.
public:
	virtual ~CFileView();

	TreeviewManager m_TreeviewManager;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

private:
	HTREEITEM hRoot, hSrc, hInc;

	void MakeTreeview(CString pstr);
};

