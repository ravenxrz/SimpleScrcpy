/**
 * @file net.h
 * @author raven (zhang.xingrui@foxmail.com)
 * @brief 网络通信
 * @version 0.1
 * @date 2020-06-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef NET_H
#define NET_H

#include <SDL2/SDL_platform.h>
#include <SDL2/SDL_stdinc.h>
#include <sys/socket.h>
#define INVALID_SOCKET -1
typedef int socket_t;

/**
 * @brief 网络初始化(目前是do nothing)
 * 
 * @return SDL_bool true,成功
 *                  false,失败
 */
SDL_bool net_init(void);

/**
 * @brief net资源释放(目前是do nothing)
 * 
 */
void net_cleanup(void);
/**
 * @brief 网络监听
 * 
 * @param addr 绑定地址
 * @param port 绑定端口
 * @param backlog 最多listern多少个客户端
 * @return socket_t 
 */
socket_t net_listen(Uint32 addr,Uint16 port, int backlog);

/**
 * @brief 接收客户端连接
 * 
 * @param server_socekt server socket
 * @return socket_t 客户端socket
 */
socket_t net_accept(socket_t server_socekt);

ssize_t net_recv(socket_t socket, void *buf, size_t len);
/**
 * @brief net_recv_all版本和net_recv的区别在于, net_recv_all至少等待收到len bytes后
 *        才返回
 * 
 * @param socket 
 * @param buf 
 * @param len 
 * @return ssize_t 
 */
ssize_t net_recv_all(socket_t socket, void *buf, size_t len);
ssize_t net_send(socket_t socket, void *buf, size_t len);
ssize_t net_send_all(socket_t socket, void *buf, size_t len);
/**
 * @brief net shudown
 *  
 * 
 * @param socket 
 * @param how 
 *          1. SHUT_RD (read) 关闭socket上更多的read
 *          2. SHUT_WR(write) 关闭socket上的更多的write
 *          3. SHUT_RDWR(both) both
 * @return SDL_bool 
 */
SDL_bool net_shutdown(socket_t socket, int how);
SDL_bool net_close(socket_t socket);

#endif // !NET_H