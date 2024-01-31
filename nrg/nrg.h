
// nrg.h : main header file for the nrg application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include <objidl.h>
#include <gdiplus.h>

#pragma comment (lib,"Gdiplus.lib")

#include "Options.h"

// CnrgApp:
// See nrg.cpp for the implementation of this class
//

class CnrgApp : public CWinAppEx
{
public:
	CnrgApp();

// Overrides
	BOOL InitInstance() override;
	void PreLoadState() override;
	void LoadCustomState() override;
	void SaveCustomState() override;

	int ExitInstance() override;

// Implementation

private:
	ULONG_PTR gdiplusToken;

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	Options options;

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CnrgApp theApp;
