#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "SWLogDebuggingTool(W).h"

#define MY_MULTI_SERVER "234.56.78.9"
#define MY_TCP_PORT 18840
#define MY_UDP_PORT 18830

UDPCommunication mUDPCommunication;
TCPCommunication mTCPCommunication;
CViewTree m_wndClassView;
int iUdpMultiSock, iUdpUniSock, iUdpSndSock;
int iTCPSocket;

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CClassView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// ����/�Ҹ�
//////////////////////////////////////////////////////////////////////

CClassView::CClassView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CClassView::~CClassView()
{
}

BEGIN_MESSAGE_MAP(CClassView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
	ON_COMMAND(ID_Log_Req, &CClassView::OnLogReq)
	ON_COMMAND(ID_Info_Req, &CClassView::OnInfoReq)
	ON_COMMAND(ID_Info_Load, &CClassView::OnInfoLoad)
	ON_COMMAND(ID_AGENT_RscReq, &CClassView::OnAgentRscreq)
	//ON_NOTIFY(TVN_SELCHANGED, IDC_MY_TREE_VIEW, &OnAgentRcsoReq_OnClick)
	ON_NOTIFY(NM_CLICK, IDC_MY_TREE_VIEW, &OnAgentRcsoReq_OnClick)
	ON_MESSAGE(WM_TREEVIEW_REFRESH_EVENT, Treeview_Refresh)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView �޽��� ó����

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//��� ��� �ʱ�ȭ - ���� ���� �� ��Ƽĳ��Ʈ �׷��ּ� ���� ��
	mUDPCommunication.WSAInit();
	mUDPCommunication.InitSocket_Wt(iUdpMultiSock, iUdpUniSock, iUdpSndSock);
	
	mXMLManager.initXML();

	int iRcvSockBufSize = 1048576*2;
	setsockopt(iUdpMultiSock, SOL_SOCKET, SO_RCVBUF, (const char*) &iRcvSockBufSize, sizeof(iRcvSockBufSize));

	//������������ �����ϱ� ���� ���� ����
	sAgentXMLDir = mUserConfig.GetExeDirectory()+"AgtInfo\\";
	mFolderManager.MakeDirectory(&sAgentXMLDir[0u], "Blank"/*���ڰ� �־�� ����.. ������ �� �Է�*/);
	
	/////////////////////////////////////////////////////////////////////////
	
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �並 ����ϴ�.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_EDITLABELS;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, IDC_MY_TREE_VIEW))
	{
		TRACE0("Ŭ���� �並 ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	// �̹����� �ε��մϴ�.
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* ��� */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// ��� ����� �θ� �������� �ƴ� �� ��Ʈ���� ���� ����õ˴ϴ�.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// ���� Ʈ�� �� �����͸� ä��ϴ�.
	FillClassView();
	m_wndClassView.Invalidate();
	m_wndClassView.UpdateWindow();
	
	return 0;
}

void CClassView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

list<string> CClassView::OpenXML(string sXMLDir)
{
	list<string> lXMLStorage;

	do
	{
		//���� ��θ� ����
		//hFind = FindFirstFile((mUserConfig.GetExeDirectory()+"AgtInfo\\*").c_str(), &FindData);
		
		hFind = FindFirstFile(sXMLDir.c_str(), &FindData);
		if(hFind==INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("������ Agent������ �����ϴ�.");
			break;
		}
	}while(hFind==INVALID_HANDLE_VALUE);

	int iDeletPoint = 0;
	do{
		if(iDeletPoint < 2)
		{
			iDeletPoint++;
		}
		else
		{
			lXMLStorage.push_back(mXMLManager.Parsing_Target_XML(mUserConfig.GetExeDirectory()+"AgtInfo\\" + FindData.cFileName, "AgentInfo", "AgentIP"));
			lXMLStorage.push_back(mXMLManager.Parsing_Target_XML(mUserConfig.GetExeDirectory()+"AgtInfo\\" + FindData.cFileName, "AgentInfo", "AgentName"));
			lXMLStorage.push_back(mXMLManager.Parsing_Target_XML(mUserConfig.GetExeDirectory()+"AgtInfo\\" + FindData.cFileName, "AgentInfo", "AgentLogFileList"));
		}
	}while(FindNextFile(hFind, &FindData));

	return lXMLStorage;
}
/*
void void CClassView::UpdateClassView()
{
	m_wndClassView.UpdateData(TRUE);

}
*/
//Ŭ�����信 Agent Info�� ���
void CClassView::FillClassView()
{
	string sFileDIrChk ="";
	list<string> AgentXMLList;
	XMLManager m_XMLManager;	

	hRoot = m_wndClassView.InsertItem(_T("��� ���"), 0, 0);
	m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_wndClassView.Expand(hRoot, TVE_EXPAND);

	//lAgtXMLStorage = OpenXML(mUserConfig.GetExeDirectory()+"AgtInfo\\*");
	
	//////////OpenXML����

	do
	{
		//���� ��θ� ����
		hFind = FindFirstFile((mUserConfig.GetExeDirectory()+"AgtInfo\\*").c_str(), &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("���ϰ�� ����");
			break;
		}
	}while(hFind==INVALID_HANDLE_VALUE);

	int iDeletPoint = 0;

	do{
		if(iDeletPoint < 2)
		{
			iDeletPoint++;
		}
		else
		{
			if(FindData.cFileName != NULL)
			{
				sFileDIrChk = mUserConfig.GetExeDirectory()+"AgtInfo\\" + FindData.cFileName;
				AgentXMLList.push_back(m_XMLManager.Parsing_Target_XML(sFileDIrChk, "AgentInfo", "AgentIP"));
				AgentXMLList.push_back(m_XMLManager.Parsing_Target_XML(sFileDIrChk, "AgentInfo", "AgentName"));
				AgentXMLList.push_back(m_XMLManager.Parsing_Target_XML(sFileDIrChk, "AgentInfo", "AgentLogFileDirectory"));
				AgentXMLList.push_back(m_XMLManager.Parsing_Target_XML(sFileDIrChk, "AgentInfo", "AgentLogFileList"));
			}
		}
	}while(FindNextFile(hFind, &FindData));

	/////////////OpenXML ��

	if(AgentXMLList.size()!=0)
	{
		string sListElement = "";

		list<string>::iterator iterStartList = AgentXMLList.begin();
		list<string>::iterator iterEndList = AgentXMLList.end();

		while(iterStartList != iterEndList)
		{
			sListElement = *iterStartList + "/";
			iterStartList++;
			sListElement += *iterStartList;
			hClass = m_wndClassView.InsertItem(_T(sListElement.c_str()), 1, 1, hRoot);
			
			iterStartList++;

			//C:\Temp\CoreDebug
	//<<<<<<< HEAD
			int index = 0;
			CString str = (*iterStartList).c_str();
			index = m_TreeviewManager.GetCharNumber(str, '\\') - 2;

			CString temp;
			AfxExtractSubString(temp, str, index + 2, '\\');
			hSrc = m_wndClassView.InsertItem(temp, 0, 0, hClass);
			
	//=======
	// 		int index = 0;
	// 		CString str = (*iterStartList).c_str();
	// 		index = m_TreeviewManager.GetCharNumber(str, '\\') - 2;
	// 
	// 		CString temp;
	// 		AfxExtractSubString(temp, str, index + 2, '\\');
	// 		hSrc = m_wndClassView.InsertItem(temp, 0, 0, hClass);
	// 		
	// 		iterStartList++;
	//>>>>>>> 93c9376e81d76ada5e39c94b2b88927f98f46478
	 		
			iterStartList++;

			sListElement = *iterStartList;
			sListElement = sListElement.substr(0, sListElement.length());
			std::stringstream strmFileList(sListElement);
			std::string sFileList;

			while(strmFileList.good())
			{
				getline(strmFileList, sFileList,'\n');
				m_wndClassView.InsertItem(_T(sFileList.c_str()), 1, 1, hSrc);
			}
			iterStartList++;
		}
	}

}

void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
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
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CClassView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndClassView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CClassView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CClassView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CClassView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CClassView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("��� �Լ� �߰�..."));
}

