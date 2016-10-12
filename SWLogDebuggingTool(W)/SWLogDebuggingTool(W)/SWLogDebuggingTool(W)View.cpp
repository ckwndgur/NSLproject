// SWLogDebuggingTool(W)View.cpp : CSWLogDebuggingToolWView Ŭ������ ����
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W)Doc.h"
#include "SWLogDebuggingTool(W)View.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSWLogDebuggingToolWView

IMPLEMENT_DYNCREATE(CSWLogDebuggingToolWView, CView)

BEGIN_MESSAGE_MAP(CSWLogDebuggingToolWView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSWLogDebuggingToolWView::OnFilePrintPreview)

	ON_COMMAND(ID_AGENT_INFOR, &CSWLogDebuggingToolWView::OnAgentInfor)
	ON_COMMAND(ID_AGENT_XML_LOAD, &CSWLogDebuggingToolWView::OnAgentXmlLoad)
END_MESSAGE_MAP()

// CSWLogDebuggingToolWView ����/�Ҹ�

CSWLogDebuggingToolWView::CSWLogDebuggingToolWView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CSWLogDebuggingToolWView::~CSWLogDebuggingToolWView()
{
	/*delete m_btn;*/
}

BOOL CSWLogDebuggingToolWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	//������������ �����ϱ� ���� XML���� ����
	//mXMLManager.CreatXML_AgentInfo();
	mXMLManager.initXML();

	//��� ��� �ʱ�ȭ - ���� ���� �� ��Ƽĳ��Ʈ �׷��ּ� ���� ��
	mUDPCommunication.WSAInit();
	mUDPCommunication.InitSocket_Wt(iUdpMultiSock, iUdpUniSock, iUdpSndSock);
	memset(&AddrStruct, 0, sizeof(AddrStruct));
	AddrStruct.sin_family = AF_INET;
	AddrStruct.sin_addr.s_addr = htonl(INADDR_ANY);
	AddrStruct.sin_port = htons(1883);
	bind(iUdpUniSock, (SOCKADDR*)&AddrStruct, sizeof(AddrStruct));

	

	return CView::PreCreateWindow(cs);
}

void CSWLogDebuggingToolWView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

// 	m_btn = new CButton();
// 	m_btn->Create("a", BS_DEFPUSHBUTTON, CRect(0,0,200,50), this, 100);

}

// CSWLogDebuggingToolWView �׸���

void CSWLogDebuggingToolWView::OnDraw(CDC* pDC)
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CSWLogDebuggingToolWView *pView = (CSWLogDebuggingToolWView *)pFrame->GetActiveView();
	

	CSWLogDebuggingToolWDoc* pDoc = GetDocument();
/*	m_btn->ShowWindow(SW_SHOW);*/
	
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	pDC->TextOut(0,0,_T("sdsa"));
	
}


// CSWLogDebuggingToolWView �μ�


void CSWLogDebuggingToolWView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CSWLogDebuggingToolWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CSWLogDebuggingToolWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CSWLogDebuggingToolWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.

}

void CSWLogDebuggingToolWView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSWLogDebuggingToolWView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CSWLogDebuggingToolWView ����

#ifdef _DEBUG
void CSWLogDebuggingToolWView::AssertValid() const
{
	CView::AssertValid();
}

void CSWLogDebuggingToolWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSWLogDebuggingToolWDoc* CSWLogDebuggingToolWView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSWLogDebuggingToolWDoc)));
	return (CSWLogDebuggingToolWDoc*)m_pDocument;
}
#endif //_DEBUG


// CSWLogDebuggingToolWView �޽��� ó����

//Agent������û ��ư�� Ŭ���Ͽ��� �� ����Ǵ� �Լ�
void CSWLogDebuggingToolWView::OnAgentInfor()
{
	// TODO: Add your command handler code here
	mUDPCommunication.InforReq(iUdpSndSock, 1883, "234.56.78.9");

	lAgtInfoList = mUDPCommunication.RcvInfor_nonTimeout(iUdpUniSock, 5);
	//lAgtInfoList = mUDPCommunication.RcvInfor(iUdpUniSock, 5);

	//mCOutputWnd.FillAgentInfoWindow(lAgtInfoList);
	string sListElement = "";

	list<string>::iterator iterStartList = lAgtInfoList.begin();
	list<string>::iterator iterEndList = lAgtInfoList.end();

	while(iterStartList != iterEndList)
	{
		sListElement = *iterStartList;
		AfxMessageBox(sListElement.c_str());
		iterStartList++;
	}

	AfxMessageBox("����");
}

//Agnet�����ε� ��ư�� Ŭ���Ͽ��� �� ����Ǵ� �Լ�
void CSWLogDebuggingToolWView::OnAgentXmlLoad()
{
	// TODO: Add your command handler code here

	string sAlpha = mUserConfig.GetExeDirectory()+"AgtInfo\\*";
	do
	{
		//���� ��θ� ����
		hFind = FindFirstFile((mUserConfig.GetExeDirectory()+"AgtInfo\\*").c_str(), &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("���ϰ�� ����");
			//fputs("FindFirstFile() error\n", stderr);
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

	string sListElement = "";

	list<string>::iterator iterStartList = lAgtXMLStorage.begin();
	list<string>::iterator iterEndList = lAgtXMLStorage.end();

	while(iterStartList != iterEndList)
	{
		sListElement = *iterStartList;
		AfxMessageBox(sListElement.c_str());
		iterStartList++;
	}
}
