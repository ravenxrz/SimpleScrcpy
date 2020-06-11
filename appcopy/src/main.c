#include <stdio.h>
#include "command.h"
#include "server.h"
#include "config.h"
#include <SDL2/SDL.h>


int main(int argc, char const *argv[])
{
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

    struct server server;
    server_init(&server);
    server_start(&server, 2555, DEFAULT_MAX_SIZE, DEFAULT_BIT_RATE);
    return 0;
}
