cmake_minimum_required( VERSION 3.0 )
project( RapidJSONExample )

configure_file( Test.json Test.json COPYONLY )

add_executable( RapidJSONExample main.cpp )

target_link_libraries( RapidJSONExample ${AGRAPH_LIBRARIES} )