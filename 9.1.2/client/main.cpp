
#include "ace/INET_Addr.h"
#include "ace/OS.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/Log_Msg.h"

#   define ACE_static_cast(TYPE, EXPR)                                 ((TYPE) (EXPR))
#   define ACE_reinterpret_cast(TYPE, EXPR)                            ((TYPE) (EXPR))
#   define ACE_const_cast(TYPE, EXPR)                                  ((TYPE) (EXPR))

int send_broadcast (u_short to_port)
{
    const char *message = "this is the message!\n";
    ACE_INET_Addr my_addr (ACE_static_cast (u_short, 10101));
    ACE_SOCK_Dgram_Bcast udp (my_addr);

    ssize_t sent = udp.send(message,
                            ACE_OS_String::strlen(message) + 1,
                            to_port);

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
    send_broadcast(10102);
    return 0;
}