void CClassView::OnClassAddMemberVariable()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CClassView::OnClassDefinition()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CClassView::OnClassProperties()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CClassView::OnNewFolder()
{
	AfxMessageBox(_T("�� ����..."));
}

void CClassView::OnPaint()
{
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CClassView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void CClassView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

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

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* ��� */);
}

void CClassView::OnLogReq()
{
	// TODO: Add your command handler code here
	//Thread Log Req

	CWinThread *LogReqThread = NULL;
	LogReqThread = AfxBeginThread(Thread_Log_Req,this);

	//Non-Thread Log Req
	/*
	HTREEITEM hItem = m_wndClassView.GetSelectedItem();
	string sLogFileName = "";
	string sLogDir = "";
	string sAgentIPwithName = "";
	string sAgentIP = "";
	stringstream sDate;
	BOOL bSuccessFlag = FALSE;
	int iIndex = 0;

	if(hItem != NULL)
	{
		sLogFileName = m_wndClassView.GetItemText(hItem);

		hItem = m_wndClassView.GetParentItem(hItem);
		hItem = m_wndClassView.GetParentItem(hItem);
		sAgentIPwithName = m_wndClassView.GetItemText(hItem);

		iIndex = sAgentIPwithName.find_first_of("/");
		sAgentIP = sAgentIPwithName.substr(0, iIndex);
	}
	char* pcAgtIP = &sAgentIP[0u];
	mTCPCommunication.TCPSockInit(iTCPSocket);
	if(mTCPCommunication.TryCnct(iTCPSocket, pcAgtIP, MY_TCP_PORT) == TRUE)
	{
		time_t tTimer;
		struct tm tTimer_St;
		tTimer = time(NULL);
		localtime_s(&tTimer_St, &tTimer);
		int iToday = (tTimer_St.tm_year + 1900) * 10000 + (tTimer_St.tm_mon + 1) * 100 + (tTimer_St.tm_mday);	
		sDate << iToday;

		sLogDir = mXMLManager.Parsing_Target_XML(mUserConfig.GetExeDirectory() + "Config.xml", "CommonPath", "Watcher");
		sLogDir += "\\" + sDate.str() + "\\" + sAgentIP + "\\";

		mTCPCommunication.LogFileReq(iTCPSocket, sLogDir, sLogFileName);
	}
	else
	{

	}
	*/
}

