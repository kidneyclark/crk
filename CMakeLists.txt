project(crk)

add_subdirectory(external)

add_library(crk 
	crk/io/impl/front.cpp
	crk/gfx/impl/front.cpp
)
target_include_directories(crk PUBLIC ./)
#target_link_libraries(crk ZLIB::ZLIB)
target_include_directories(crk PUBLIC ./external/cgltf/)

target_link_libraries(crk glad)
target_link_libraries(crk simdjson)

if(WIN32)
	target_compile_definitions(crk PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()