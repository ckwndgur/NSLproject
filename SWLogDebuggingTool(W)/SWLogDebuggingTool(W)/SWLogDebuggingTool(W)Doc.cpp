
// SWLogDebuggingTool(W)Doc.cpp : CSWLogDebuggingToolWDoc Ŭ������ ����
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"

#include "SWLogDebuggingTool(W)Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSWLogDebuggingToolWDoc

IMPLEMENT_DYNCREATE(CSWLogDebuggingToolWDoc, CDocument)

BEGIN_MESSAGE_MAP(CSWLogDebuggingToolWDoc, CDocument)
END_MESSAGE_MAP()


// CSWLogDebuggingToolWDoc ����/�Ҹ�

CSWLogDebuggingToolWDoc::CSWLogDebuggingToolWDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CSWLogDebuggingToolWDoc::~CSWLogDebuggingToolWDoc()
{
}

BOOL CSWLogDebuggingToolWDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}

BOOL CSWLogDebuggingToolWDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;

	return TRUE;
}


// CSWLogDebuggingToolWDoc serialization

void CSWLogDebuggingToolWDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CSWLogDebuggingToolWDoc ����

#ifdef _DEBUG
void CSWLogDebuggingToolWDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSWLogDebuggingToolWDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSWLogDebuggingToolWDoc ���
