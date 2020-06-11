//
// Created by raven on 6/11/20.
//

#ifndef PHONEMIRROR_DEVICE_H
#define PHONEMIRROR_DEVICE_H

#include <SDL2/SDL_stdinc.h>

#include "common.h"
#include "net.h"

#define DEVICE_NAME_FILED_LENGTH 64

SDL_bool device_read_info(socket_t device_socket, char *name, struct size *frame_size);

#endif //PHONEMIRROR_DEVICE_H
