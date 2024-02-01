
// nrgDoc.cpp : implementation of the CnrgDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "nrg.h"
#endif

#include "nrgDoc.h"

#include <propkey.h>

#include "nrg.h"
#include "Options.h"

#include "QDAnderson.h"
#include "QDKondo.h"
#include "TwoQDRKKY.h"
#include "nrgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CnrgDoc

IMPLEMENT_DYNCREATE(CnrgDoc, CDocument)

BEGIN_MESSAGE_MAP(CnrgDoc, CDocument)
END_MESSAGE_MAP()


// CnrgDoc construction/destruction

CnrgDoc::CnrgDoc()
{
	// TODO: add one-time construction code here

	m_RenormalizationFlowChart.title = L"Renormalization Group Flow";
	m_RenormalizationFlowChart.XAxisLabel = L"Iteration";
	m_RenormalizationFlowChart.YAxisLabel = L"Energy";

	m_SpectralFunctionChart.title = L"Spectral Function";

	m_SpectralFunctionChart.XAxisLabel = L"ω";
	m_SpectralFunctionChart.YAxisLabel = L"A(ω)";
	
	m_RenormalizationFlowChart.XAxisMin = 0;
	m_RenormalizationFlowChart.YAxisMin = 0;


	SetupCharts();

	computationData.spectralChart = &m_SpectralFunctionChart;
}

CnrgDoc::~CnrgDoc()
{
	CancelThread();
	if (m_thread.joinable()) m_thread.join();
}

BOOL CnrgDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}




// CnrgDoc serialization

void CnrgDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CnrgDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CnrgDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CnrgDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CnrgDoc diagnostics

#ifdef _DEBUG
void CnrgDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CnrgDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CnrgDoc commands


bool CnrgDoc::Tick()
{
	const bool res = computationData.TransferToChart(m_RenormalizationFlowChart);

	if (computationData.isFinished())
		return true;

	return res;
}


void CnrgDoc::StartComputation()
{
	if (m_thread.joinable()) m_thread.join();

	NRG::NRGAlgorithm* nrg = nullptr;

	switch (theApp.options.model)
	{
	case 0:
		SetTitle(L"Anderson");
		nrg = new NRG::QDAnderson(theApp.options.iterationsNumber, theApp.options.keptStates, theApp.options.Lambda);
		break;
	case 1:
		SetTitle(L"Kondo");
		nrg = new NRG::QDKondo(theApp.options.iterationsNumber, theApp.options.keptStates, theApp.options.Lambda);
		break;
	case 2:
		SetTitle(L"Two Quantum Dots");
		nrg = new NRG::TwoQDRKKY(theApp.options.iterationsNumber, theApp.options.keptStates, theApp.options.Lambda);
		break;
	}

	if (nrg)
	{		
		controller.endIt = false;
		computationData.resetFinished();

		m_RenormalizationFlowChart.clear();
		m_SpectralFunctionChart.clear();

		SetupCharts();

		CnrgView* view = GetView();
		if (view)
		{
			view->StartRefreshing();			
		}

		nrg->SetResultsRetriever(&computationData);
		nrg->SetController(&controller);
		nrg->Init();

		auto thread = new NRGComputationThread(nrg);		
		m_thread = thread->Start();
	}
}


void CnrgDoc::SetupCharts()
{
	// renormalization flow chart

	m_RenormalizationFlowChart.XAxisMax = theApp.options.iterationsNumber;
	m_RenormalizationFlowChart.YAxisMax = theApp.options.YMaxFlow;

	const int bigTicks = static_cast<int>(ceill(theApp.options.iterationsNumber / 10.));
	m_RenormalizationFlowChart.SetNumBigTicksX(bigTicks);
	m_RenormalizationFlowChart.SetNumTicksX(bigTicks*2); // for now


	m_RenormalizationFlowChart.SetNumBigTicksY(theApp.options.YBigTicksFlow);
	m_RenormalizationFlowChart.SetNumTicksY(theApp.options.YBigTicksFlow*theApp.options.YSmallTicksFlow);


	// spectral function chart

	m_SpectralFunctionChart.XAxisMin = theApp.options.XMinSpectral;
	m_SpectralFunctionChart.XAxisMax = theApp.options.XMaxSpectral;
	m_SpectralFunctionChart.SetNumBigTicksX(theApp.options.XBigTicksSpectral);
	m_SpectralFunctionChart.SetNumTicksX(theApp.options.XBigTicksSpectral*theApp.options.XSmallTicksSpectral);

	m_SpectralFunctionChart.YAxisMin = 0;
	m_SpectralFunctionChart.YAxisMax = theApp.options.YMaxSpectral;
	m_SpectralFunctionChart.SetNumBigTicksY(theApp.options.YBigTicksSpectral);
	m_SpectralFunctionChart.SetNumTicksY(theApp.options.YBigTicksSpectral*theApp.options.YSmallTicksSpectral);
}


void CnrgDoc::CancelThread()
{
	controller.endIt = true;
}


CnrgView* CnrgDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	while (pos)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CnrgView)))
			return dynamic_cast<CnrgView*>(pView);
	}

	return nullptr;
}
