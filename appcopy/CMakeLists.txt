# 添加所有源文件和头文件
include_directories(./include)
aux_source_directory(./src SRC_DIR)
# set(EXTRA_SRC )

# 添加target
add_executable(pmcopy ${SRC_DIR})
# 链接依赖库
target_link_libraries(pmcopy SDL2)

# 测试
add_subdirectory(test)