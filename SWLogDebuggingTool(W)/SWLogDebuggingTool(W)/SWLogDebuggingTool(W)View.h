#include "UDPCommunication.h"
#include "AgtInfoList.h"
#include "TextManager.h"
#include "OutputWnd.h"
#include "XMLManager.h"
#include "GetDataFromAgent_WIN.h"

// SWLogDebuggingTool(W)View.h : CSWLogDebuggingToolWView 클래스의 인터페이스
//

#pragma once


class CSWLogDebuggingToolWView : public CView
{
protected: // serialization에서만 만들어집니다.
	CSWLogDebuggingToolWView();
	DECLARE_DYNCREATE(CSWLogDebuggingToolWView)

	// 특성입니다.
public:
	CSWLogDebuggingToolWDoc* GetDocument() const;

	// 작업입니다.
public:


	//원격지 정보 저장 부분 - XML
	XMLManager mXMLManager;

	//원격지 정보 불러오기 부분 - XML
	WIN32_FIND_DATA FindData;
	HANDLE hFind;
	HANDLE hFile;
	UserConfig mUserConfig;
	list<string> lAgtXMLStorage;

	//UDP통신부분 - Agent Info
	SOCKADDR_IN AddrStruct;
	int iUdpMultiSock, iUdpUniSock, iUdpSndSock;
	list<string> lAgtInfoList;
	COutputWnd mCOutputWnd;
	UDPCommunication mUDPCommunication;
	AgtInfoList mAgtInfoList;

	// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
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
public:
	afx_msg void OnAgentInfor();
	afx_msg void OnAgentXmlLoad();
};

#ifndef _DEBUG  // SWLogDebuggingTool(W)View.cpp의 디버그 버전
inline CSWLogDebuggingToolWDoc* CSWLogDebuggingToolWView::GetDocument() const
{ return reinterpret_cast<CSWLogDebuggingToolWDoc*>(m_pDocument); }
#endif

