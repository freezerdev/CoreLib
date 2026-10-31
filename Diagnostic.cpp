#include "Base.h"
#include "Diagnostic.h"
#ifdef _DEBUG
#include "TimeUtils.h"
#include "File.h"
#include "FilePathUtils.h"
#include <mutex>

NS_BEGIN

static std::unique_ptr<std::mutex> g_pDiagnosticMutex;

//#################################################################################################
void DiagnosticInit(void)
{
	if(!g_pDiagnosticMutex)
		g_pDiagnosticMutex = std::make_unique<std::mutex>();
}

//#################################################################################################
void DiagnosticFree(void)
{
	g_pDiagnosticMutex.reset();
}

//#################################################################################################
void DiagnosticAssert(const bool bCondition, PCNSTR szFile, const long nLine)
{
	if(!bCondition)
	{
		CStr strMsg(CStr::EFormatType::Format, _N("[{0}] Assertion failed.  {1} ({2}){3}"), GetFormattedTime(), szFile, nLine, g_szEol);
		if(g_pDiagnosticMutex)
		{
			CFilePath pathFile = GetExecutablePath();
			pathFile.SetExtension(_N("txt"));

			NHANDLE hFile = INVALID_NHANDLE;

			CFile file;
			std::lock_guard<std::mutex> lock(*g_pDiagnosticMutex);
			if(file.Create(pathFile.Get(), CFile::EFileMode::AlwaysReadWrite) == FW_NO_ERROR)
			{
				size_t nBytesWritten;
				file.SetPosition(0, CFile::EFileSeek::End);		// Move to the end so we append
				file.Write((PCBYTE)(PCNSTR)strMsg, strMsg.GetSize(), nBytesWritten);
			}
		}

		strMsg.Delete(strMsg.GetLength() - g_nEolLen, CStr::end);
#ifdef _WIN32
		strMsg.DebugPrint();
		std::cout << strMsg.AsUtf8() << std::endl;
#else
		std::cout << strMsg << std::endl;
#endif
	}
}

//#################################################################################################
void DiagnosticMessage(PCNSTR szMsg)
{
	CStr strMsg(CStr::EFormatType::Format, _N("[{0}] {1}{2}"), GetFormattedTime(), szMsg, g_szEol);
	if(g_pDiagnosticMutex)
	{
		CFilePath pathFile = GetExecutablePath();
		pathFile.SetExtension(_N("txt"));

		NHANDLE hFile = INVALID_NHANDLE;

		CFile file;
		std::lock_guard<std::mutex> lock(*g_pDiagnosticMutex);
		if(file.Create(pathFile.Get(), CFile::EFileMode::AlwaysReadWrite) == FW_NO_ERROR)
		{
			size_t nBytesWritten;
			file.SetPosition(0, CFile::EFileSeek::End);		// Move to the end so we append
			file.Write((PCBYTE)(PCNSTR)strMsg, strMsg.GetSize(), nBytesWritten);
		}
	}

	strMsg.Delete(strMsg.GetLength() - g_nEolLen, CStr::end);
#ifdef _WIN32
	strMsg.DebugPrint();
	std::cout << strMsg.AsUtf8() << std::endl;
#else
	std::cout << strMsg << std::endl;
#endif
}

NS_END

#endif
