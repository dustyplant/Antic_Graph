cmake_minimum_required( VERSION 3.0 )
project( CubeExample )


configure_file( Shaders/VertexShader.glsl Shaders/VertexShader.glsl COPYONLY )
configure_file( Shaders/FragmentShader.glsl Shaders/FragmentShader.glsl COPYONLY )

add_executable( CubeExample
	main.cpp
)

target_link_libraries( CubeExample ${AGRAPH_LIBRARIES} )