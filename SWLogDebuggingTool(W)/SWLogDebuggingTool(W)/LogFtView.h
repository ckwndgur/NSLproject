#pragma once

#include "TextManager.h"
#include "Filter.h"



// LogFtView view

class LogFtView : public CScrollView
{
	DECLARE_DYNCREATE(LogFtView)

protected:
	LogFtView();           // protected constructor used by dynamic creation
	virtual ~LogFtView();

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


