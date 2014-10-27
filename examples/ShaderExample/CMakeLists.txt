cmake_minimum_required( VERSION 3.0 )
project( ShaderExample )

configure_file( Shaders/VertexShader.glsl Shaders/VertexShader.glsl COPYONLY )
configure_file( Shaders/FragmentShader.glsl Shaders/FragmentShader.glsl COPYONLY )

add_executable( ShaderExample main.cpp )

target_link_libraries( ShaderExample ${AGRAPH_LIBRARIES} )