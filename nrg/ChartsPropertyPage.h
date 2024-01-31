#pragma once
#include "NumberEdit.h"


// CChartsPropertyPage dialog

class CChartsPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CChartsPropertyPage)
	DECLARE_MESSAGE_MAP()

public:
	CChartsPropertyPage();
	~CChartsPropertyPage() override;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARTSPROPERTYPAGE };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	BOOL OnApply() override;
	BOOL OnInitDialog() override;
	void ApplyValues();
	afx_msg void OnEnChangeEdit();

	double YMaxFlow;
	double YMaxSpectral;
	double XMinSpectral;
	double XMaxSpectral;
	unsigned int YBigTicksFlow;
	unsigned int YSmallTicksFlow;
	unsigned int YBigTicksSpectral;
	unsigned int YSmallTicksSpectral;
	unsigned int XBigTicksSpectral;
	unsigned int XSmallTicksSpectral;
	CNumberEdit m_EnergyEdit;
	CNumberEdit m_SpectralMaxEdit;
	CNumberEdit m_FrequencyMinEdit;
	CNumberEdit m_FrequencyMaxEdit;
};
