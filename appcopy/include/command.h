/**
 * @file command.h
 * @author raven (zhang.xingrui@foxmail.com)
 * @brief 开启子进程执行命令(主要是adb的相关命令),如
 *      1. adb push
 *      2. adb reverse
 *      3. xxx
 * @version 0.1
 * @date 2020-06-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
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

# define PRIsizet "zu"
# define PRIexitcode "d"

/**
 * @brief 开启子进程,执行 path 命令, 传递 argv 参数
 * 
 * @param path 命令路径,如果命令在环境变量中,执行传递命令名即可
 * @param argv 命令参数
 * @return process_t 
 *          成功开启子进程,返回子进程pid
 *          失败,返回-1
 */
process_t cmd_execute(const char *path, const char *const argv[]);
/**
 * @brief 结束进程id为pid的进程
 * 
 * @param pid 进程id
 * @return SDL_bool 成功 true, 失败false
 */
SDL_bool cmd_terminate(process_t pid);
/**
 * @brief 等待子进程结束
 * 
 * @param pid 子进程id
 * @param exit_code 如果不为NULL, 保存子进程结束时的推出码
 * @return SDL_bool true,等待结束成功
 *                  false, 等待结束失败
 */
SDL_bool cmd_simple_wait(process_t pid, exit_code_t *exit_code);

/**
 * @brief 执行adb命令
 * 
 * @param adb_cmd adb命令
 * @param len adb命令的长度. 例如: {"adb", "push" , "source_file_path", "dest_file_path"}的长度为4
 * @return process_t 执行子进程的pid
 */
process_t adb_execute(const char *const adb_cmd[], int len);
/**
 * @brief adb_forward命令
 * 
 * @param local_port 本地端口
 * @param device_socket_name 设置socket名
 * @return process_t 执行子进程的pid
 */
process_t adb_forward(uint16_t local_port, const char *device_socket_name);
/**
 * @brief adb_reverse命令
 * 
 * @param device_socket_name 设备socket名
 * @param local_port 本地端口
 * @return process_t 执行子进程的pid
 */
process_t adb_reverse(const char *device_socket_name, uint16_t local_port);
/**
 * @brief adb_reverse_remove命令
 * 
 * @param device_socket_name 设备socket名
 * @return process_t 执行子进程的pid
 */
process_t adb_reverse_remove(const char *device_socket_name);
/**
 * @brief adb_push命令
 * 
 * @param local 本地文件路径
 * @param remote 推送目的地路径
 * @return process_t  执行子进程的pid
 */
process_t adb_push(const char *local, const char *remote);
/**
 * @brief adb_remove_path命令
 * 
 * @param path 移除的文件的的路径
 * @return process_t 执行子进程的pid
 */
process_t adb_remove_path(const char *path);

/**
 * @brief 检查 proc 是否开启成功
 * 
 * @param proc 
 * @param name 进程名,(用于log标识)
 * @return SDL_bool 
 */
SDL_bool process_check_success(process_t proc, const char *name);

#endif // COMMAND_H