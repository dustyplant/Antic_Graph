cmake_minimum_required( VERSION 3.0 )
project( TilesetExample )

configure_file( ${PROJECT_SOURCE_DIR}/res/twilight.json 	${PROJECT_BINARY_DIR}/res/twilight.json 		COPYONLY )
configure_file( ${AGRAPH_RES_PATH}twilight-tiles.png 		${PROJECT_BINARY_DIR}/res/twilight-tiles.png 	COPYONLY )

add_executable( TilesetExample ${PROJECT_SOURCE_DIR}/TilesetExample.cpp )
target_link_libraries( TilesetExample ${AGRAPH_LIBRARIES} )