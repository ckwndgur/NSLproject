
// SWLogDebuggingTool(W).h : SWLogDebuggingTool(W) ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CSWLogDebuggingToolWApp:
// �� Ŭ������ ������ ���ؼ��� SWLogDebuggingTool(W).cpp�� �����Ͻʽÿ�.
//

class CSWLogDebuggingToolWApp : public CWinAppEx
{
public:
	CSWLogDebuggingToolWApp();
	CMultiDocTemplate* pDocTemplate;

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	
};

extern CSWLogDebuggingToolWApp theApp;
