#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "SWLogDebuggingTool(W).h"

CViewTree m_wndClassView;
UDPCommunication mUDPCommunication;
int iUdpMultiSock, iUdpUniSock, iUdpSndSock;

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
// 생성/소멸
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
	ON_COMMAND(ID_Log_Req, &OnLogReq)
	ON_COMMAND(ID_Info_Req, &OnInfoReq)
	ON_COMMAND(ID_Info_Load, &OnInfoLoad)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 메시지 처리기

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//통신 기능 초기화 - 소켓 생성 및 멀티캐스트 그룹주소 설정 등
	mUDPCommunication.WSAInit();
	mUDPCommunication.InitSocket_Wt(iUdpMultiSock, iUdpUniSock, iUdpSndSock);
	mXMLManager.initXML();

	int iRcvSockBufSize = 1048576*2;
	setsockopt(iUdpMultiSock, SOL_SOCKET, SO_RCVBUF, (const char*) &iRcvSockBufSize, sizeof(iRcvSockBufSize));

	//원격지정보를 저장하기 위한 폴더 생성
	sAgentXMLDir = mUserConfig.GetExeDirectory()+"AgtInfo\\";
	mFolderManager.MakeDirectory(&sAgentXMLDir[0u], "Blank"/*인자값 있어야 동작.. 임의의 값 입력*/);
	
	/////////////////////////////////////////////////////////////////////////
	
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 뷰를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_EDITLABELS;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("클래스 뷰를 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// 이미지를 로드합니다.
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 잠금 */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
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

	// 정적 트리 뷰 데이터를 채웁니다.
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
		//파일 경로를 추출
		//hFind = FindFirstFile((mUserConfig.GetExeDirectory()+"AgtInfo\\*").c_str(), &FindData);
		
		hFind = FindFirstFile(sXMLDir.c_str(), &FindData);
		if(hFind==INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("수집한 Agent정보가 없습니다.");
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
//클래스뷰에 Agent Info를 출력
void CClassView::FillClassView()
{
	
	string sFileDIrChk ="";
	list<string> AgentXMLList;
	XMLManager m_XMLManager;	

	hRoot = m_wndClassView.InsertItem(_T("노드 목록"), 0, 0);
	m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_wndClassView.Expand(hRoot, TVE_EXPAND);

	//lAgtXMLStorage = OpenXML(mUserConfig.GetExeDirectory()+"AgtInfo\\*");
	
	//////////OpenXML시작

	do
	{
		//파일 경로를 추출
		hFind = FindFirstFile((mUserConfig.GetExeDirectory()+"AgtInfo\\*").c_str(), &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("파일경로 에러");
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
			sFileDIrChk = mUserConfig.GetExeDirectory()+"AgtInfo\\" + FindData.cFileName;
			AgentXMLList.push_back(m_XMLManager.Parsing_Target_XML(sFileDIrChk, "AgentInfo", "AgentIP"));
			AgentXMLList.push_back(m_XMLManager.Parsing_Target_XML(sFileDIrChk, "AgentInfo", "AgentName"));
			//AgentXMLList.push_back(m_XMLManager.Parsing_Target_XML(sFileDIrChk, "AgentInfo", "AgentLogFileDirectory"));
			AgentXMLList.push_back(m_XMLManager.Parsing_Target_XML(sFileDIrChk, "AgentInfo", "AgentLogFileList"));
			
		}
	}while(FindNextFile(hFind, &FindData));

	/////////////OpenXML 끝

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

		sListElement = *iterStartList;
		sListElement = sListElement.substr(0, sListElement.length()-1);
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
		// 클릭한 항목을 선택합니다.
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
	AfxMessageBox(_T("멤버 함수 추가..."));
}

void CClassView::OnClassAddMemberVariable()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CClassView::OnClassDefinition()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CClassView::OnClassProperties()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CClassView::OnNewFolder()
{
	AfxMessageBox(_T("새 폴더..."));
}

void CClassView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
		TRACE(_T("비트맵을 로드할 수 없습니다. %x\n"), uiBmpId);
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
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* 잠금 */);
}

