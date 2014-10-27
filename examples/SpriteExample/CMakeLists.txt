cmake_minimum_required( VERSION 3.0 )
project( SpriteExample )

add_executable( SpriteExample main.cpp )

target_link_libraries( SpriteExample ${AGRAPH_LIBRARIES} )