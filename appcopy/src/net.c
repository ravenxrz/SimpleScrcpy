#include "net.h"

#include <stdio.h>
#include "log.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET_ERROR -1
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

SDL_bool net_init(void)
{
    // do nothing
    return SDL_TRUE;
}

void net_cleanup(void)
{
    // do nothing
}

socket_t net_listen(Uint32 addr, Uint16 port, int backlog)
{
    socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        perror("socket");
        return INVALID_SOCKET;
    }

    int reuse = 1;
    if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR , (const void *) & reuse, sizeof(reuse)) == -1)
    {
        perror("setsockopt(SO_REUSERADDR");
    }

    SOCKADDR_IN sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(addr);
    sin.sin_port = htons(port);

    // bind sock和addr:port
    if(bind(sock,(SOCKADDR *)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        perror("bind");
        return INVALID_SOCKET;
    }

    if(listen(sock, backlog) == SOCKET_ERROR)
    {
        perror("listen");
        return INVALID_SOCKET;
    }

    return sock;
}

socket_t net_accept(socket_t server_socekt)
{
    SOCKADDR_IN csin;
    socklen_t sin_size = sizeof(csin);
    return accept(server_socekt, (SOCKADDR *)&csin, &sin_size);
}

ssize_t net_recv(socket_t socket, void *buf, size_t len)
{
    return recv(socket, buf, len, 0);
}

ssize_t net_recv_all(socket_t socket, void *buf, size_t len)
{
    return recv(socket, buf, len, MSG_WAITALL);
}

ssize_t net_send(socket_t socket, void *buf, size_t len)
{
    return send(socket, buf, len, 0);
}

ssize_t net_send_all(socket_t socket, void *buf, size_t len)
{
    ssize_t w;
    while(len > 0)
    {
        w = send(socket, buf, len, 0);
        len -= w;
        buf += w;   // 指针移动到下一个需要发送的内容的首地址
    }
    return w;   // TODO: 源代码这里是否有点问题
}

SDL_bool net_shutdown(socket_t socket, int how)
{
    return !shutdown(socket, how);
}

SDL_bool net_close(socket_t socket)
{
    return !close(socket);
}
