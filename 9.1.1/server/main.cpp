#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"
#include "ace/SOCK_Dgram.h"

#   define ACE_static_cast(TYPE, EXPR)                                 ((TYPE) (EXPR))
#   define ACE_reinterpret_cast(TYPE, EXPR)                            ((TYPE) (EXPR))
#   define ACE_const_cast(TYPE, EXPR)                                  ((TYPE) (EXPR))

void echo_dgram(void)
{
    ACE_INET_Addr my_addr (ACE_static_cast (u_short, 10102));
    ACE_INET_Addr your_addr;
    ACE_SOCK_Dgram udp (my_addr);

    char buff[BUFSIZ];
    size_t buflen = sizeof(buff);
    ssize_t recv_cnt = udp.recv(buff, buflen, your_addr);
    if (recv_cnt > 0)
    {
        ACE_DEBUG ((LM_ERROR,
            ACE_TEXT ("(%P|%t) recv: %s\n"),
            buff));
        udp.send(buff, ACE_static_cast(size_t, recv_cnt), your_addr);
    }

    udp.close();
    return;
}

int ACE_MAIN(int argc, ACE_TCHAR *argv[])
{
    echo_dgram();
	return 0;
}

