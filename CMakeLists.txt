cmake_minimum_required( VERSION 2.8 )

project( SDL2_Graph_API )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set( CMAKE_STATIC_LINKER_FLAGS "${CMAKE_CXX_FLAGS} --static" )

set( ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_BINARY_DIR}/lib )
set( SDL2_Graph_API PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

include_directories( ${CMAKE_SOURCE_DIR}/include )
link_directories( ${CMAKE_SOURCE_DIR}/lib )

add_library( SDL2_Graph_API STATIC ${CMAKE_SOURCE_DIR}/src/Antic_Graph.cpp )

#if( WIN32 )
	#target_link_libraries( SDL2_Graph_API mingw32 SDL2main )
#endif( WIN32 )

target_link_libraries( SDL2_Graph_API libSDL2.a libSDL2_image.a libSDL2_ttf.a )
