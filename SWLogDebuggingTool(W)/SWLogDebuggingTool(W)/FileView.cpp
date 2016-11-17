
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SWLogDebuggingTool(W).h"
#include "SWLogDebuggingTool(W)Doc.h"
#include "LogFileView.h"
#include "LogFtView.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar �޽��� ó����

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �並 ����ϴ�.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("���� �並 ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	// �� �̹����� �ε��մϴ�.
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /*���*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// ��� ����� �θ� �������� �ƴ� �� ��Ʈ���� ���� ����õ˴ϴ�.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// ���� Ʈ�� �� �����͸� ���� �ڵ�� ä��ϴ�.
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::RefreshFileView()
{
	m_wndFileView.DeleteAllItems();
	MakeTreeview("C:\\LogDebugging");
	m_wndFileView.Invalidate();
	m_wndFileView.UpdateWindow();

}
void CFileView::MakeTreeview(CString pstr) // Folder searching and make tree view + going to use SetItemData for saving information
{
	CFileFind finder;
	CString strWildcard(pstr);
	int treeindex;

	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) continue;

		if (finder.IsDirectory()) 
		{
			CString str = finder.GetFilePath();
			treeindex = m_TreeviewManager.GetCharNumber(str, '\\') - 2; //ex: C:\LogDebugging\20160417\202.31.137.116\dsa -> defaujlt: 2*/
			
			CString temp;
			AfxExtractSubString(temp, str, treeindex+2, '\\');

			if (treeindex == 0)
			{
				TreeviewData *mTreeviewData = new TreeviewData();
				hSrc = m_wndFileView.InsertItem(temp, 0, 0, hRoot);
				mTreeviewData->setFileName(temp);
				mTreeviewData->setFullDirectory(finder.GetFilePath());
				m_wndFileView.SetItemData(hSrc, (DWORD)mTreeviewData);
			} 
			else if (treeindex == 1)
			{
				TreeviewData *mTreeviewData = new TreeviewData();
				hInc = m_wndFileView.InsertItem(temp, 0, 0, hSrc);
				mTreeviewData->setFileName(temp);
				mTreeviewData->setFullDirectory(finder.GetFilePath());
				m_wndFileView.SetItemData(hInc, (DWORD)mTreeviewData);
			}

			MakeTreeview(str);
		}
		else
		{
			TreeviewData *mTreeviewData = new TreeviewData();
			m_wndFileView.InsertItem(finder.GetFileTitle(), 1, 1, hInc);
			mTreeviewData->setFileName(finder.GetFileName());
			mTreeviewData->setFullDirectory(finder.GetFilePath());
			m_wndFileView.SetItemData(hInc, (DWORD)mTreeviewData);
		}
	}
	finder.Close();
}

void CFileView::FillFileView()
{
	hRoot = m_wndFileView.InsertItem(_T("LoggDebugging"), 0, 0);
	
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
 
	MakeTreeview("C:\\LogDebugging");

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
	m_wndFileView.Expand(hInc, TVE_EXPAND);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Ŭ���� �׸��� �����մϴ�.
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		csTVDataFileName = pWndTree->GetItemText(hTreeItem);
		HTREEITEM hTreeItem_buf = hTreeItem;

		hTreeItem = pWndTree->GetNextItem(hTreeItem, TVGN_PARENT);
		CString mid = pWndTree->GetItemText(hTreeItem);

		hTreeItem = pWndTree->GetNextItem(hTreeItem, TVGN_PARENT);
		CString first = pWndTree->GetItemText(hTreeItem);


		csTVDataFilePath = "C:\\LogDebugging\\" + first + "\\" + mid + "\\" + csTVDataFileName +".txt";
		
		//mSelTVData = (TreeviewData *)pWndTree->GetItemData(hTreeItem);
		//csTVDataFileName = mSelTVData->getFileName();
		//csTVDataFilePath = mSelTVData->getFullDirectory();
		
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem_buf);
		}
	}
	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("�Ӽ�...."));

}

void CFileView::OnFileOpen()
{
	CSWLogDebuggingToolWApp *pApp = (CSWLogDebuggingToolWApp *)AfxGetApp();
	CSWLogDebuggingToolWDoc *pDoc = (CSWLogDebuggingToolWDoc *)pApp->pDocTemplate->OpenDocumentFile(csTVDataFilePath);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *) pFrame->GetActiveFrame();
	LogFileView *pView = (LogFileView *)pChild->GetFileViewPane(); 
	LogFtView *pFtView = (LogFtView *)pChild->GetFtViewPane();
	

	if (csTVDataFilePath.GetLength() >0 )
	{
		pView->m_strView = mTextManager.ReadTextList((LPSTR)(LPCTSTR)csTVDataFilePath);
		pView->m_bView = TRUE;
		pView->m_textsize = Cal_scrollview(csTVDataFilePath);

		pView->m_openflag = TRUE;
		pView->openfilepath = csTVDataFilePath;

		pFtView->m_strViewPath = csTVDataFilePath;
		pFtView->m_textsize = Cal_scrollview(csTVDataFilePath);
		
		pView->Invalidate(TRUE);
		
	} 
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

CSize CFileView::Cal_scrollview(CString fulldirectory)
{
	CSize textsize;
	
	textsize.cx = mTextManager.GetMaxLineSize((LPSTR)(LPCTSTR)fulldirectory);
	textsize.cy = mTextManager.GetLinelength((LPSTR)(LPCTSTR)fulldirectory);
	
	return textsize;
	
}

CString CFileView::getData()
{
	return csData;
}

void CFileView::OnFileOpenWith()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	hRoot = NULL;
	hInc = NULL;
	hSrc = NULL;
	m_wndFileView.DeleteAllItems();
	m_wndFileView.Invalidate(TRUE);
	FillFileView();
	
}

void CFileView::OnDummyCompile()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CFileView::OnEditCut()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CFileView::OnEditCopy()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CFileView::OnEditClear()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ��� */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("��Ʈ���� �ε��� �� �����ϴ�. %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;

	if (point != CPoint(-1, -1))
	{
		// Ŭ���� �׸��� �����մϴ�.
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		csTVDataFileName = pWndTree->GetItemText(hTreeItem);
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}