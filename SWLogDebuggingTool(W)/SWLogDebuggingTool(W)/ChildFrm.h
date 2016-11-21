
// ChildFrm.h : CChildFrame Ŭ������ �������̽�
//

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

protected:
	CSplitterWndEx m_wndSplitter;
	CSplitterWndEx m_wndSplitterDown;
	
// Ư���Դϴ�.
public:
	CScrollView* GetFileViewPane();
	CScrollView* GetFtViewPane();

// �۾��Դϴ�.
public:

// �������Դϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// �����Դϴ�.
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};
