include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

set(lenactions_src
  lib/utils.cc
	lib/pixel.cc
	lib/convolution.cc
	lib/image.cc
  
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
