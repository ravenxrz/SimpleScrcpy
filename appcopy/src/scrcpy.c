/**
 * @file scrcpy.c
 * @author raven (zhang.xingrui@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-06-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "scrcpy.h"
#include "server.h"
#include "screen.h"
#include "frames.h"


static struct server server = SERVER_INITIALIZER;
static struct screen screen = SCREEN_INITIALIZER;
static struct frames frames;


SDL_bool scrcpy(Uint16 local_port, Uint16 max_size, Uint32 bit_rate)
{
    //  启动server 包括:
    // 1. push Jar file into the device
    // 2. 本机开启socket监听(on local port),等待device连接
    // 3. 启动device 进程
    if(!server_start(&server, local_port,max_size, bit_rate))
    {
        // 启动失败
        return SDL_FALSE;
    }

    SDL_bool ret = SDL_TRUE;
    
    // 初始化sdl
    if(!sdl_init_and_configure())
    {
        ret = SDL_FALSE;
        goto finally_destroy_server;
    }

    // 等待device端连接
#define SERVER_CONNECT_TIMEOUT_MS 2000
    socket_t device_socket = server_connect_to(&server, SERVER_CONNECT_TIMEOUT_MS);
    if(device_socket == INVALID_SOCKET)
    {
        server_stop(&server);
        ret = SDL_FALSE;
        goto finally_destroy_server;
    }
    
    // 连接后,开始正式工作

finally_destroy_server:
    server_destroy(&server);
}