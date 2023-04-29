
// GitClone.h : fichier d'en-tête principal de l'application PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'pch.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"		// symboles principaux


// CGitCloneApp :
// Consultez GitClone.cpp pour l'implémentation de cette classe
//

class CGitCloneApp : public CWinApp
{
public:
	CGitCloneApp();

// Substitutions
public:
	virtual BOOL InitInstance();

// Implémentation

	DECLARE_MESSAGE_MAP()
};

extern CGitCloneApp theApp;
