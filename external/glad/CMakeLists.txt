project(glad)

add_library(glad 
	src/glad.c
)
target_include_directories(glad PUBLIC ./include/)
if(WIN32)
	target_compile_definitions(glad PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()