void CClassView::OnInfoReq()
{
	// TODO: Add your command handler code here
	//SocketBinding(iUdpUniSock, AddrStruct, AF_INET, INADDR_ANY, 1883);

	//Info Req
	mUDPCommunication.InforReq(iUdpMultiSock, MY_UDP_PORT, MY_MULTI_SERVER);
	//mUDPCommunication.InforReq(iUdpMultiSock, MY_UDP_PORT, "192.168.0.12");

	//Non-Thread InfoRcv
	/*memset(&AddrStruct, 0, sizeof(AddrStruct));
	AddrStruct.sin_family = AF_INET;
	AddrStruct.sin_addr.s_addr = htonl(INADDR_ANY);
	AddrStruct.sin_port = htons(MY_UDP_PORT);
	bind(iUdpUniSock, (SOCKADDR*)&AddrStruct, sizeof(AddrStruct));

	mUDPCommunication.RcvInfor(iUdpUniSock, 4);
	RefreshClassView();
	*/

	
	//Thread InfoRcv
	CWinThread *InfoRcvThread = NULL;
	InfoRcvThread = AfxBeginThread(Thread_Info_Rcv,this);

	/*Agent Info ����Ȯ�� �ڵ�*/
	//DisplayAllElement_List(lAgtInfoList);
}

void CClassView::RefreshClassView()
{
	m_wndClassView.DeleteAllItems();
	FillClassView();
	m_wndClassView.Invalidate();
	m_wndClassView.UpdateWindow();
}

void CClassView::OnInfoLoad()
{
	// TODO: Add your command handler code here
	RefreshClassView();
}

