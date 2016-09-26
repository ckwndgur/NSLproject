#include "UDPCommunication.h"
#include "AgtInfoList.h"
#include "TextManager.h"
#include "OutputWnd.h"
#include "XMLManager.h"
#include "GetDataFromAgent_WIN.h"

// SWLogDebuggingTool(W)View.h : CSWLogDebuggingToolWView Ŭ������ �������̽�
//

#pragma once


class CSWLogDebuggingToolWView : public CView
{
protected: // serialization������ ��������ϴ�.
	CSWLogDebuggingToolWView();
	DECLARE_DYNCREATE(CSWLogDebuggingToolWView)

	// Ư���Դϴ�.
public:
	CSWLogDebuggingToolWDoc* GetDocument() const;

	// �۾��Դϴ�.
public:


	//������ ���� ���� �κ� - XML
	XMLManager mXMLManager;

	//������ ���� �ҷ����� �κ� - XML
	WIN32_FIND_DATA FindData;
	HANDLE hFind;
	HANDLE hFile;
	UserConfig mUserConfig;
	list<string> lAgtXMLStorage;

	//UDP��źκ� - Agent Info
	SOCKADDR_IN AddrStruct;
	int iUdpMultiSock, iUdpUniSock, iUdpSndSock;
	list<string> lAgtInfoList;
	COutputWnd mCOutputWnd;
	UDPCommunication mUDPCommunication;
	AgtInfoList mAgtInfoList;

	// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
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
public:
	afx_msg void OnAgentInfor();
	afx_msg void OnAgentXmlLoad();
};

#ifndef _DEBUG  // SWLogDebuggingTool(W)View.cpp�� ����� ����
inline CSWLogDebuggingToolWDoc* CSWLogDebuggingToolWView::GetDocument() const
{ return reinterpret_cast<CSWLogDebuggingToolWDoc*>(m_pDocument); }
#endif

