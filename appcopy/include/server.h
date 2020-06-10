/**
 * @file server.h
 * @author raven (zhang.xingrui@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-06-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef SERVER_H
#define SERVER_H

#include "command.h"
#include "net.h"

struct server
{
    process_t process;
    socket_t server_socket;
    socket_t device_socket;
    SDL_bool adb_reverse_enabled;
    SDL_bool server_copied_to_device;
};

#define SERVER_ININTIALIZER                   \
    {                                         \
        .process = PROCESS_NONE,              \
        .server_socket = INVALID_SOCKET,      \
        .device_socket = INVALID_SOCKET,      \
        .adb_reverse_enabled = SDL_FALSE,     \
        .server_copied_to_device = SDL_FALSE, \
    }

/**
 * @brief init default values
 * 
 * @param server 
 */
void server_init(struct server *server);

/**
 * @brief start the server, 包括:
 * 1. push jar file 到 device中
 * 2. enable tunnel
 * 3. start socket
 * 
 * @param server 
 * @param local_port 
 * @param max_size 
 * @param bit_rate 
 * @return SDL_bool 
 */
SDL_bool server_start(struct server *server, Uint16 local_port, Uint16 max_size, Uint32 bit_rate);

/**
 * @brief block 知道与deivce建立连接
 * 
 * @param server 
 * @param timeout_ms 
 * @return socket_t 
 */
socket_t server_connect_to(struct server *server, Uint32 timeout_ms);

/**
 * @brief disconenct 并且 kill 掉server 进程
 * 
 * @param server 
 */
void server_stop(struct server *server);

/**
 * @brief close 并且 release sockets
 * 
 * @param server 
 */
void server_destroy(struct server *server);

#endif // !SERVER_H