#ifndef COMMAND_H
#define COMMAND_H

#include <inttypes.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_platform.h>

#include <sys/types.h>
#define PROCESS_NONE -1
typedef pid_t process_t;
typedef int exit_code_t;

#define NO_EXIT_CODE -1

process_t cmd_execute(const char *path, const char *const argv[]);
SDL_bool cmd_terminate(process_t pid);
SDL_bool cmd_simple_wait(process_t pid, exit_code_t *exit_code);

process_t adb_execute(const char *const adb_cmd[], int len);
process_t adb_forward(uint16_t local_port, const char *device_socket_name);
process_t adb_reverse(const char *device_socket_name, uint16_t local_port);
process_t adb_reverse_remove(const char *device_socket_name);
process_t adb_push(const char *local, const char *remote);
process_t adb_remove_path(const char *path);

#endif // COMMAND_H