/**
 * @file fpscounter.c
 * @author raven (zhang.xingrui@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-06-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "fpscounter.h"

#include <SDL2/SDL_timer.h>
#include "log.h"

void fps_counter_init(struct fps_counter *counter)
{
    counter->started = SDL_FALSE;
    // 其余字段只有在started为true时才生效,所以无需初始化(不过个人建议还是初始化)
}

void fps_counter_start(struct fps_counter *counter)
{
    counter->started = SDL_TRUE;
    counter->slice_start = SDL_GetTicks();
    counter->nr_rendered = 0;
#ifdef SKIP_FRAMES
    counter->nr_skipped = 0;
#endif //SKIP_FRAMES
}

void fps_counter_stop(struct fps_counter *counter)
{
    counter->started = SDL_FALSE;
}

static void display_fps(struct fps_counter *counter)
{
#ifdef SKIP_FRAMES
    if(counter->nr_skipped)
    {
        LOGI("%d fps (+%d frames skipped)", counter->nr_rendered, counter->nr_skipped);
    } else {
#endif
        LOGI("%d fps", counter->nr_rendered);
#ifdef SKIP_FRAMES
    }
#endif // SKIP_FRAMES
}

static void check_expired(struct fps_counter *counter)
{
    Uint32 now = SDL_GetTicks();
    if(now - counter->slice_start >= 1000)
    {
        display_fps(counter);

        Uint32 elapsed_slices = (now - counter->slice_start) / 1000;
        counter->slice_start += 1000 * elapsed_slices;
        counter->nr_rendered = 0;
#ifdef SKIP_FRAMES
        counter->nr_skipped = 0;
#endif //
    }
}



void fps_counter_add_rendered_frame(struct fps_counter *counter)
{
    check_expired(counter);
    ++counter->nr_rendered;
}

#ifdef SKIP_FRAMES
void fps_counter_add_skipped_frame(struct fps_counter *counter)
{
    check_expired(counter);
    ++counter->nr_skipped;
}

#endif // SKIP_FRAMES
