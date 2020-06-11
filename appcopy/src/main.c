#include "scrcpy.h"
#include "config.h"
#include <stdio.h>
#include <SDL2/SDL.h>


int main(int argc, char const *argv[])
{
    // open debug
#ifdef BUILD_DEBUG
       SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif
   
   // main program start
   Uint16 local_port = DEFAULT_LOCAL_PORT;
   Uint16 max_size = DEFAULT_MAX_SIZE;
   Uint32  bit_rate = DEFAULT_BIT_RATE;
   scrcpy(local_port,max_size,bit_rate);
    
    return 0;
}
