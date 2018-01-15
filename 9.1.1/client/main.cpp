
#include "ace/INET_Addr.h"
#include "ace/OS.h"
#include "ace/SOCK_Dgram.h"
#include "ace/Log_Msg.h"

#   define ACE_static_cast(TYPE, EXPR)                                 ((TYPE) (EXPR))
#   define ACE_reinterpret_cast(TYPE, EXPR)                            ((TYPE) (EXPR))
#   define ACE_const_cast(TYPE, EXPR)                                  ((TYPE) (EXPR))
int send_unicast (const ACE_INET_Addr &to)
{
    const char *message = "this is the message!\n";
    ACE_INET_Addr my_addr (ACE_static_cast (u_short, 10101));
    ACE_SOCK_Dgram udp (my_addr);

    ssize_t sent = udp.send(message,
                            ACE_OS_String::strlen(message) + 1,
                            to);

    udp.close();
    if (sent == -1)
    {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                            ACE_TEXT ("send")), -1);
    }

    return 0;
}
int ACE_MAIN(int argc, ACE_TCHAR *argv[])
{
	ACE_INET_Addr srvr(10102, ACE_LOCALHOST);
    send_unicast(srvr);
    return 0;
}

