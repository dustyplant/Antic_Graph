cmake_minimum_required( VERSION 3.0 )

configure_file( "${PROJECT_SOURCE_DIR}/examples/AGRAPH_Examples_Config.h" "${PROJECT_BINARY_DIR}/AGRAPH_Examples_Config.h" )
include_directories( ${PROJECT_BINARY_DIR} )

add_subdirectory( CubeExample )
add_subdirectory( RenderQuadExample )
add_subdirectory( ShaderExample )
add_subdirectory( SpriteExample )
add_subdirectory( RapidJSONExample )
add_subdirectory( LoadingSpriteSheets )
add_subdirectory( SpriteSheetExample )