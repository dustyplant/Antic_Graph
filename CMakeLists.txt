cmake_minimum_required( VERSION 3.0 )
project( Antic_Graph )

# Flags for C++11 and for better debugging output.
set( CMAKE_CXX_FLAGS " -g --std=c++11 " )

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH} )
set( AGRAPH_RES_PATH "${PROJECT_SOURCE_DIR}/res/" CACHE STRING "Path to the Antic_Graph res folder." )

set( AGRAPH_INCLUDE_DIRS 
	# The base Antic_Graph include directory to find header files.
	${PROJECT_SOURCE_DIR}/include

	# Include the RapidJSON header library.
	${PROJECT_SOURCE_DIR}/submodules/rapidjson/include

	CACHE STRING "Antic_Graph include directories"
)

# Addes all the include directories at once.
include_directories( ${AGRAPH_INCLUDE_DIRS} )

# So that the examples and tests can link to the Antic_Graph library once it is made.
set( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )
link_directories( ${PROJECT_BINARY_DIR}/lib )

# Compile and link the static Antic_Graph library.
add_library( Antic_Graph STATIC
	${PROJECT_SOURCE_DIR}/src/Antic_Graph.cpp
	${PROJECT_SOURCE_DIR}/src/JSONUtils.cpp
	${PROJECT_SOURCE_DIR}/src/Texture.cpp
	${PROJECT_SOURCE_DIR}/src/Shader.cpp
	${PROJECT_SOURCE_DIR}/src/Sprite.cpp
)


# Set up the libraries to be linked to for examples and tests.
if( WIN32 )
	set( AGRAPH_LIBRARIES_BASE ${AGRAPH_LIBRARIES_BASE} mingw32 glew32 SDL2main )
endif( WIN32 )


# Add in the submodules.
add_subdirectory( ${PROJECT_SOURCE_DIR}/submodules )

find_package( PkgConfig REQUIRED )
pkg_search_module( GLFW glfw3 )

if( ${GLFW_FOUND} )
	include_directories( ${GLFW_INCLUDE_DIRS} )
	set( AGRAPH_LIBRARIES_BASE ${AGRPH_LIBRARIES_BASE} ${GLFW_STATIC_LIBRARIES} )
else()
	add_subdirectory( ${PROJECT_SOURCE_DIR}/submodules/glfw )
	include_directories( ${PROJECT_SOURCE_DIR}/submodules/glfw/include )
	set( AGRAPH_LIBRARIES_BASE ${AGRPH_LIBRARIES_BASE} glfw3 ${GLFW_LIBRARIES} )
endif()



find_package( GLEW REQUIRED )
if( GLEW_FOUND )
	include_directories( ${GLEW_INCLUDE_DIR} )
	set( AGRAPH_LIBRARIES_BASE ${AGRAPH_LIBRARIES_BASE} ${GLEW_LIBRARIES} )
endif( GLEW_FOUND )

find_package( OpenGL REQUIRED )
if( OPENGL_FOUND )
	include_directories( ${OPENGL_INCLUDE_DIR} )
	set( AGRAPH_LIBRARIES_COMMON ${OPENGL_LIBRARIES} )
endif( OPENGL_FOUND )


# Finds SDL2 library.
# include(FindPkgConfig)
# PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
# include_directories( ${SDL2_INCLUDE_DIRS} )
# set( AGRAPH_LIBRARIES_COMMON ${AGRAPH_LIBRARIES_COMMON} ${SDL2_LIBRARIES} )


# Finds the DevIL (Developer Image Library) library.
find_package( DevIL REQUIRED )
if( IL_FOUND )
	include_directories( ${IL_INCLUDE_DIR} )
	set( AGRAPH_LIBRARIES_BASE ${AGRAPH_LIBRARIES_BASE} ${IL_LIBRARIES} ${ILU_LIBRARIES} )
endif( IL_FOUND )




# Links the libraries into the project.
target_link_libraries( Antic_Graph ${AGRAPH_LIBRARIES_BASE} ${AGRAPH_LIBRARIES_COMMON} )


# Adds the libraries together with the newly linked Antic_Graph library. Used to compile the examples and tests.
set( AGRAPH_LIBRARIES ${AGRAPH_LIBRARIES_BASE} Antic_Graph ${AGRAPH_LIBRARIES_COMMON} CACHE STRING "Antic_Graph link libraries" )

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