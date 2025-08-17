project(crk C)

set(CMAKE_C_STANDARD 90)
add_subdirectory(external)

add_library(crk 
	crk/io_disk.c
	crk/io_memstream.c
	crk/memstream.c
	crk/thread_pthread.c
	crk/tabula.c
	crk/gfx_gl_shader.c
)
target_include_directories(crk PUBLIC ./)
#target_link_libraries(crk ZLIB::ZLIB)
target_include_directories(crk PUBLIC ./external/cgltf/)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(crk Threads::Threads)
target_link_libraries(crk glad)
target_link_libraries(crk simdjson)
target_link_libraries(crk cglm)

if(WIN32)
	target_compile_definitions(crk PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()