void CClassView::OnAgentRcsoReq_OnClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	//Thread
	CWinThread *RcsReqThread = NULL;
	RcsReqThread = AfxBeginThread(Thread_RcsReq_Click, this);

	//Non-Thread
	/*
	TCPCommunication mTCPCommunication;
	int iTCPSocket = 0;
	
	string sItem = "";
	char cHDDUSage[4096];
	float fCPUUsage;
	DWORD dwRAMUsage;

	TV_HITTESTINFO hit_info;

	// ȭ��󿡼� ���콺�� ��ġ�� ��´�.
	::GetCursorPos(&hit_info.pt);

	// ���� ���콺 ��ǥ�� Ʈ����Ʈ�� ������ ��ǥ�� �����Ѵ�.
	::ScreenToClient(m_wndClassView.m_hWnd, &hit_info.pt);

	// ���� ���콺 ��ǥ�� ��ġ�� �׸� ������ ��´�.
	HTREEITEM current_item = m_wndClassView.HitTest(&hit_info);
	HTREEITEM Current_PItem;
	HTREEITEM ChildITem;
	HTREEITEM ListITem;
	list<string> LogList;

	int iIndex = 0;
	string sAgentIP = "";
	string sPItem = "";
	BOOL bCnctFlag;
	if(current_item != NULL)
	{
		// ���콺�� ��ġ�� �׸��� ã�Ҵٸ� �ش� �׸��� �����Ѵ�.
		m_wndClassView.Select(current_item, TVGN_CARET);
		Current_PItem = m_wndClassView.GetParentItem(current_item);
		sPItem = m_wndClassView.GetItemText(Current_PItem);
		sItem = m_wndClassView.GetItemText(current_item);

		if(sPItem == "��� ���")
		{
			iIndex = sItem.find_first_of("/");
			sAgentIP = sItem.substr(0, iIndex);
			char* pcAgtIP = &sAgentIP[0u];

			mTCPCommunication.TCPSockInit(iTCPSocket);
			bCnctFlag = mTCPCommunication.TryCnct(iTCPSocket, pcAgtIP, MY_TCP_PORT);
			if(bCnctFlag == TRUE)
			{
				memcpy(&cHDDUSage, mTCPCommunication.ReqRsc(iTCPSocket, fCPUUsage, dwRAMUsage), 4096);
			}
			
			mTCPCommunication.TCPSockInit(iTCPSocket);			
			bCnctFlag = mTCPCommunication.TryCnct(iTCPSocket, pcAgtIP, MY_TCP_PORT);
			if(bCnctFlag == TRUE)
			{
				mTCPCommunication.LogListReq(iTCPSocket, pcAgtIP);
			}
		}
	}
	*/
}

void CClassView::OnAgentRscreq()
{
	// TODO: Add your command handler code here
	mUDPCommunication.ResourceReq(iUdpMultiSock, MY_UDP_PORT, MY_MULTI_SERVER);

	memset(&AddrStruct, 0, sizeof(AddrStruct));
	AddrStruct.sin_family = AF_INET;
	AddrStruct.sin_addr.s_addr = htonl(INADDR_ANY);
	AddrStruct.sin_port = htons(MY_UDP_PORT);
	bind(iUdpUniSock, (SOCKADDR*)&AddrStruct, sizeof(AddrStruct));

	mUDPCommunication.RcvRsc(iUdpUniSock, 4);
}

void CClassView::SocketBinding(int& iSocket, SOCKADDR_IN mSocketAddr, int iAddrFamily, long lSourceIP, int iSourcePort)
{
	memset(&mSocketAddr, 0, sizeof(mSocketAddr));
	mSocketAddr.sin_family = iAddrFamily;
	mSocketAddr.sin_addr.s_addr = htonl(lSourceIP);
	mSocketAddr.sin_port = htons(iSourcePort);
	bind(iSocket, (SOCKADDR*)&mSocketAddr, sizeof(mSocketAddr));
}

void CClassView::DisplayAllElement_List(list<string> lList)
{
	string sListElement = "";

	list<string>::iterator iterStartList = lList.begin();
	list<string>::iterator iterEndList = lList.end();

	while(iterStartList != iterEndList)
	{
		sListElement = *iterStartList;
		AfxMessageBox(sListElement.c_str());
		iterStartList++;
	}
}

