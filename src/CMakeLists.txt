include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

set(lenactions_src
  lib/utils.cc
	lib/pixel.cc
	lib/convolution.cc
	lib/image.cc
	lib/unionfind.cc
	
  
  app/main.cc
)

add_executable(lenactions
  ${lenactions_src}
)

install(
  TARGETS
    lenactions
  DESTINATION
    bin/
)




find_package(BISON)
find_package(FLEX)

bison_target(
  Parser
  ${CMAKE_CURRENT_SOURCE_DIR}/lenash/parser.y
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
)
flex_target(
  Lexer
  ${CMAKE_CURRENT_SOURCE_DIR}/lenash/lexer.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)
add_flex_bison_dependency(Lexer Parser)

set(lenash_src
  lib/utils.cc
	lib/pixel.cc
	lib/convolution.cc
	lib/image.cc
	lib/unionfind.cc
	
	${CMAKE_CURRENT_BINARY_DIR}/parser.cc
	${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

add_executable(lenash
	${lenash_src}
)
target_link_libraries(lenash
  fl
)

install(
	TARGETS
		lenash
	DESTINATION
		${CMAKE_INSTALL_PREFIX}/bin
)
