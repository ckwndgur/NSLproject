
// ChildFrm.h : CChildFrame 클래스의 인터페이스
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
	
// 특성입니다.
public:
	CScrollView* GetFileViewPane();
	CScrollView* GetFtViewPane();

// 작업입니다.
public:

// 재정의입니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// 구현입니다.
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};