//TCP File Rcv using Thread
UINT CClassView::Thread_Log_Req(LPVOID pParam)
{
	TCPCommunication mTCPCommunication;
	XMLManager mXMLManager;
	UserConfig mUserConfig;
	int iTCPSocket = 0;

	CClassView *Thread_View = (CClassView*)pParam;
	
	// TODO: Add your command handler code here
	HTREEITEM hItem = m_wndClassView.GetSelectedItem();
	HTREEITEM hChild;
	HTREEITEM hGrandChild;
	HTREEITEM hParents;
	HTREEITEM hGrandParents;

	string sLogFileName = "";
	string sLogDir = "";
	string sAgentIPwithName = "";
	string sAgentIP = "";
	stringstream sDate;
	BOOL bSuccessFlag = FALSE;
	int iIndex = 0;
	int iNumofLog = 0;
	int iTotalNumofLog = 0;

	if(hItem != NULL)
	{
		sLogFileName = m_wndClassView.GetItemText(hItem);

		hParents = m_wndClassView.GetParentItem(hItem);
		hGrandParents = m_wndClassView.GetParentItem(hParents);
		sAgentIPwithName = m_wndClassView.GetItemText(hGrandParents);
		iIndex = sAgentIPwithName.find_first_of("/");
		sAgentIP = sAgentIPwithName.substr(0, iIndex);
	}
	//index != -1, ������ ���� �α� ������ ��� �ٷ� ���� ��û
	//Get �θ��� �ι����� IP�� ��ġ�̸� ȹ��
	if(iIndex != -1)
	{
		char* pcAgtIP = &sAgentIP[0u];
		mTCPCommunication.TCPSockInit(iTCPSocket);
		if(mTCPCommunication.TryCnct(iTCPSocket, pcAgtIP, MY_TCP_PORT) == TRUE)
		{
			time_t tTimer;
			struct tm tTimer_St;
			tTimer = time(NULL);
			localtime_s(&tTimer_St, &tTimer);
			int iToday = (tTimer_St.tm_year + 1900) * 10000 + (tTimer_St.tm_mon + 1) * 100 + (tTimer_St.tm_mday);	
			sDate << iToday;

			sLogDir = mXMLManager.Parsing_Target_XML(mUserConfig.GetExeDirectory() + "Config.xml", "CommonPath", "Watcher");
			sLogDir += "\\" + sDate.str() + "\\" + sAgentIP + "\\";

			mTCPCommunication.LogFileReq(iTCPSocket, sLogDir, sLogFileName);
		}
		else
		{

		}
	}
	//index == -1 �� ���
	//������ ���� IP�� ��ġ�̸��� ���, �ش� ��ġ�� �����ϴ� ��� �α� ������ ��û��
	else
	{
		sAgentIPwithName = m_wndClassView.GetItemText(hParents);
		
		if(sAgentIPwithName == "��� ���")
		{
			sAgentIPwithName = m_wndClassView.GetItemText(hItem);
			iIndex = sAgentIPwithName.find_first_of("/");
			sAgentIP = sAgentIPwithName.substr(0, iIndex);

			char* pcAgtIP = &sAgentIP[0u];
			mTCPCommunication.TCPSockInit(iTCPSocket);
			//��忡 �����ϴ� ��� ������ ������ ����
			hChild = m_wndClassView.GetChildItem(hItem);
			hGrandChild = m_wndClassView.GetChildItem(hChild);
			while(hGrandChild = m_wndClassView.GetNextSiblingItem(hGrandChild))
			{
				iTotalNumofLog++;
			}
			////
			
			hGrandChild = m_wndClassView.GetChildItem(hChild);

			time_t tTimer;
			struct tm tTimer_St;
			tTimer = time(NULL);
			localtime_s(&tTimer_St, &tTimer);
			int iToday = (tTimer_St.tm_year + 1900) * 10000 + (tTimer_St.tm_mon + 1) * 100 + (tTimer_St.tm_mday);	
			sDate << iToday;

			sLogDir = mXMLManager.Parsing_Target_XML(mUserConfig.GetExeDirectory() + "Config.xml", "CommonPath", "Watcher");
			sLogDir += "\\" + sDate.str() + "\\" + sAgentIP + "\\";

			for(iNumofLog=0;iNumofLog <= iTotalNumofLog;iNumofLog++)
			{

				mTCPCommunication.TCPSockInit(iTCPSocket);
				if(mTCPCommunication.TryCnct(iTCPSocket, pcAgtIP, MY_TCP_PORT) == TRUE)
				{
					Sleep(200);
					sLogFileName = m_wndClassView.GetItemText(hGrandChild);
					mTCPCommunication.LogFileReq(iTCPSocket, sLogDir, sLogFileName);
				}
				else
				{

				}
				hGrandChild = m_wndClassView.GetNextItem(hGrandChild, TVGN_NEXT);
			}

		}
	}
	return 0;
}

UINT CClassView::Thread_Info_Rcv(LPVOID pParam)
{
	CClassView *Thread_LogRvcv = (CClassView*)pParam;
	UDPCommunication mUDPCommunication;
	int iUdpUniSock = 0;
	SOCKADDR_IN AddrStruct;

	memset(&AddrStruct, 0, sizeof(AddrStruct));
	AddrStruct.sin_family = AF_INET;
	AddrStruct.sin_addr.s_addr = htonl(INADDR_ANY);
	AddrStruct.sin_port = htons(MY_UDP_PORT);
	bind(iUdpUniSock, (SOCKADDR*)&AddrStruct, sizeof(AddrStruct));

	mUDPCommunication.RcvInfor(iUdpUniSock, 4);

	return 0;
}

