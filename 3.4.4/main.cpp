
#define ACE_NTRACE 0
#include "ace/Log_Msg.h"
#include "ace/streams.h"

int ACE_MAIN(int, ACE_TCHAR *argv[])
{
	ACE_LOG_MSG->open( argv[0]);
	ACE_TRACE(ACE_TEXT("main"));

	ACE_OSTREAM_TYPE *output =
		new std::ofstream ("ostream.output.test");

	ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%IThis will go to STDERR\n")));

	ACE_LOG_MSG->open
		(argv[0], ACE_Log_Msg::SYSLOG, ACE_TEXT ("syslogTest"));
	ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);
	ACE_DEBUG
		((LM_DEBUG, ACE_TEXT ("%IThis will goes to STDERR & SYSLOG\n")));

	ACE_LOG_MSG->msg_ostream(output, 0);
	ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
	ACE_DEBUG
		((LM_DEBUG,
		   ACE_TEXT ("%IThis will go to STDERR, ")
		   ACE_TEXT ("syslog & an ostream\n")));

	ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
	delete output;
				

	ACE_DEBUG((LM_INFO, ACE_TEXT("%IGoodnignt\n")));
	
	return 0;
}

void foo(void)
{
	ACE_TRACE(ACE_TEXT("FOO"));
	ACE_DEBUG((LM_INFO, ACE_TEXT("%IHowday Pardner\n")));
}
