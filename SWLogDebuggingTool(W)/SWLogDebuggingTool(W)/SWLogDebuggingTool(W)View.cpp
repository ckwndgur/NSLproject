// SWLogDebuggingTool(W)View.cpp : CSWLogDebuggingToolWView 클래스의 구현
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"

#include "SWLogDebuggingTool(W)Doc.h"
#include "SWLogDebuggingTool(W)View.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSWLogDebuggingToolWView

IMPLEMENT_DYNCREATE(CSWLogDebuggingToolWView, CView)

BEGIN_MESSAGE_MAP(CSWLogDebuggingToolWView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSWLogDebuggingToolWView::OnFilePrintPreview)

	ON_COMMAND(ID_AGENT_INFOR, &CSWLogDebuggingToolWView::OnAgentInfor)
	ON_COMMAND(ID_AGENT_XML_LOAD, &CSWLogDebuggingToolWView::OnAgentXmlLoad)
END_MESSAGE_MAP()

// CSWLogDebuggingToolWView 생성/소멸

CSWLogDebuggingToolWView::CSWLogDebuggingToolWView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CSWLogDebuggingToolWView::~CSWLogDebuggingToolWView()
{
}

BOOL CSWLogDebuggingToolWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	//원격지정보를 저장하기 위한 XML파일 생성
	//mXMLManager.CreatXML_AgentInfo();
	mXMLManager.initXML();

	//통신 기능 초기화 - 소켓 생성 및 멀티캐스트 그룹주소 설정 등
	mUDPCommunication.WSAInit();
	mUDPCommunication.InitSocket_Wt(iUdpMultiSock, iUdpUniSock, iUdpSndSock);
	memset(&AddrStruct, 0, sizeof(AddrStruct));
	AddrStruct.sin_family = AF_INET;
	AddrStruct.sin_addr.s_addr = htonl(INADDR_ANY);
	AddrStruct.sin_port = htons(1883);
	bind(iUdpUniSock, (SOCKADDR*)&AddrStruct, sizeof(AddrStruct));

	return CView::PreCreateWindow(cs);
}

// CSWLogDebuggingToolWView 그리기

void CSWLogDebuggingToolWView::OnDraw(CDC* /*pDC*/)
{
	CSWLogDebuggingToolWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

}


// CSWLogDebuggingToolWView 인쇄


void CSWLogDebuggingToolWView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CSWLogDebuggingToolWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CSWLogDebuggingToolWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CSWLogDebuggingToolWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.

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


// CSWLogDebuggingToolWView 진단

#ifdef _DEBUG
void CSWLogDebuggingToolWView::AssertValid() const
{
	CView::AssertValid();
}

void CSWLogDebuggingToolWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSWLogDebuggingToolWDoc* CSWLogDebuggingToolWView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSWLogDebuggingToolWDoc)));
	return (CSWLogDebuggingToolWDoc*)m_pDocument;
}
#endif //_DEBUG


// CSWLogDebuggingToolWView 메시지 처리기

//Agent정보요청 버튼을 클릭하였을 때 실행되는 함수
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

	AfxMessageBox("종료");
}

//Agnet정보로드 버튼을 클릭하였을 때 실행되는 함수
void CSWLogDebuggingToolWView::OnAgentXmlLoad()
{
	// TODO: Add your command handler code here

	string sAlpha = mUserConfig.GetExeDirectory()+"AgtInfo\\*";
	do
	{
		//파일 경로를 추출
		hFind = FindFirstFile((mUserConfig.GetExeDirectory()+"AgtInfo\\*").c_str(), &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("파일경로 에러");
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
