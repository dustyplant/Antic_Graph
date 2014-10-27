cmake_minimum_required( VERSION 3.0 )
project( LoadingSpriteSheets )

configure_file( TestSpriteSheet.json TestSpriteSheet.json COPYONLY )

add_executable( LoadingSpriteSheets main.cpp )
target_link_libraries( LoadingSpriteSheets ${AGRAPH_LIBRARIES} )
