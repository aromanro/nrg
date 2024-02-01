#pragma once
#include "NumberEdit.h"


// CNRGPropertyPage dialog

class CNRGPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CNRGPropertyPage)

public:
	CNRGPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NRGPROPERTYPAGE };
#endif

private:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void ApplyValues();
	BOOL OnApply() override;
	BOOL OnInitDialog() override;
	afx_msg void OnEnChangeEdit();
	afx_msg void OnBnClickedRadio();

	unsigned int iterationsNumber;
	unsigned int keptStates;
	double Lambda;
	int model;
	
	CNumberEdit m_numberEdit;
};