UINT CClassView::Thread_RcsReq_Click(LPVOID pParam)
{
	TCPCommunication mTCPCommunication;
	int iTCPSocket = 0;

	string sItem = "";
	char cHDDUSage[4096];
	float fCPUUsage;
	DWORD dwRAMUsage;

	TV_HITTESTINFO hit_info;

	// ȭ��󿡼� ���콺�� ��ġ�� ��´�.
	::GetCursorPos(&hit_info.pt);

	// ���� ���콺 ��ǥ�� Ʈ����Ʈ�� ������ ��ǥ�� �����Ѵ�.
	::ScreenToClient(m_wndClassView.m_hWnd, &hit_info.pt);

	// ���� ���콺 ��ǥ�� ��ġ�� �׸� ������ ��´�.
	HTREEITEM current_item = m_wndClassView.HitTest(&hit_info);
	HTREEITEM Current_PItem;
	HTREEITEM ChildITem;
	HTREEITEM ListITem;
	list<string> LogList;

	int iIndex = 0;
	string sAgentIP = "";
	string sPItem = "";
	BOOL bCnctFlag;
	if(current_item != NULL)
	{
		// ���콺�� ��ġ�� �׸��� ã�Ҵٸ� �ش� �׸��� �����Ѵ�.
		m_wndClassView.Select(current_item, TVGN_CARET);
		Current_PItem = m_wndClassView.GetParentItem(current_item);
		sPItem = m_wndClassView.GetItemText(Current_PItem);
		sItem = m_wndClassView.GetItemText(current_item);

		if(sPItem == "��� ���")
		{
			iIndex = sItem.find_first_of("/");
			sAgentIP = sItem.substr(0, iIndex);
			char* pcAgtIP = &sAgentIP[0u];

			mTCPCommunication.TCPSockInit(iTCPSocket);
			bCnctFlag = mTCPCommunication.TryCnct(iTCPSocket, pcAgtIP, MY_TCP_PORT);
			if(bCnctFlag == TRUE)
			{
				memcpy(&cHDDUSage, mTCPCommunication.ReqRsc(iTCPSocket, fCPUUsage, dwRAMUsage), 4096);
			}

			mTCPCommunication.TCPSockInit(iTCPSocket);			
			bCnctFlag = mTCPCommunication.TryCnct(iTCPSocket, pcAgtIP, MY_TCP_PORT);
			if(bCnctFlag == TRUE)
			{
				mTCPCommunication.LogListReq(iTCPSocket, pcAgtIP);
			}
			
			Sleep(5);
			
			CClassView *pcThis = (CClassView*)pParam;
			::PostMessage(pcThis->GetSafeHwnd(), WM_TREEVIEW_REFRESH_EVENT, (WPARAM)current_item, 0);
			//::SendMessage(pcThis->GetSafeHwnd(), WM_TREEVIEW_REFRESH_EVENT, (WPARAM)current_item, 0);
			//AfxGetMainWnd()->SendMessage(WM_TREEVIEW_REFRESH_EVENT, 0, 0);
		}
	}
	return 0;
}

HRESULT CClassView::Treeview_Refresh(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hSelectItem = (HTREEITEM)wParam;
	HTREEITEM hChildItem = m_wndClassView.GetChildItem(hSelectItem);
	HTREEITEM hGrandChildItem = m_wndClassView.GetChildItem(hChildItem);
	list<string> AgentFileList;
	string sFileDIrChk;
	string sFileLIst = "";
	XMLManager m_XMLManager;

	//���� ���� ����Ʈ ����

	sFileDIrChk = m_wndClassView.GetItemText(hChildItem);
	m_wndClassView.DeleteItem(hChildItem);
	m_wndClassView.InsertItem(_T(sFileDIrChk.c_str()), 0, 0, hSelectItem);
	hChildItem = m_wndClassView.GetChildItem(hSelectItem);
	do
	{
		//���� ��θ� ����
		hFind = FindFirstFile((mUserConfig.GetExeDirectory()+"AgtInfo\\*").c_str(), &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("���ϰ�� ����");
			break;
		}
	}while(hFind==INVALID_HANDLE_VALUE);

	int iDeletPoint = 0;

	do{
		if(iDeletPoint < 2)
		{
			iDeletPoint++;
		}
		else
		{
			if(FindData.cFileName != NULL)
			{
				sFileDIrChk = mUserConfig.GetExeDirectory()+"AgtInfo\\" + FindData.cFileName;
				sFileLIst = m_XMLManager.Parsing_Target_XML(sFileDIrChk, "AgentInfo", "AgentLogFileList");
				//AgentFileList.push_back(m_XMLManager.Parsing_Target_XML(sFileDIrChk, "AgentInfo", "AgentLogFileList"));
			}
		}
	}while(FindNextFile(hFind, &FindData));

	/////////////OpenXML ��
	sFileLIst = sFileLIst.substr(0, strlen(sFileLIst.c_str())-1);
	std::stringstream strmFileList(sFileLIst);
	std::string sFileList = "";

	while(strmFileList.good())
	{
		getline(strmFileList, sFileList,'\n');
		m_wndClassView.InsertItem(_T(sFileList.c_str()), 1, 1, hChildItem);
	}

	m_wndClassView.Invalidate();
	m_wndClassView.UpdateWindow();

	return TRUE;
}



