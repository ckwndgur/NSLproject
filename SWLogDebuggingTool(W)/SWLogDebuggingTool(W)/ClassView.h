
#pragma once
#include <string>
#include <sstream>
#include "ViewTree.h"
#include "UserConfig.h"
#include "XMLManager.h"
#include "AgtInfoList.h"
#include "UDPCommunication.h"
#include "afxcmn.h"
#include "FileView.h"
#include "TreeviewManager.h"
#include "TreeviewData.h"

class CClassToolBar : public CMFCToolBar
{
	
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}
	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CClassView : public CDockablePane
{
public:
	CClassView();
	virtual ~CClassView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	
	//UDPCommunication mUDPCommunication;
	SOCKADDR_IN AddrStruct;
	//int iUdpMultiSock, iUdpUniSock, iUdpSndSock;
	AgtInfoList mAgtInfoList;

	XMLManager mXMLManager;
	FolderManager mFolderManager;
	string sAgentXMLDir;
	
	WIN32_FIND_DATA FindData;
	HANDLE hFind, hFile;
	UserConfig mUserConfig;
	list<string> lAgtInfoList;

	//COutputWnd mCOutputWnd;

	HTREEITEM hRoot;
	HTREEITEM hSrc;
	HTREEITEM hInc;
	HTREEITEM hClass;

	void SocketBinding(int& iSocket, SOCKADDR_IN mSocketAddr, int iAddrFamily, long lSourceIP, int iSourcePort);
	void DisplayAllElement_List(list<string> lList);
	void RefreshClassView();
	list<string> OpenXML(string sXMLDir);

//protected:
	CClassToolBar m_wndToolBar;
	CFileView mCFileView;
	//CViewTree m_wndClassView;
	CImageList m_ClassViewImages;
	UINT m_nCurrSort;

	void FillClassView();

private:
	TreeviewManager m_TreeviewManager;


//재정의입니다.
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLogReq();
	afx_msg void OnInfoReq();
	afx_msg void OnInfoLoad();
};

