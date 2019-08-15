
// AeCC-Force-English.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAeCCForceEnglishApp:
// See AeCC-Force-English.cpp for the implementation of this class
//

class CAeCCForceEnglishApp : public CWinApp
{
public:
	CAeCCForceEnglishApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CAeCCForceEnglishApp theApp;