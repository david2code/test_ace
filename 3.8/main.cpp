
#include "ace/Log_Msg.h"
#include "ace/Service_Config.h"

int ACE_MAIN(int argc, ACE_TCHAR *argv[])
{
	if (ACE_Service_Config::open(argc,
								  argv,
								  ACE_DEFAULT_LOGGER_KEY,
								  1,
								  0,
								  1) < 0)
	{
		ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
							ACE_TEXT ("Service Config open")),
							1);
		
	}
	

	ACE_TRACE (ACE_TEXT("MAIN"));
	
	ACE_DEBUG
		((LM_DEBUG, ACE_TEXT ("%IHi mom\n")));
	ACE_DEBUG
		((LM_NOTICE, ACE_TEXT ("%IHi NOtice\n")));

	ACE_DEBUG((LM_INFO, ACE_TEXT("%IGoodnignt\n")));

	ACE_INT32 i;
	for (i = 0; i < 30000; i++)
	{
		ACE_DEBUG((LM_INFO, ACE_TEXT("%IHELLO, my running numbe is %d\n"), i));
	}
	return 0;
}

