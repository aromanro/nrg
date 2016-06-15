
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
public:
	virtual BOOL InitInstance();

// Implementation

private:
	ULONG_PTR gdiplusToken;

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	Options options;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CnrgApp theApp;
