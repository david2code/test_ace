#include "ace/streams.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Callback.h"
#include "ace/Log_Record.h"

class Callback : public ACE_Log_Msg_Callback
{

public:
    void log (ACE_Log_Record &log_record) 
    {
        log_record.print (ACE_TEXT ("1"), 0, cerr);
        log_record.print (ACE_TEXT ("2"), ACE_Log_Msg::VERBOSE, cerr);
    }
};

