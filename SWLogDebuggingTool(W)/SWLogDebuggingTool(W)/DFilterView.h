#pragma once

#include "TextManager.h"
#include "Filter.h"


// DFilterView view

class DFilterView : public CScrollView
{
	DECLARE_DYNCREATE(DFilterView)

protected:
	

public:
	DFilterView();           // protected constructor used by dynamic creation
	virtual ~DFilterView();

	CString filterform;

	void MakeFilterForm(CString columnlist);

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


