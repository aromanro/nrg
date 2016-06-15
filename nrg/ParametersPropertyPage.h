#pragma once

#include "NumberEdit.h"

// CParametersPropertyPage dialog

class CParametersPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CParametersPropertyPage)

public:
	CParametersPropertyPage();
	virtual ~CParametersPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAMETERSPROPERTYPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
	void ApplyValues();

protected:
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
