
// nrgView.h : interface of the CnrgView class
//

#pragma once


class CnrgView : public CView
{
protected: // create from serialization only
	CnrgView();
	DECLARE_DYNCREATE(CnrgView)
	DECLARE_MESSAGE_MAP()

public:
	~CnrgView() override;

	CnrgDoc* GetDocument() const;

	void StartRefreshing();

	void OnDraw(CDC* pDC) override;  // overridden to draw this view
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;

#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

protected:
	UINT_PTR timer;

	BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	void OnInitialUpdate() override;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL) override;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in nrgView.cpp
inline CnrgDoc* CnrgView::GetDocument() const
   { return reinterpret_cast<CnrgDoc*>(m_pDocument); }
#endif

