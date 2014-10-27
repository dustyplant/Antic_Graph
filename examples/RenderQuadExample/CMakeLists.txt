cmake_minimum_required( VERSION 3.0 )
project( RenderQuadExample )

#configure_file( Shaders/VertexShader.glsl Shaders/VertexShader.glsl COPYONLY )
#configure_file( Shaders/FragmentShader.glsl Shaders/FragmentShader.glsl COPYONLY )

add_executable( RenderQuadExample main.cpp )

target_link_libraries( RenderQuadExample ${AGRAPH_LIBRARIES} IL ILU )