#ifndef __TRACE_H
#define __TRACE_H

#define ACE_NTRACE 0
#include "ace/Log_Msg.h"


#define DEBUG_PREFIX		ACE_TEXT ("DEBUG%I")
#define INFO_PREFIX		ACE_TEXT ("INFO%I")
#define NOTICE_PREFIX		ACE_TEXT ("NOTICE%I")
#define WARNING_PREFIX		ACE_TEXT ("WARNING%I")
#define ERROR_PREFIX		ACE_TEXT ("ERROR%I")
#define CRITICAL_PREFIX		ACE_TEXT ("CRITICAL%I")
#define ALERT_PREFIX		ACE_TEXT ("ALERT%I")
#define EMERGENCY_PREFIX		ACE_TEXT ("EMERGENCY%I")

#define MY_DEBUG(FMT, ...)\
	ACE_DEBUG((LM_DEBUG,\
	DEBUG_PREFIX FMT \
	__VA_ARGS__))
	
#define MY_INFO(FMT, ...)\
	ACE_DEBUG((LM_INFO,\
	INFO_PREFIX FMT \
	__VA_ARGS__))
	
#define MY_NOTICE(FMT, ...)\
	ACE_DEBUG((LM_NOTICE,\
	NOTICE_PREFIX FMT \
	__VA_ARGS__))
	
#define MY_WARNING(FMT, ...)\
	ACE_DEBUG((LM_WARNING,\
	WARNING_PREFIX FMT \
	__VA_ARGS__))
	
#define MY_ERROR(FMT, ...)\
	ACE_DEBUG((LM_ERROR,\
	ERROR_PREFIX FMT \
	__VA_ARGS__))
	
#define MY_CRITICAL(FMT, ...)\
	ACE_DEBUG((LM_CRITICAL,\
	CRITICAL_PREFIX FMT \
	__VA_ARGS__))
	
#define MY_ALERT(FMT, ...)\
	ACE_DEBUG((LM_ALERT,\
	ALERT_PREFIX FMT \
	__VA_ARGS__))
	
#define MY_EMERGENCY(FMT, ...)\
	ACE_DEBUG((LM_EMERGENCY,\
	EMERGENCY_PREFIX FMT \
	__VA_ARGS__))

class Trace
{
public:
	Trace (const ACE_TCHAR *prefix,
           const ACE_TCHAR *name,
           int line,
           const ACE_TCHAR *file)
	{
        this->prefix_ = prefix;
        this->name_     = name;
        this->line_     = line;
        this->file_     = file;

        ACE_Log_Msg *lm = ACE_LOG_MSG;

        if (lm->tracing_enabled()
            && lm->trace_active() == 0)
        {
            lm->trace_active(1);
            ACE_DEBUG
                ((LM_TRACE,
                  ACE_TEXT ("%s%*s(%t) calling %s in file '%s'")
                  ACE_TEXT (" on line %d\n"),
                  this->prefix_,
                  Trace::nesting_indent_ * lm->inc(),
                  ACE_TEXT (" "),
                  this->name_,
                  this->file_,
                  this->line_));
            lm->trace_active(0);
        }
	}

    void setLine (int line)
    {
        this->line_ = line;
    }

    ~Trace (void)
    {
        ACE_Log_Msg *lm = ACE_LOG_MSG;
        if (lm->tracing_enabled()
            && lm->trace_active() == 0)
        {
            lm->trace_active(1);
            ACE_DEBUG
                ((LM_TRACE,
                  ACE_TEXT ("%s%*s(%t) leaving %s in file '%s'")
                  ACE_TEXT (" on line %d\n"),
                  this->prefix_,
                  Trace::nesting_indent_ * lm->dec(),
                  ACE_TEXT (" "),
                  this->name_,
                  this->file_,
                  this->line_));
            lm->trace_active(0);
                  
        }
    }
		

private:		
    enum { nesting_indent_ = 3};

    const ACE_TCHAR *prefix_;
    const ACE_TCHAR *name_;
    const ACE_TCHAR *file_;
    int line_;
};


#define TRACE_PREFIX    ACE_TEXT ("TRACE")

#if (ACE_NTRACE == 1)
#   define TRACE(X)
#   define TRACE_RETURN(V)
#   define TRACE_RETURN_VOID()
#else
#   define TRACE(X)         \
           Trace ___ (TRACE_PREFIX,\
                      ACE_TEXT (X),\
                      __LINE__,     \
                      ACE_TEXT (__FILE__))

#   define TRACE_RETURN(V)  \
           do { ___.setLine(__LINE__); return V; } while (0)
#   define TRACE_RETURN_VOID()  \
           do { ___.setLine(__LINE__); } while(0)
#endif

#endif