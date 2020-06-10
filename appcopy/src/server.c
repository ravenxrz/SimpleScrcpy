/**
 * @file server.c
 * @author raven (zhang.xingrui@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-06-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "server.h"

#include <error.h>
#include <inttypes.h>
#include <stdint.h>
#include <SDL2/SDL_assert.h>

#include "config.h"
#include "log.h"
#include "net.h"

#define SOCKET_NAME "scrcpy"

#ifdef OVERRIDE_SERVER_PATH
#define DEFAULT_SERVER_PATH OVERRIDE_SERVER_PATH
#endif 

/**
 * @brief Get the server path 
 * 
 * @return server path
 */
static const char *get_server_path(void)
{
    const char *server_path = getenv("SCRCPY_SERVER_PATH");
    if(!server_path)
    {
        server_path = DEFAULT_SERVER_PATH;
    }
    return server_path;
}

static SDL_bool push_server(void)
{
    process_t process = adb_push(get_server_path(), DEFAULT_DEVICE_PATH);
    return process_check_success(process, "adb push");
}

static SDL_bool remove_server(void)
{
    process_t process = adb_remove_path(DEFAULT_DEVICE_PATH);
    return process_check_success(process, "adb shell rm");
}

static SDL_bool enable_tunnel( Uint16 local_port) {
    process_t process = adb_reverse( SOCKET_NAME, local_port);
    return process_check_success(process, "adb reverse");
}

static SDL_bool disable_tunnel() {
    process_t process = adb_reverse_remove(SOCKET_NAME);
    return process_check_success(process, "adb reverse --remove");
}

static socket_t listen_on_port(Uint16 port) {
#define IPV4_LOCALHOST 0x7F000001
    return net_listen(IPV4_LOCALHOST, port, 1);
}

static process_t execute_server(Uint16 max_size, Uint32 bit_rate) {
    char max_size_string[6];
    char bit_rate_string[11];
    sprintf(max_size_string, "%"PRIu16, max_size);
    sprintf(bit_rate_string, "%"PRIu32, bit_rate);
    const char *const cmd[] = {
        "shell",
        "CLASSPATH=/data/local/tmp/scrcpy-server.jar",
        "app_process",
        "/", // unused
        "com.genymobile.scrcpy.Server",
        max_size_string,
        bit_rate_string,
    };
    return adb_execute(cmd, sizeof(cmd) / sizeof(cmd[0]));
}

static void close_socket(socket_t *socket)
{
    SDL_assert(*socket != INVALID_SOCKET);
    net_shutdown(*socket, SHUT_RDWR);
    if(!net_close(*socket))
    {
        LOGW("Cannot close socket");
        return;
    }
    *socket = INVALID_SOCKET;
}

void server_init(struct server *server)
{
    *server = (struct server)SERVER_ININTIALIZER;
}

SDL_bool server_start(struct server *server, Uint16 local_port, Uint16 max_size, Uint32 bit_rate)
{
    // 1. push jar file into the phone
    if(!push_server())
    {
        return SDL_FALSE;
    }
    server->server_copied_to_device = SDL_TRUE;

    // 2. enable the tunnel
    if(!!enable_tunnel(local_port))
    {
        return SDL_FALSE;
    }
    server->adb_reverse_enabled = SDL_TRUE;

    // 3. start socket on pc, watitting for the client to connect
    server->server_socket = listen_on_port(local_port);
    if(server->server_socket == INVALID_SOCKET)
    {
        LOGE("Could not listen on port %" PRId16, local_port);
        disable_tunnel();
        return SDL_FALSE;
    }

    // 4. 启动phone端进程
    server->process = execute_server(max_size, bit_rate);
    if(server->process == PROCESS_NONE)
    {
        close_socket(&server->server_socket);
        disable_tunnel();
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

socket_t server_connect_to(struct server *server, Uint32 timeout_ms)
{
    server->device_socket = net_accept(server->server_socket);
    if(server->device_socket == INVALID_SOCKET)
    {
        return INVALID_SOCKET;
    }

    close_socket(&server->server_socket);

    // remove jar file residing in phone
    remove_server();
    server->server_copied_to_device = SDL_FALSE;

    disable_tunnel();
    server->adb_reverse_enabled = SDL_FALSE;
    return server->device_socket;
}

void server_stop(struct server *server)
{
    SDL_assert(server->process != PROCESS_NONE);

    if(!cmd_terminate(server->process))
    {
        LOGW("Cannot termiate server");
    }

    cmd_simple_wait(server->process, NULL);
    LOGD("Server terminated");

    if(server->adb_reverse_enabled)
    {
        disable_tunnel();
    }

    if(server->server_copied_to_device)
    {
        remove_server();
    }

}

void server_destroy(struct server *server)
{
    if(server->server_socket != INVALID_SOCKET)
    {
        close_socket(&server->server_socket);
    }
    if(server->device_socket != INVALID_SOCKET)
    {
        close_socket(&server->device_socket);
    }
}
