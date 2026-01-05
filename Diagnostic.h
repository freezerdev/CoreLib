#pragma once

NS_BEGIN

#ifdef _DEBUG

void DiagnosticInit(void);
void DiagnosticFree(void);
void DiagnosticAssert(const bool bCondition, PCNSTR szFile, const long nLine);
void DiagnosticMessage(PCNSTR szMsg);

#ifdef _WIN32
#define DIAG_WIDE_(s)	L ## s
#define DIAG_WIDE(s)	DIAG_WIDE_(s)
#define Assert(cond)	DiagnosticAssert((cond), DIAG_WIDE(__FILE__), __LINE__)
#else
#define Assert(cond)	DiagnosticAssert((cond), __FILE__, __LINE__)
#endif

#define Verify(cond)	Assert(cond)

#else

#define Assert(cond)	((void)0)
#define Verify(cond)	((void)(cond))

#endif

NS_END
