
#include "ace/Log_Msg.h"

int ACE_MAIN(int, ACE_TCHAR *argv[])
{
	ACE_LOG_MSG->open(argv[0],
					  ACE_Log_Msg::LOGGER,
					  ACE_DEFAULT_LOGGER_KEY);
	

	ACE_TRACE (ACE_TEXT("MAIN"));
	
	ACE_DEBUG
		((LM_DEBUG, ACE_TEXT ("%IHi mom\n")));

	ACE_DEBUG((LM_INFO, ACE_TEXT("%IGoodnignt\n")));
	
	return 0;
}

