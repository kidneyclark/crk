project(crk)

add_library(crk 
	crk/mem/impl/front.cpp
	crk/utl/impl/memstream.cpp
	crk/utl/impl/tabula.cpp
)
target_include_directories(crk PUBLIC ./)
#target_link_libraries(crk ZLIB::ZLIB)
if(WIN32)
	target_compile_definitions(crk PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()