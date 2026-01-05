#include "Base.h"
#include "CoreLib.h"
#include "RandUtils.h"
#include "ExceptionUtils.h"
#ifdef _WIN32
#include "FileSystemUtils.h"
#include "WtsUtils.h"
#else
#include "NetUtils.h"
#include <clocale>
#endif

#ifdef _DEBUG
#include "Diagnostic.h"
#endif

NS_BEGIN

//#################################################################################################
void CoreLibInit(void)
{
	ExceptionInit();
	SetProcessExceptionHandlers();

#ifndef _WIN32
	std::setlocale(LC_CTYPE, "en_US.UTF-8");
#endif

#ifdef _DEBUG
	DiagnosticInit();
#endif
	CRandom::Init();

#ifdef _WIN32
	FileSystemInit();
	WtsInit();
#else
	CurlInit();
#endif
}

//#################################################################################################
void CoreLibFree(void)
{
#ifndef _WIN32
	CurlFree();
#endif

	CRandom::Free();
#ifdef _DEBUG
	DiagnosticFree();
#endif
	ExceptionFree();
}

NS_END
