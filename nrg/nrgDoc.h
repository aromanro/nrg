
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

public:
	bool Tick();
	void StartComputation();
	void SetupCharts();
	void CancelThread();
	CnrgView* GetView();

	NRGResultsData computationData;
	NRGController controller;

	std::thread m_thread;

	Chart m_RenormalizationFlowChart;
	Chart m_SpectralFunctionChart;

	BOOL OnNewDocument() override;
	void Serialize(CArchive& ar) override;
#ifdef SHARED_HANDLERS
	void InitializeSearchContent() override;
	void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds) override;
#endif // SHARED_HANDLERS

// Implementation
	~CnrgDoc() override;
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

protected:
// Generated message map functions
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
