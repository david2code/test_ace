#include "ace/config-all.h"
#include "ace/Auto_Ptr.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/Message_Block.h"
#include "ace/Message_Queue.h"
//#include "ace/Task.h"
#include "ace/SString.h"
#include "ace/OS.h"
#include "ace/streams.h"
#include "ace/Synch.h"
#include "ace/Signal.h"

#   define ACE_static_cast(TYPE, EXPR)                                 ((TYPE) (EXPR))
#   define ACE_reinterpret_cast(TYPE, EXPR)                            ((TYPE) (EXPR))
#   define ACE_const_cast(TYPE, EXPR)                                  ((TYPE) (EXPR))
#if 1
class ClientService : public ACE_Event_Handler
{
public:
    ACE_SOCK_Stream &peer (void) {return this->sock_; }

    int open(void);

    virtual ACE_HANDLE get_handle (void) const
    {return this->sock_.get_handle();}

    virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
    virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE);

    virtual int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask close_mask);
protected:
    ACE_SOCK_Stream sock_;
    ACE_Message_Queue<ACE_NULL_SYNCH> output_queue_;
};

int ClientService::open(void)
{
    ACE_TCHAR peer_name[MAXHOSTNAMELEN];
    ACE_INET_Addr peer_addr;

    if (this->sock_.get_remote_addr(peer_addr) == 0 &&
        peer_addr.addr_to_string(peer_name, MAXHOSTNAMELEN) == 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) Connect from %s\n"),
                    peer_name));
    return this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK);
}

int ClientService::handle_input(ACE_HANDLE)
{
    const size_t INPUT_SIZE = 4096;
    char buffer[INPUT_SIZE];
    ssize_t recv_cnt, send_cnt;

    if ((recv_cnt = this->sock_.recv(buffer, sizeof(buffer))) <= 0)
    {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) Connection closed!\n")));
        return -1;
        
    }

    send_cnt = this->sock_.send(buffer, ACE_static_cast(size_t, recv_cnt));
    if (send_cnt == recv_cnt)
        return 0;

    if (send_cnt == -1 && ACE_OS::last_error() != EWOULDBLOCK)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("(%P|%t) %p\n"),
                           ACE_TEXT("SEND")),
                           0);

    if (send_cnt == -1)
        send_cnt = 0;

    ACE_Message_Block *mb;
    size_t remaining = ACE_static_cast(size_t, (recv_cnt - send_cnt));
    ACE_NEW_RETURN(mb, ACE_Message_Block(&buffer[send_cnt], remaining), -1);

    int output_off = this->output_queue_.is_empty();
    ACE_Time_Value nowait (ACE_OS::gettimeofday());
    if (this->output_queue_.enqueue_tail (mb, &nowait) == -1)
    {
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p; discarding data\n"),
                    ACE_TEXT ("enqueue failed")));

        mb->release();
        return 0;
    }

    if (output_off)
        return this->reactor()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
    return 0;
}

int ClientService::handle_output(ACE_HANDLE)
{
    ACE_Message_Block *mb;
    ACE_Time_Value nowait(ACE_OS::gettimeofday());
    while(0 == this->output_queue_.dequeue_head(mb, &nowait))
    {
        ssize_t send_cnt = this->sock_.send(mb->rd_ptr(), mb->length());
        if (send_cnt == -1)
        {
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("send")));
        }
        else
            mb->rd_ptr(ACE_static_cast(size_t, send_cnt));

        if (mb->length() > 0)
        {
            this->output_queue_.enqueue_head(mb);
            break;
        }

        mb->release();
    }

    return (this->output_queue_.is_empty()) ? -1 : 0;
}

int ClientService::handle_close(ACE_HANDLE, ACE_Reactor_Mask mask)
{
    if (mask == ACE_Event_Handler::WRITE_MASK)
        return 0;

    mask = ACE_Event_Handler::ALL_EVENTS_MASK |
           ACE_Event_Handler::DONT_CALL;
    this->reactor()->remove_handler(this, mask);
    this->sock_.close();
    this->output_queue_.flush();
    delete this;
    return 0;
}
#endif

#if 1
class ClientAcceptor : public ACE_Event_Handler
{
public:
    virtual ~ClientAcceptor ();

    int open(const ACE_INET_Addr &listen_addr);

    virtual ACE_HANDLE get_handle (void) const
    {return this->acceptor_.get_handle();}

    virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);

    virtual int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask close_mask);
protected:
    ACE_SOCK_Acceptor acceptor_;
};

