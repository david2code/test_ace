
#define ACE_NTRACE 0
#include "ace/Log_Msg.h"
void foo(void);
int ACE_MAIN(int, ACE_TCHAR *argv[])
{
	ACE_TRACE(ACE_TEXT("main"));

	ACE_DEBUG((LM_INFO, ACE_TEXT("%IHi Mom\n")));

	ACE_LOG_MSG->open(
		argv[0], ACE_Log_Msg::SYSLOG, ACE_TEXT ("syslogText"));
	
	foo();

	ACE_LOG_MSG->open( argv[0]);
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IGoodnignt\n")));
	
	return 0;
}

void foo(void)
{
	ACE_TRACE(ACE_TEXT("FOO"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IHowday Pardner\n")));
}
