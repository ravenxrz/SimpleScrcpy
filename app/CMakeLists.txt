# 添加所有源文件和头文件
include_directories(include)
aux_source_directory(src SRC_DIR)
set(EXTRA_SRC src/sys/unix/command.c src/sys/unix/net.c)

# 添加target
add_executable(phonemirror ${SRC_DIR} ${EXTRA_SRC})
# 链接依赖库
target_link_libraries(phonemirror avformat avcodec avutil SDL2)