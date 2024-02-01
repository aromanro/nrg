#pragma once

#include "NumberEdit.h"

// CParametersPropertyPage dialog

class CParametersPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CParametersPropertyPage)
	DECLARE_MESSAGE_MAP()

public:
	CParametersPropertyPage();

	BOOL OnApply() override;
	BOOL OnInitDialog() override;
	void ApplyValues();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAMETERSPROPERTYPAGE };
#endif

private:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	CNumberEdit m_UEdit;

	CNumberEdit m_epsEdit;
	CNumberEdit m_JEdit;
	CNumberEdit m_BEdit;
	CNumberEdit m_deltaEdit;
	// for spectral function
	CNumberEdit m_bEdit;
	CNumberEdit m_stepEdit;

	afx_msg void OnEnChangeEdit();

	// Parameters
	// for models
	double U;
	double eps;
	double J;
	double B;
	double delta;
	// for spectral function
	double b;
	double step;
};
