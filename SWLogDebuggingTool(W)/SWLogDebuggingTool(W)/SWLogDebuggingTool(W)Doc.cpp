
// SWLogDebuggingTool(W)Doc.cpp : CSWLogDebuggingToolWDoc 클래스의 구현
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


// CSWLogDebuggingToolWDoc 생성/소멸

CSWLogDebuggingToolWDoc::CSWLogDebuggingToolWDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CSWLogDebuggingToolWDoc::~CSWLogDebuggingToolWDoc()
{
}

BOOL CSWLogDebuggingToolWDoc::OnNewDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

BOOL CSWLogDebuggingToolWDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}


// CSWLogDebuggingToolWDoc serialization

void CSWLogDebuggingToolWDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CSWLogDebuggingToolWDoc 진단

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


// CSWLogDebuggingToolWDoc 명령
