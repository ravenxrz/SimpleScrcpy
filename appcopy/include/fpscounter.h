/**
 * @file fpscounter.h
 * @author raven (zhang.xingrui@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-06-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <SDL2/SDL_stdinc.h>
#include "config.h"

struct fps_counter{
    SDL_bool started;
    Uint32 slice_start;
    int nr_rendered;
#ifdef SKIP_FRAMES
    int nr_skipped;
#endif // SKIP_FRAMES
};

void fps_counter_init(struct fps_counter *counter);
void fps_counter_start(struct fps_counter *counter);
void fps_counter_stop(struct fps_counter *counter);

void fps_counter_add_rendered_frame(struct fps_counter *counter);

#ifdef SKIP_FRAMES
void fps_counter_add_skipped_frame(struct fps_counter *counter);
#endif // SKIP_FRAMES

#endif // FPSCOUNTER_H