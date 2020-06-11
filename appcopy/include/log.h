#ifndef LOG_H
#define LOG_H

#include <SDL2/SDL_log.h>
#include <stdio.h>

#define LOG_BASE(LEVEL) fprintf(stderr,"[%s] [file %s] [function %s] [line %d]\n",#LEVEL, __FILE__,__FUNCTION__,__LINE__)
#define LOG(LEVEL,...) \
do{                   \
    LOG_BASE(LEVEL); \
    SDL_Log##LEVEL(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__); \
}while(0)

#define LOGV(...) LOG(Verbose,__VA_ARGS__);

#define LOGD(...) LOG(Debug,__VA_ARGS__)
#define LOGI(...) LOG(Info,__VA_ARGS__)
#define LOGW(...) LOG(Warn,__VA_ARGS__)
#define LOGE(...) LOG(Error,__VA_ARGS__)
#define LOGC(...) LOG(Critical, __VA_ARGS__)


#endif // !LOG_H
