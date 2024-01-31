#pragma once
#include "NumberEdit.h"


// CNRGPropertyPage dialog

class CNRGPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CNRGPropertyPage)

public:
	CNRGPropertyPage();
	~CNRGPropertyPage() override;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NRGPROPERTYPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	void ApplyValues();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit();
	afx_msg void OnBnClickedRadio();

	unsigned int iterationsNumber;
	unsigned int keptStates;
	double Lambda;
	int model;
	
	CNumberEdit m_numberEdit;
};
