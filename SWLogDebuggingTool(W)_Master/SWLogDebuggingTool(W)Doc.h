
// SWLogDebuggingTool(W)Doc.h : CSWLogDebuggingToolWDoc Ŭ������ �������̽�
//


#pragma once


class CSWLogDebuggingToolWDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CSWLogDebuggingToolWDoc();
	DECLARE_DYNCREATE(CSWLogDebuggingToolWDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
// �����Դϴ�.
public:
	virtual ~CSWLogDebuggingToolWDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


