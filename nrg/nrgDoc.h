
// nrgDoc.h : interface of the CnrgDoc class
//


#pragma once

#include "NRGComputationThread.h"

#include "Chart.h"
#include "NRGResultsData.h"
#include "NRGController.h"

class CnrgView;

class CnrgDoc : public CDocument
{
protected: // create from serialization only
	CnrgDoc();
	DECLARE_DYNCREATE(CnrgDoc)

// Attributes
public:
	NRGResultsData computationData;
	NRGController controller;

	std::thread m_thread;

// Operations
public:

	Chart m_RenormalizationFlowChart;
	Chart m_SpectralFunctionChart;

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CnrgDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	bool Tick();
	void StartComputation();
	void SetupCharts();
	void CancelThread();
	CnrgView* GetView();
};