int ClientAcceptor::open( const ACE_INET_Addr &listen_addr)
{
    if (this->acceptor_.open(listen_addr, 1) == -1)
    {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT("%p\n"),
                            ACE_TEXT("acceptor.open")),
                            -1);
        
    }

    return this->reactor()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK);
}

int ClientAcceptor::handle_input(ACE_HANDLE)
{
    ClientService *client;
    ACE_NEW_RETURN (client, ClientService, -1);
    auto_ptr<ClientService> p (client);

    if (this->acceptor_.accept(client->peer()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, 
                            ACE_TEXT("(%P|%t) %p\n")
                            ACE_TEXT("Failed to accept ")
                            ACE_TEXT("Client Connection ")),
                            -1);
    p.release();

    client->reactor(this->reactor());
    if (client->open() == -1)
        client->handle_close(ACE_INVALID_HANDLE, 0);

    return 0;
}

int ClientAcceptor::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
{
    if (this->acceptor_.get_handle() != ACE_INVALID_HANDLE)
    {
        ACE_Reactor_Mask m = ACE_Event_Handler::ACCEPT_MASK |
                             ACE_Event_Handler::DONT_CALL;

        this->reactor()->remove_handler (this, m);
        this->acceptor_.close();
    }

    return 0;
}

ClientAcceptor::~ClientAcceptor()
{
    this->handle_close(ACE_INVALID_HANDLE, 0);
}
#endif

#if 1
class LoopStopper : public ACE_Event_Handler
{
public:
    LoopStopper (int signum = SIGINT);

    virtual int handle_signal (int signum,
                                siginfo_t * = 0,
                                ucontext_t * = 0);
};

LoopStopper::LoopStopper(int signum)
{
    ACE_Reactor::instance()->register_handler(signum, this);
}

int LoopStopper::handle_signal(int, siginfo_t *, ucontext_t *)
{
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("(%P|%t) handle signal\n"),
                ACE_TEXT ("ints")));
    ACE_Reactor::instance()->end_reactor_event_loop();
    return 0;
}
#endif

#if 1

class LogSwitcher : public ACE_Event_Handler
{
public:
    LogSwitcher (int on_sig, int off_sig);

    virtual int handle_signal( int signum,
                                siginfo_t * = 0,
                                ucontext_t * = 0);
    virtual int handle_exception(ACE_HANDLE fd = ACE_INVALID_HANDLE);
private:
    LogSwitcher(){}

    int on_sig_;
    int off_sig_;
    int on_off_;
};

LogSwitcher::LogSwitcher(int on_sig, int off_sig) 
    : on_sig_(on_sig), off_sig_(off_sig)
{
    ACE_Sig_Set sigs;
    sigs.sig_add(on_sig);
    sigs.sig_add(off_sig);
    ACE_Reactor::instance()->register_handler(sigs, this);
}

int LogSwitcher::handle_signal(int signum, siginfo_t *, ucontext_t *)
{
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("(%P|%t) handle signal %d\n"),
                signum));
    if (signum == this->on_sig_ || signum == this->off_sig_)
    {
        this->on_off_ = signum == this->on_sig_;
        ACE_Reactor::instance()->notify(this);
    }

    return 0;
}

int LogSwitcher::handle_exception(ACE_HANDLE)
{
    if (this->on_off_)
    {
        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::SILENT);
    }
    else
    {
        ACE_LOG_MSG->set_flags(ACE_Log_Msg::SILENT);
    }
    return 0;
}
#endif

#if 1

class MyTimerHandler : public ACE_Event_Handler
{
public:
    int handle_timeout (const ACE_Time_Value &current_time,
                        const void * = 0)
    {
        time_t epoch = ((timespec_t)current_time).tv_sec;
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("Handle_timeout: %s\n"),
                    ACE_OS::ctime(&epoch)));

        return 0;
    }
};
#endif
#if 1
class TemperatureSensor
{
public:
    TemperatureSensor (const char *location)
        : location_(location),
          count_(0),
          temperature_(0.0)
    {}

    const char *location() const
    {
        return this->location_;
    }

    int querySensor(void)
    {
        return ++this->count_;
    }

    float temperature(void) const
    {
        return this->temperature_;
    }
private:
    const char *location_;
    int count_;
    float temperature_;
};

class TemperatureQueryHandler : public ACE_Event_Handler
{
public:
    TemperatureQueryHandler()
        : ACE_Event_Handler(),
          counter_(0),
          averageTemperature_(0.0)
    {}

