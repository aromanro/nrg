#pragma once
#include "NumberEdit.h"


// CChartsPropertyPage dialog

class CChartsPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CChartsPropertyPage)

public:
	CChartsPropertyPage();
	virtual ~CChartsPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARTSPROPERTYPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
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
