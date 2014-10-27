cmake_minimum_required( VERSION 3.0 )
project( SpriteSheetExample )

configure_file( Elisa.json Elisa.json COPYONLY )

add_executable( SpriteSheetExample main.cpp )
target_link_libraries( SpriteSheetExample ${AGRAPH_LIBRARIES} )
