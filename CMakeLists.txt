cmake_minimum_required( VERSION 2.8 )

project( Antic_Graph )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set( CMAKE_STATIC_LINKER_FLAGS "${CMAKE_CXX_FLAGS} --static" )

set( ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_BINARY_DIR}/lib )
set( Antic_Graph PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

include_directories( ${CMAKE_SOURCE_DIR}/include )
link_directories( ${CMAKE_SOURCE_DIR}/lib )

add_library( Antic_Graph STATIC ${CMAKE_SOURCE_DIR}/src/Antic_Graph.cpp )

#if( WIN32 )
	#target_link_libraries( Antic_Graph mingw32 SDL2main )
#endif( WIN32 )

target_link_libraries( Antic_Graph libSDL2.a libSDL2_image.a libSDL2_ttf.a )