/*
void CClassView::OnLButtonDown(UINT nFlags, CPoint point) {
	m_nClickFlags = nFlags;
	HTREEITEM iClk = GetClickedItem();
	if (iClk) {
		if (m_fMultiselectable) {
			if (m_nClickFlags & MK_CONTROL) {
				// we change selection
				ChgSelection(iClk);
				return;
			}
		}
		// we set as selection
		m_wndClassView.SetItemState(iClk, TVIS_SELECTED , TVIS_SELECTED);
	} else CTreeCtrl::OnLButtonDown(nFlags, point);
}

HTREEITEM CClassView::GetClickedItem() {
	DWORD dwPos = GetMessagePos();
	CPoint spt( LOWORD(dwPos), HIWORD(dwPos) );
	ScreenToClient(&spt);
	m_pt = spt;
	UINT test;
	HTREEITEM hti = HitTest(spt, &test);
	if ((hti != 0) && (test & (TVHT_ONITEM | TVHT_ONITEMRIGHT))) return
		hti;
	if (hti == 0) {
		ClearSelection();
	}
	return 0;
}

void CClassView::ChgSelection(HTREEITEM iClk) {
	if GetItemState(iClk, TVIS_SELECTED) == TVIS_SELECTED) {
		SetItemState(iClk, 0 , TVIS_SELECTED);
	} else {
		SetItemState(iClk, TVIS_SELECTED , TVIS_SELECTED);
	}
}
*/
/*
void CClassView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Set focus to control if key strokes are needed.
	// Focus is not automatically given to control on lbuttondown

	m_dwDragStart = GetTickCount();

	if(nFlags & MK_CONTROL ) 
	{
		// Control key is down
		UINT flag;
		HTREEITEM hItem = m_wndClassView.HitTest( point, &flag );
		if( hItem )
		{
			// Toggle selection state
			UINT uNewSelState = 
				m_wndClassView.GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED ? 
				0 : TVIS_SELECTED;

			// Get old selected (focus) item and state
			HTREEITEM hItemOld = m_wndClassView.GetSelectedItem();
			UINT uOldSelState  = hItemOld ? 
				m_wndClassView.GetItemState(hItemOld, TVIS_SELECTED) : 0;

			// Select new item
			if( m_wndClassView.GetSelectedItem() == hItem )
				m_wndClassView.SelectItem( NULL );		// to prevent edit
			CTreeCtrl::OnLButtonDown(nFlags, point);

			// Set proper selection (highlight) state for new item
			m_wndClassView.SetItemState(hItem, uNewSelState,  TVIS_SELECTED);

			// Restore state of old selected item
			if (hItemOld && hItemOld != hItem)
				m_wndClassView.SetItemState(hItemOld, uOldSelState, TVIS_SELECTED);

			m_hItemFirstSel = NULL;

			return;
		}
	} 
	else if(nFlags & MK_SHIFT)
	{
		// Shift key is down
		UINT flag;
		HTREEITEM hItem = HitTest( point, &flag );

		// Initialize the reference item if this is the first shift selection
		if( !m_hItemFirstSel )
			m_hItemFirstSel = m_wndClassView.GetSelectedItem();

		// Select new item
		if( m_wndClassView.GetSelectedItem() == hItem )
			m_wndClassView.SelectItem( NULL );			// to prevent edit
		CTreeCtrl::OnLButtonDown(nFlags, point);

		if( m_hItemFirstSel )
		{
			SelectItems( m_hItemFirstSel, hItem );
			return;
		}
	}
	else
	{
		// Normal - remove all selection and let default 
		// handler do the rest
		ClearSelection();
		m_hItemFirstSel = NULL;
	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CClassView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ( (nChar==VK_UP || nChar==VK_DOWN) && GetKeyState( VK_SHIFT )&0x8000)
	{
		// Initialize the reference item if this is the first shift selection
		if( !m_hItemFirstSel )
		{
			m_hItemFirstSel = m_wndClassView.GetSelectedItem();
			ClearSelection();
		}

		// Find which item is currently selected
		HTREEITEM hItemPrevSel = m_wndClassView.GetSelectedItem();

		HTREEITEM hItemNext;
		if ( nChar==VK_UP )
			hItemNext = m_wndClassView.GetPrevVisibleItem( hItemPrevSel );
		else
			hItemNext = m_wndClassView.GetNextVisibleItem( hItemPrevSel );

		if ( hItemNext )
		{
			// Determine if we need to reselect previously selected item
			BOOL bReselect = 
				!( m_wndClassView.GetItemState( hItemNext, TVIS_SELECTED ) & TVIS_SELECTED );

			// Select the next item - this will also deselect the previous item
			m_wndClassView.SelectItem( hItemNext );

			// Reselect the previously selected item
			if ( bReselect )
				m_wndClassView.SetItemState( hItemPrevSel, TVIS_SELECTED, TVIS_SELECTED );
		}
		return;
	}
	else if( nChar >= VK_SPACE )
	{
		m_hItemFirstSel = NULL;
		ClearSelection();
	}
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CClassView::ClearSelection()
{
	// This can be time consuming for very large trees 
	// and is called every time the user does a normal selection
	// If performance is an issue, it may be better to maintain 
	// a list of selected items
	for ( HTREEITEM hItem = m_wndClassView.GetRootItem(); hItem!=NULL; hItem= m_wndClassView.GetNextItem( hItem ) )
		if ( m_wndClassView.GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
			m_wndClassView.SetItemState( hItem, 0, TVIS_SELECTED );
}

// SelectItems	- Selects items from hItemFrom to hItemTo. Does not
//		- select child item if parent is collapsed. Removes
//		- selection from all other items
// hItemFrom	- item to start selecting from
// hItemTo	- item to end selection at.
BOOL CClassView::SelectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo)
{
	HTREEITEM hItem = m_wndClassView.GetRootItem();

	// Clear selection upto the first item
	while ( hItem && hItem!=hItemFrom && hItem!=hItemTo )
	{
		hItem = m_wndClassView.GetNextVisibleItem( hItem );
		m_wndClassView.SetItemState( hItem, 0, TVIS_SELECTED );
	}

	if ( !hItem )
		return FALSE;	// Item is not visible

	m_wndClassView.SelectItem( hItemTo );

	// Rearrange hItemFrom and hItemTo so that hItemFirst is at top
	if( hItem == hItemTo )
	{
		hItemTo = hItemFrom;
		hItemFrom = hItem;
	}


	// Go through remaining visible items
	BOOL bSelect = TRUE;
	while ( hItem )
	{
		// Select or remove selection depending on whether item
		// is still within the range.
		m_wndClassView.SetItemState( hItem, bSelect ? TVIS_SELECTED : 0, TVIS_SELECTED );

		// Do we need to start removing items from selection
		if( hItem == hItemTo ) 
			bSelect = FALSE;

		hItem = m_wndClassView.GetNextVisibleItem( hItem );
	}

	return TRUE;
}

HTREEITEM CClassView::GetFirstSelectedItem()
{
	for ( HTREEITEM hItem = m_wndClassView.GetRootItem(); hItem!=NULL; hItem = m_wndClassView.GetNextItem( hItem ) )
		if ( m_wndClassView.GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
			return hItem;

	return NULL;
}

HTREEITEM CClassView::GetNextSelectedItem( HTREEITEM hItem )
{
	for ( hItem = m_wndClassView.GetNextItem( hItem ); hItem!=NULL; hItem = m_wndClassView.GetNextItem( hItem ) )
		if ( m_wndClassView.GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
			return hItem;

	return NULL;
}

	HTREEITEM CClassView::GetPrevSelectedItem( HTREEITEM hItem )
	{
		for ( hItem = GetPrevItem( hItem ); hItem!=NULL; hItem = GetPrevItem( hItem ) )
			if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
				return hItem;

		return NULL;
	}
*/


/*
void CClassView::OnMultiSelect() 
{
	UpdateData();
	m_wndClassView.EnableMultiSelect(m_bMultiSelect != FALSE);
	if(!m_bMultiSelect) {
		HTREEITEM hItem = m_wndClassView.GetSelectedItem();
		m_wndClassView.ClearSelection(hItem);
	}
}
*/