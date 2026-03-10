cmake_minimum_required(VERSION 3.10)
project(libtruerandom VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(TRUERANDOM_TESTS "Build and register tests" ON)
option(TRUERANDOM_INSTALL "Install headers and library" ON)

set(TRUERANDOM_SOURCES
  src/core.c
  src/urandom.c
  src/linux.c
  src/windows.c
  src/darwin.c
  src/posix_fallback.c
)

add_library(truerandom ${TRUERANDOM_SOURCES})
set_target_properties(truerandom PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)
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
if(WIN32)
  target_link_libraries(truerandom PRIVATE bcrypt)
endif()

if(TRUERANDOM_INSTALL)
  include(GNUInstallDirs)

  install(TARGETS truerandom
    EXPORT truerandomTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
  install(DIRECTORY include/truerandom
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
  )

  install(EXPORT truerandomTargets
    FILE truerandomConfig.cmake
    NAMESPACE truerandom::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/truerandom
  )

  configure_file(libtruerandom.pc.in libtruerandom.pc @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libtruerandom.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
  )
endif()

if(TRUERANDOM_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
