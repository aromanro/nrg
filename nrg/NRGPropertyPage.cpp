// NRGPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "nrg.h"
#include "NRGPropertyPage.h"
#include "afxdialogex.h"

#include "nrg.h"

// CNRGPropertyPage dialog

IMPLEMENT_DYNAMIC(CNRGPropertyPage, CMFCPropertyPage)

CNRGPropertyPage::CNRGPropertyPage()
	: CMFCPropertyPage(IDD_NRGPROPERTYPAGE)
{
	iterationsNumber = theApp.options.iterationsNumber;
	keptStates = theApp.options.keptStates;
	Lambda = theApp.options.Lambda;
	model = theApp.options.model;
}

void CNRGPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, m_numberEdit);
	DDX_Radio(pDX, IDC_RADIO1, model);

	DDX_Text(pDX, IDC_EDIT1, iterationsNumber);
	DDX_Text(pDX, IDC_EDIT2, keptStates);
	DDX_Text(pDX, IDC_EDIT3, Lambda);

	DDV_MinMaxUInt(pDX, iterationsNumber, 10, 80);
	DDV_MinMaxUInt(pDX, keptStates, 50, 8000);
	DDV_MinMaxDouble(pDX, Lambda, 1., 4.);
}


BEGIN_MESSAGE_MAP(CNRGPropertyPage, CMFCPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CNRGPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &CNRGPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT3, &CNRGPropertyPage::OnEnChangeEdit)
	ON_BN_CLICKED(IDC_RADIO1, &CNRGPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO2, &CNRGPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO3, &CNRGPropertyPage::OnBnClickedRadio)
END_MESSAGE_MAP()


// CNRGPropertyPage message handlers


void CNRGPropertyPage::ApplyValues()
{
	theApp.options.iterationsNumber = iterationsNumber;
	theApp.options.keptStates = keptStates;
	theApp.options.Lambda = Lambda;
	theApp.options.model = model;

	theApp.options.Save();
}


BOOL CNRGPropertyPage::OnApply()
{
	UpdateData();

	ApplyValues();

	return CMFCPropertyPage::OnApply();
}


BOOL CNRGPropertyPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_numberEdit.allowNegative = false;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CNRGPropertyPage::OnEnChangeEdit()
{
	SetModified();
}



void CNRGPropertyPage::OnBnClickedRadio()
{
	SetModified();
}


