// ChartsPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "nrg.h"
#include "ChartsPropertyPage.h"
#include "afxdialogex.h"

#include "nrg.h"
#include "Options.h"

// CChartsPropertyPage dialog

IMPLEMENT_DYNAMIC(CChartsPropertyPage, CMFCPropertyPage)

CChartsPropertyPage::CChartsPropertyPage()
	: CMFCPropertyPage(IDD_CHARTSPROPERTYPAGE)	
{
	YMaxFlow = theApp.options.YMaxFlow;
	YMaxSpectral = theApp.options.YMaxSpectral;
	XMinSpectral = theApp.options.XMinSpectral;
	XMaxSpectral = theApp.options.XMaxSpectral;
	YBigTicksFlow = theApp.options.YBigTicksFlow;
	YSmallTicksFlow = theApp.options.YSmallTicksFlow;
	YBigTicksSpectral = theApp.options.YBigTicksSpectral;
	YSmallTicksSpectral = theApp.options.YSmallTicksSpectral;
	XBigTicksSpectral = theApp.options.XBigTicksSpectral;
	XSmallTicksSpectral = theApp.options.XSmallTicksSpectral;
}

CChartsPropertyPage::~CChartsPropertyPage()
{
}

void CChartsPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EnergyEdit);
	DDX_Control(pDX, IDC_EDIT4, m_SpectralMaxEdit);
	DDX_Control(pDX, IDC_EDIT7, m_FrequencyMinEdit);
	DDX_Control(pDX, IDC_EDIT8, m_FrequencyMaxEdit);

	DDX_Text(pDX, IDC_EDIT1, YMaxFlow);
	DDX_Text(pDX, IDC_EDIT2, YBigTicksFlow);
	DDX_Text(pDX, IDC_EDIT3, YSmallTicksFlow);
	DDX_Text(pDX, IDC_EDIT4, YMaxSpectral);
	DDX_Text(pDX, IDC_EDIT5, YBigTicksSpectral);
	DDX_Text(pDX, IDC_EDIT6, YSmallTicksSpectral);
	DDX_Text(pDX, IDC_EDIT7, XMinSpectral);
	DDX_Text(pDX, IDC_EDIT8, XMaxSpectral);
	DDX_Text(pDX, IDC_EDIT9, XBigTicksSpectral);
	DDX_Text(pDX, IDC_EDIT10, XSmallTicksSpectral);
}


BEGIN_MESSAGE_MAP(CChartsPropertyPage, CMFCPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT3, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT4, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT5, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT6, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT7, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT8, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT9, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT10, &CChartsPropertyPage::OnEnChangeEdit)
END_MESSAGE_MAP()


// CChartsPropertyPage message handlers


BOOL CChartsPropertyPage::OnApply()
{
	UpdateData();

	ApplyValues();

	return CMFCPropertyPage::OnApply();
}


BOOL CChartsPropertyPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	m_EnergyEdit.allowNegative = false;
	m_SpectralMaxEdit.allowNegative = false;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CChartsPropertyPage::ApplyValues()
{
	theApp.options.YMaxFlow = YMaxFlow;
	theApp.options.YMaxSpectral = YMaxSpectral;
	theApp.options.XMinSpectral = XMinSpectral;
	theApp.options.XMaxSpectral = XMaxSpectral;
	theApp.options.YBigTicksFlow = YBigTicksFlow;
	theApp.options.YSmallTicksFlow = YSmallTicksFlow;
	theApp.options.YBigTicksSpectral = YBigTicksSpectral;
	theApp.options.YSmallTicksSpectral = YSmallTicksSpectral;
	theApp.options.XBigTicksSpectral = XBigTicksSpectral;
	theApp.options.XSmallTicksSpectral = XSmallTicksSpectral;

	theApp.options.Save();
}


void CChartsPropertyPage::OnEnChangeEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CMFCPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	SetModified();
}