void CClassView::OnLogReq()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = m_wndClassView.GetSelectedItem();
	string sLogFileName = "";
	string sAgentIPwithName = "";
	string sAgentIP = "";
	string sLogDir = "";
	stringstream sDate;
	int iIndex = 0;

	memset(&AddrStruct, 0, sizeof(AddrStruct));
	AddrStruct.sin_family = AF_INET;
	AddrStruct.sin_addr.s_addr = htonl(INADDR_ANY);
	AddrStruct.sin_port = htons(1883);
	bind(iUdpUniSock, (SOCKADDR*)&AddrStruct, sizeof(AddrStruct));

	if(hItem != NULL)
	{
		sLogFileName = m_wndClassView.GetItemText(hItem);

		hItem = m_wndClassView.GetParentItem(hItem);
		sAgentIPwithName = m_wndClassView.GetItemText(hItem);

		iIndex = sAgentIPwithName.find_first_of("/");
		sAgentIP = sAgentIPwithName.substr(0, iIndex);
	}

	if(mUDPCommunication.SndDataReq_MFC(iUdpUniSock, sAgentIP, sLogFileName, 1883) != TRUE)
		AfxMessageBox("SndReq Fail");

	time_t tTimer;
	struct tm tTimer_St;
	tTimer = time(NULL);
	localtime_s(&tTimer_St, &tTimer);
	int iToday = (tTimer_St.tm_year + 1900) * 10000 + (tTimer_St.tm_mon + 1) * 100 + (tTimer_St.tm_mday);	
	sDate << iToday;

	sLogDir = mXMLManager.Parsing_Target_XML(mUserConfig.GetExeDirectory() + "Config.xml", "CommonPath", "Watcher");
	sLogDir += "\\" + sDate.str() + "\\" + sAgentIP + "\\";

	mUDPCommunication.LogFileRcv(iUdpUniSock, (char*)sLogDir.c_str(), (char*)sLogFileName.c_str());

	mCFileView.RefreshFileView();

}

void CClassView::RefreshClassView()
{
	m_wndClassView.DeleteAllItems();
	FillClassView();
	m_wndClassView.Invalidate();
	m_wndClassView.UpdateWindow();
}

void CClassView::OnInfoReq()
{
	// TODO: Add your command handler code here
	SocketBinding(iUdpUniSock, AddrStruct, AF_INET, INADDR_ANY, 1883);
	mUDPCommunication.InforReq(iUdpSndSock, 1883, "234.56.78.9");
	mUDPCommunication.RcvInfor(iUdpUniSock, 5);
	RefreshClassView();
	/*Agent Info 수신확인 코드*/
	//DisplayAllElement_List(lAgtInfoList);
}


void CClassView::OnInfoLoad()
{
	// TODO: Add your command handler code here
	
	RefreshClassView();

	//m_wndClassView.DeleteItem(hItem);

	//m_wndClassView.RefreshData();
	
	//m_wndClassView.DeleteAllItems();

	//m_wndClassView.UpdateWindow();
	
	//FillClassView();

	//m_wndClassView.UpdateData(FALSE);
	//m_wndClassView.UpdateData(TRUE);
	
	//m_wndClassView.SetRedraw();	
	//m_wndClassView.DeleteAllItems();
	//m_wndClassView.UpdateWindow();
	//m_wndClassView.UnlockWindowUpdate();
	//m_wndClassView.Invalidate();

	/*
	do
	{
		//파일 경로를 추출
		hFind = FindFirstFile((mUserConfig.GetExeDirectory()+"AgtInfo\\*").c_str(), &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("파일경로 에러");
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
			lAgtXMLStorage.push_back(mXMLManager.LoadAllXMLinDir(mUserConfig.GetExeDirectory()+"AgtInfo\\" + FindData.cFileName, "AgentInfo", "AgentIP"));
			lAgtXMLStorage.push_back(mXMLManager.LoadAllXMLinDir(mUserConfig.GetExeDirectory()+"AgtInfo\\" + FindData.cFileName, "AgentInfo", "AgentName"));
			lAgtXMLStorage.push_back(mXMLManager.LoadAllXMLinDir(mUserConfig.GetExeDirectory()+"AgtInfo\\" + FindData.cFileName, "AgentInfo", "AgentLogFileList"));
		}
	}while(FindNextFile(hFind, &FindData));

	//DisplayAllElement_List(lAgtXMLStorage);
	*/

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