    int handle_timeout (const ACE_Time_Value &current_time,
                        const void *arg)
    {
        time_t epoch = ((timespec_t)current_time).tv_sec;

        const TemperatureSensor *const_sensor =
            ACE_reinterpret_cast (const TemperatureSensor *, arg);

        TemperatureSensor *sensor =
            ACE_const_cast(TemperatureSensor *, const_sensor);

        int queryCount = sensor->querySensor();
        this->updateAverageTemperature(sensor);
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("%s\t")
                    ACE_TEXT ("%d/%d\t")
                    ACE_TEXT ("%0.2f/%0.2f\t")
                    ACE_TEXT ("%s\n"),
                    sensor->location(),
                    ++this->counter_,
                    queryCount,
                    this->averageTemperature_,
                    sensor->temperature(),
                    ACE_OS::ctime(&epoch)));

        return 0;
    }


private:
    void updateAverageTemperature (TemperatureSensor *sensor)
    {
    }

    int counter_;
    float averageTemperature_;
};

#endif

#if 1
class SigintHandler : public ACE_Event_Handler
{
public:
    SigintHandler (long timerId, int currentInterval)
        : ACE_Event_Handler(),
          timerId_(timerId),
          currentInterval_(currentInterval)
    {
    }

    int handle_signal (int signum,
                            siginfo_t * = 0,
                            ucontext_t * = 0)
    {
        if (signum == SIGINT)
        {
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("Resetting interval of timer ")
                        ACE_TEXT ("%d to %d \n"),
                        this->timerId_,
                        ++this->currentInterval_));

            ACE_Time_Value newInterval (this->currentInterval_);
            
            ACE_Reactor::instance()->reset_timer_interval(this->timerId_, newInterval);
        }
        else if(signum == SIGTSTP)
        {
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("Cancel the timer %d\n"),
                        this->timerId_));
            
            ACE_Reactor::instance()->cancel_timer(this->timerId_);
        }
        return 0;
    }

private:
    long timerId_;
    int currentInterval_;
};

#endif

int ACE_MAIN(int argc, ACE_TCHAR *argv[])
{
	//ACE_INET_Addr port_to_listen ("HAStatus");
	ACE_INET_Addr port_to_listen(50000, ACE_LOCALHOST);
	ClientAcceptor acceptor;

    acceptor.reactor(ACE_Reactor::instance());
	if (acceptor.open (port_to_listen) == -1)
		ACE_ERROR_RETURN ((LM_ERROR,
		                   ACE_TEXT("%p\n"),
		                   ACE_TEXT("acceptor.open")),
		                   100);
    //LoopStopper stopper;
    //LogSwitcher logswc(SIGINT, SIGTSTP);
    #if 0
    MyTimerHandler * timer = new MyTimerHandler();
    ACE_Time_Value initialDelay(3);
    ACE_Time_Value interval(5);
    ACE_Reactor::instance()->schedule_timer(timer,
                                            0,
                                            initialDelay,
                                            interval);
    
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("Timer ready\n")));
    #endif

    #if 0
    TemperatureQueryHandler *temperatureMonitor =
        new TemperatureQueryHandler();

    ACE_Time_Value initialDelay(3);
    ACE_Time_Value interval(5);
    TemperatureSensor *sensorOne =
        new TemperatureSensor("Kitchen");
    ACE_Reactor::instance()->schedule_timer(temperatureMonitor,
                                            sensorOne,
                                            initialDelay,
                                            interval);
    
    ACE_Time_Value initialDelay2(3);
    ACE_Time_Value interval2(3);
    TemperatureSensor *sensorTwo =
        new TemperatureSensor("foyer");
    ACE_Reactor::instance()->schedule_timer(temperatureMonitor,
                                            sensorTwo,
                                            initialDelay2,
                                            interval2);
    #endif

    #if 1
    MyTimerHandler * timer = new MyTimerHandler();
    ACE_Time_Value initialDelay(3);
    ACE_Time_Value interval(5);
    long timerId = ACE_Reactor::instance()->schedule_timer(timer,
                                            0,
                                            initialDelay,
                                            interval);

    SigintHandler *handleSigint = new SigintHandler(timerId, 5);
    
    ACE_Reactor::instance()->register_handler(SIGINT, handleSigint);
    ACE_Reactor::instance()->register_handler(SIGTSTP, handleSigint);
    #endif
    ACE_Reactor::instance()->run_reactor_event_loop();
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("Done!\n")));
	return 0;
}

