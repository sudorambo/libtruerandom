cmake_minimum_required(VERSION 3.10)
project(libtruerandom C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(TRUERANDOM_TESTS "Build and register tests" ON)
option(TRUERANDOM_INSTALL "Install headers and library" ON)

set(TRUERANDOM_SOURCES
  src/core.c
  src/linux.c
  src/windows.c
  src/darwin.c
  src/posix_fallback.c
)

add_library(truerandom ${TRUERANDOM_SOURCES})
target_include_directories(truerandom
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_options(truerandom PRIVATE
  $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
)

if(TRUERANDOM_INSTALL)
  install(TARGETS truerandom
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
  )
  install(DIRECTORY include/truerandom
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
  )
endif()

if(TRUERANDOM_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
