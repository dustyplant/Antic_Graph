cmake_minimum_required( VERSION 3.0 )
project( Antic_Graph )

# Flags for C++11 and for better debugging output.
set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -g --std=c++11 " )

include_directories( ${CMAKE_SOURCE_DIR}/include )

# So that the examples and tests can link to the Antic_Graph library once it is made.
set( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )
link_directories( ${PROJECT_BINARY_DIR}/lib )
link_directories( ${CMAKE_SOURCE_DIR}/lib )

# Compile and link the static Antic_Graph library.
add_library( Antic_Graph STATIC
	${CMAKE_SOURCE_DIR}/src/Antic_Graph.cpp
	${CMAKE_SOURCE_DIR}/src/Texture.cpp
	${CMAKE_SOURCE_DIR}/src/Shader.cpp
	${CMAKE_SOURCE_DIR}/src/Sprite.cpp
)


# Set up the libraries to be linked to for examples and tests.
if( WIN32 )
	set( AGRAPH_LIBRARIES ${AGRAPH_LIBRARIES} mingw32 glew32 SDL2main )
endif( WIN32 )

if( UNIX )
	set( AGRAPH_LIBRARIES ${AGRAPH_LIBRARIES} GLEW )
endif( UNIX )


# set( AGRAPH_LIBRARIES_COMMON GL GLU SDL2 )
set( AGRAPH_LIBRARIES_COMMON GL GLU )

# Finds SDL2 library.
include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
include_directories( ${SDL2_INCLUDE_DIRS} )
set( AGRAPH_LIBRARIES_COMMON ${AGRAPH_LIBRARIES_COMMON} ${SDL2_LIBRARIES} )

# Finds the DevIL (Developer Image Library) library.
find_package( DevIL REQUIRED )
if( IL_FOUND )
	include_directories( ${IL_INCLUDE_DIR} )
	set( AGRAPH_LIBRARIES ${AGRAPH_LIBRARIES} ${IL_LIBRARIES} ${ILU_LIBRARIES} )
endif( IL_FOUND )

# Links the libraries into the project.
target_link_libraries( Antic_Graph ${AGRAPH_LIBRARIES} ${AGRAPH_LIBRARIES_COMMON} )

# Adds the libraries together with the newly linked Antic_Graph library. Used to compile the examples and tests.
set( AGRAPH_LIBRARIES ${AGRAPH_LIBRARIES} Antic_Graph ${AGRAPH_LIBRARIES_COMMON} )

# Addes the examples to be compiled by CMake.
add_subdirectory( examples )

# TODO: Set up GTest cmake package find.
# find_package( GTEST )
# if( GTEST_FOUND )
# 	include_directories( ${GTEST_INCLUDE_DIRS} )
# 
# 	add_subdirectory( test )
# 	enable_testing()
# 	add_test( NAME ShaderTest COMMAND ShaderTest -V )
# endif( GTEST_FOUND)

# Run tests with the "make test" command at the root of the build directory.