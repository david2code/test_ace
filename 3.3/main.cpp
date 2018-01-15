#include "Trace.h"
void foo(void);
int ACE_MAIN(int, ACE_TCHAR *[])
{
	TRACE(ACE_TEXT("main"));
	MY_DEBUG(ACE_TEXT("%IHi Mom\n"));
	foo();
	MY_DEBUG(ACE_TEXT("%IGoodnignt\n"));
	
	TRACE_RETURN(0);
}

void foo(void)
{
	TRACE(ACE_TEXT("FOO"));
	MY_DEBUG(ACE_TEXT("%IHowday Pardner\n"));
	TRACE_RETURN_VOID();
}
