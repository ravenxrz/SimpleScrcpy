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
#include "device.h"
#include "scrcpy.h"
#include "server.h"
#include "screen.h"
#include "frames.h"
#include "decoder.h"
#include "log.h"
#include "events.h"

static struct server server = SERVER_INITIALIZER;
static struct screen screen = SCREEN_INITIALIZER;
static struct frames frames;
static struct decoder decoder;


static void event_loop(void)
{
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        switch (event.type) {
            case EVENT_DECODER_STOPPED:
                LOGD("Video decoder stoped");
                return;
            case SDL_QUIT:
                LOGD("User requested to quit");
                return;
            case EVENT_NEW_FRAME:
                if (!screen.has_frame) {
                    screen.has_frame = SDL_TRUE;
                    // first frame to show
                    screen_show_window(&screen);
                }
                if (!screen_update_frame(&screen, &frames)) {
                    return;
                }
            case SDL_WINDOWEVENT:
                switch(event.window.event)
                {
                    case SDL_WINDOWEVENT_EXPOSED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        screen_render(&screen);
                        break;
                }
                break;
        }
    }
}


SDL_bool scrcpy(Uint16 local_port, Uint16 max_size, Uint32 bit_rate)
{
    //  启动server 包括:
    // 1. push Jar file into the device
    // 2. 本机开启socket监听(on local port),等待device连接
    // 3. 启动device 进程
    if (!server_start(&server, local_port, max_size, bit_rate)) {
        // 启动失败
        return SDL_FALSE;
    }
    LOGD("server start success");
    
    SDL_bool ret = SDL_TRUE;
    
    // 初始化sdl
    if (!sdl_init_and_configure()) {
        ret = SDL_FALSE;
        goto finally_destroy_server;
    }
    LOGD("sdl init success");
    
    // 等待device端连接
#define SERVER_CONNECT_TIMEOUT_MS 2000
    socket_t device_socket = server_connect_to(&server, SERVER_CONNECT_TIMEOUT_MS);
    LOGD("device has connected to pc");
    if (device_socket == INVALID_SOCKET) {
        server_stop(&server);
        ret = SDL_FALSE;
        goto finally_destroy_server;
    }
    
    // 连接后,开始正式工作
    char device_name[DEVICE_NAME_FILED_LENGTH];
    struct size frame_size;
    
    // 读取设备info
    if (!device_read_info(device_socket, device_name, &frame_size)) {
        server_stop(&server);
        ret = SDL_FALSE;
        goto finally_destroy_server;
    }
    LOGD("read device info success");
    
    // 初始化frame
    if (!frames_init(&frames)) {
        server_stop(&server);
        ret = SDL_FALSE;
        goto finally_destroy_server;
    }
    LOGD("init frames success");
    
    // 从device中接收stream,并解码
    decoder_init(&decoder, &frames, device_socket);
    
    if (!decoder_start(&decoder)) {
        ret = SDL_FALSE;
        server_stop(&server);
        goto finally_destroy_frames;
    }
    LOGD("decoder thread start");
    
    if (!(screen_init_rendering(&screen, device_name, frame_size))) {
        ret = SDL_FALSE;
        goto finally_stop_decoder;
    }
    LOGD("screen init success");
    
    event_loop();
    
    LOGD("quit...");
    screen_destroy(&screen);
    finally_stop_decoder:
    decoder_stop(&decoder);
    server_stop(&server);
    decoder_join(&decoder);
    finally_destroy_frames:
    frames_destroy(&frames);
    finally_destroy_server:
    server_destroy(&server);
    
    return ret;
}