
// nrgView.cpp : implementation of the CnrgView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "nrg.h"
#endif

#include "nrgDoc.h"
#include "nrgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CnrgView

IMPLEMENT_DYNCREATE(CnrgView, CView)

BEGIN_MESSAGE_MAP(CnrgView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CnrgView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CnrgView construction/destruction

CnrgView::CnrgView()
	: timer(NULL)
{
	// TODO: add construction code here

}

CnrgView::~CnrgView()
{
}

BOOL CnrgView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CnrgView drawing

void CnrgView::OnDraw(CDC* pDC)
{
	CnrgDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	
	CRect rect;
	GetClientRect(rect);

	CRect rect1(rect);
	CRect rect2(rect);
	rect1.bottom = rect1.top + rect1.Height()/2;

	pDoc->m_RenormalizationFlowChart.Draw(pDC, rect1);
	

	rect2.top = rect2.top + rect2.Height() / 2;

	pDoc->m_SpectralFunctionChart.Draw(pDC, rect2);
}


// CnrgView printing


void CnrgView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CnrgView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CnrgView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CnrgView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CnrgView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CnrgView::OnContextMenu(CWnd* /* pWnd */, CPoint /*point*/)
{
#ifndef SHARED_HANDLERS
#endif
}


// CnrgView diagnostics

#ifdef _DEBUG
void CnrgView::AssertValid() const
{
	CView::AssertValid();
}

void CnrgView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CnrgDoc* CnrgView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CnrgDoc)));
	return (CnrgDoc*)m_pDocument;
}
#endif //_DEBUG


// CnrgView message handlers


void CnrgView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
}


void CnrgView::OnDestroy()
{
	CView::OnDestroy();

	if (timer) KillTimer(timer);
}


void CnrgView::OnTimer(UINT_PTR nIDEvent)
{
	CView::OnTimer(nIDEvent);

	CnrgDoc* pDoc = GetDocument();

	if (pDoc->computationData.isFinished())
	{
		KillTimer(timer);
		timer = 0;

		EndWaitCursor();
		Invalidate();
	}

	if (pDoc->Tick()) Invalidate();
}


void CnrgView::StartRefreshing()
{
	if (!timer) timer = SetTimer(1, 1000, NULL);
	BeginWaitCursor();
}


void CnrgView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CView::OnPrepareDC(pDC, pInfo);

	if (pDC->IsPrinting())
	{
		CRect rect;
		GetClientRect(rect);

		pDC->SetMapMode(MM_ISOTROPIC);

		int cx = pDC->GetDeviceCaps(HORZRES);
		int cy = pDC->GetDeviceCaps(VERTRES);
		
		pDC->SetWindowExt(rect.Width(), rect.Height());
		pDC->SetViewportExt(cx, cy);
		pDC->SetViewportOrg(0, 0);
	}
}


BOOL CnrgView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CnrgDoc* pDoc = GetDocument();

	if (pDoc && !pDoc->computationData.isFinished())
	{
		RestoreWaitCursor();

		return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}
