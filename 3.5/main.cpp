
#include "ace/Log_Msg.h"
#include "Callback.h"

int ACE_MAIN(int, ACE_TCHAR *argv[])
{
	Callback *callback = new Callback;
	
	ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);
	ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);
	ACE_LOG_MSG->msg_callback (callback);

	ACE_TRACE (ACE_TEXT("MAIN"));
	
	ACE_DEBUG
		((LM_DEBUG, ACE_TEXT ("%IHi mom\n")));

	ACE_DEBUG((LM_INFO, ACE_TEXT("%IGoodnignt\n")));
	
	return 0;
}

void foo(void)
{
	ACE_TRACE(ACE_TEXT("FOO"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IHowday Pardner\n")));
}
