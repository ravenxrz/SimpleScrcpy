//
// Created by raven on 6/11/20.
//

#ifndef PHONEMIRROR_DECODER_H
#define PHONEMIRROR_DECODER_H

#include "net.h"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_thread.h>

struct frames;
struct decoder
{
    struct frames *frames;
    socket_t video_socket;
    SDL_Thread  *thread;
    SDL_mutex  *mutex;
};

void decoder_init(struct decoder *decoder, struct frames *frames, socket_t video_socket);
SDL_bool decoder_start(struct decoder *decoder);
void decoder_stop(struct decoder *decoder);
void decoder_join(struct decoder *decoder);


#endif //PHONEMIRROR_DECODER_H
