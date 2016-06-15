// ParametersPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "nrg.h"
#include "ParametersPropertyPage.h"
#include "afxdialogex.h"


// CParametersPropertyPage dialog

IMPLEMENT_DYNAMIC(CParametersPropertyPage, CPropertyPage)

CParametersPropertyPage::CParametersPropertyPage()
	: CPropertyPage(IDD_PARAMETERSPROPERTYPAGE)
{
	U = theApp.options.U;
	eps = theApp.options.eps;
	J = theApp.options.J;
	B = theApp.options.B;
	delta = theApp.options.delta;

	b = theApp.options.b;
	step = theApp.options.step;
}

CParametersPropertyPage::~CParametersPropertyPage()
{
}

void CParametersPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, m_UEdit);
	DDX_Control(pDX, IDC_EDIT2, m_epsEdit);
	DDX_Control(pDX, IDC_EDIT3, m_JEdit);
	DDX_Control(pDX, IDC_EDIT4, m_deltaEdit);
	DDX_Control(pDX, IDC_EDIT5, m_BEdit);
	// for spectral function
	DDX_Control(pDX, IDC_EDIT6, m_bEdit);
	DDX_Control(pDX, IDC_EDIT7, m_stepEdit);

	DDX_Text(pDX, IDC_EDIT1, U);
	DDX_Text(pDX, IDC_EDIT2, eps);
	DDX_Text(pDX, IDC_EDIT3, J);
	DDX_Text(pDX, IDC_EDIT4, delta);
	DDX_Text(pDX, IDC_EDIT5, B);
	DDX_Text(pDX, IDC_EDIT6, b);
	DDX_Text(pDX, IDC_EDIT7, step);



}


BEGIN_MESSAGE_MAP(CParametersPropertyPage, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CParametersPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &CParametersPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT3, &CParametersPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT4, &CParametersPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT5, &CParametersPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT6, &CParametersPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT7, &CParametersPropertyPage::OnEnChangeEdit)
END_MESSAGE_MAP()


// CParametersPropertyPage message handlers


BOOL CParametersPropertyPage::OnApply()
{
	UpdateData();

	ApplyValues();

	return CPropertyPage::OnApply();
}


BOOL CParametersPropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_UEdit.allowNegative = false;
	m_deltaEdit.allowNegative = false;
	m_bEdit.allowNegative = false;
	m_stepEdit.allowNegative = false;


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CParametersPropertyPage::ApplyValues()
{
	theApp.options.U = U;
	theApp.options.eps = eps;
	theApp.options.J = J;
	theApp.options.B = B;
	theApp.options.delta = delta;

	theApp.options.b = b;
	theApp.options.step = step;

	theApp.options.Save();
}


void CParametersPropertyPage::OnEnChangeEdit()
{
	SetModified();
}


