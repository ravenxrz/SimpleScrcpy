#include "scrcpy.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libavformat/avformat.h>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include "command.h"
#include "common.h"
#include "decoder.h"
#include "device.h"
#include "events.h"
#include "frames.h"
#include "fpscounter.h"
#include "log.h"
#include "lockutil.h"
#include "net.h"
#include "screen.h"
#include "server.h"
#include "tinyxpm.h"

static struct server server = SERVER_INITIALIZER;
static struct screen screen = SCREEN_INITIALIZER;
static struct frames frames;
static struct decoder decoder;

static void event_loop(void) {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        switch (event.type) {
            case EVENT_DECODER_STOPPED:
                LOGD("Video decoder stopped");
                return;
            case SDL_QUIT:
                LOGD("User requested to quit");
                return;
            case EVENT_NEW_FRAME:
                if (!screen.has_frame) {
                    screen.has_frame = SDL_TRUE;
                    // this is the very first frame, show the window
                    screen_show_window(&screen);
                }
                if (!screen_update_frame(&screen, &frames)) {
                    return;
                }
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                case SDL_WINDOWEVENT_EXPOSED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    screen_render(&screen);
                    break;
                }
                break;
        }
    }
}

SDL_bool scrcpy(const char *serial, Uint16 local_port, Uint16 max_size, Uint32 bit_rate) {
    if (!server_start(&server, serial, local_port, max_size, bit_rate)) {
        return SDL_FALSE;
    }

    SDL_bool ret = SDL_TRUE;

    if (!sdl_init_and_configure()) {
        ret = SDL_FALSE;
        goto finally_destroy_server;
    }

    // SDL initialization replace the signal handler for SIGTERM, so Ctrl+C is
    // managed by the event loop. This blocking call blocks the event loop, so
    // timeout the connection not to block indefinitely in case of SIGTERM.
#define SERVER_CONNECT_TIMEOUT_MS 2000
    socket_t device_socket = server_connect_to(&server, serial, SERVER_CONNECT_TIMEOUT_MS);
    if (device_socket == INVALID_SOCKET) {
        server_stop(&server, serial);
        ret = SDL_FALSE;
        goto finally_destroy_server;
    }

    char device_name[DEVICE_NAME_FIELD_LENGTH];
    struct size frame_size;

    // screenrecord does not send frames when the screen content does not change
    // therefore, we transmit the screen size before the video stream, to be able
    // to init the window immediately
    if (!device_read_info(device_socket, device_name, &frame_size)) {
        server_stop(&server, serial);
        ret = SDL_FALSE;
        goto finally_destroy_server;
    }

    if (!frames_init(&frames)) {
        server_stop(&server, serial);
        ret = SDL_FALSE;
        goto finally_destroy_server;
    }

    decoder_init(&decoder, &frames, device_socket);

    // now we consumed the header values, the socket receives the video stream
    // start the decoder
    if (!decoder_start(&decoder)) {
        ret = SDL_FALSE;
        server_stop(&server, serial);
        goto finally_destroy_frames;
    }

    if (!screen_init_rendering(&screen, device_name, frame_size)) {
        ret = SDL_FALSE;
        goto finally_stop_decoder;
    }

    event_loop();

    LOGD("quit...");
    screen_destroy(&screen);
finally_stop_decoder:
    decoder_stop(&decoder);
    // stop the server before decoder_join() to wake up the decoder
    server_stop(&server, serial);
    decoder_join(&decoder);
finally_destroy_frames:
    frames_destroy(&frames);
finally_destroy_server:
    server_destroy(&server);

    return ret;
}
