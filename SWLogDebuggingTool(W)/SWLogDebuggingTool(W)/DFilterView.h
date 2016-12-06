#pragma once



// DFilterView view

class DFilterView : public CScrollView
{
	DECLARE_DYNCREATE(DFilterView)

protected:
	DFilterView();           // protected constructor used by dynamic creation
	virtual ~DFilterView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
};


