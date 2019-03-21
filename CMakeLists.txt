#
# Copyright 2016 c++ project template
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.5)
project(project_template)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  message(WARNING "CMAKE_INSTALL_PREFIX is set to default path of ${CMAKE_INSTALL_PREFIX}, resetting to ${CMAKE_INSTALL_PREFIX}/project_template")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/eosio")
elseif ("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
  message(WARNING "CMAKE_INSTALL_PREFIX is explicitly set to /usr/local. This is not recommended.")
endif()

option(BUILD_TESTS "Build GTest-based tests" ON)
option(OPTION1 "Select Option1" ON)
option(OPTION2 "Select Option1" ON)
option(OPTION3 "Select Option1" ON)
option(CODE_COVERAGE "Build with code coverage enabled" ON)
option(WITH_EXCEPTIONS "Build with exceptions enabled" ON)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 4)
set(VERSION_PATCH 1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${project_template_SOURCE_DIR}/CMakeModules)
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
include(GNUInstallDirs)
include( InstallDirectoryPermissions )
include(utils)


if (${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
  set(COMPILER_IS_CLANG 1)
  set(COMPILER_IS_GNU 0)
  set(COMPILER_IS_MSVC 0)
elseif (${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
  set(COMPILER_IS_CLANG 0)
  set(COMPILER_IS_GNU 1)
  set(COMPILER_IS_MSVC 0)
elseif (${CMAKE_C_COMPILER_ID} STREQUAL "MSVC")
  set(COMPILER_IS_CLANG 0)
  set(COMPILER_IS_GNU 0)
  set(COMPILER_IS_MSVC 1)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
  set(COMPILER_IS_CLANG 1)
  set(COMPILER_IS_GNU 0)
  set(COMPILER_IS_MSVC 0)
else ()
  set(COMPILER_IS_CLANG 0)
  set(COMPILER_IS_GNU 0)
  set(COMPILER_IS_MSVC 0)
endif ()

include(CheckIncludeFile)
include(CheckSymbolExists)

check_include_file("alloca.h" HAVE_ALLOCA_H)
check_include_file("unistd.h" HAVE_UNISTD_H)
check_symbol_exists(snprintf "stdio.h" HAVE_SNPRINTF)
check_symbol_exists(sysconf "unistd.h" HAVE_SYSCONF)
check_symbol_exists(strcasecmp "strings.h" HAVE_STRCASECMP)

if (WIN32)
  check_symbol_exists(ENABLE_VIRTUAL_TERMINAL_PROCESSING "windows.h" HAVE_WIN32_VT100)
endif ()

if (COMPILER_IS_MSVC)
  # disable warning C4018: signed/unsigned mismatch
  # disable warning C4056, C4756: overflow in floating-point constant arithmetic
  #   seems to not like float compare w/ HUGE_VALF; bug?
  # disable warnings C4267 and C4244: conversion/truncation from larger to smaller type.
  # disable warning C4800: implicit conversion from larger int to bool
  add_definitions(-W3 -wd4018 -wd4056 -wd4756 -wd4267 -wd4244 -wd4800 -WX -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)

  if (NOT WITH_EXCEPTIONS)
    # disable exception use in C++ library
    add_definitions(-D_HAS_EXCEPTIONS=0)
  endif ()
else ()
  # disable -Wunused-parameter: this is really common when implementing
  #   interfaces, etc.
  # disable -Wpointer-arith: this is a GCC extension, and doesn't work in MSVC.
  add_definitions(
          -Wall -Wextra -Werror -Wno-unused-parameter -Wpointer-arith -g -std=c++11
          -Wold-style-cast -Wuninitialized
  )


  if (NOT WITH_EXCEPTIONS)
    add_definitions(-fno-exceptions)
  endif ()

  # Need to define __STDC_*_MACROS because C99 specifies that C++ shouldn't
  # define format (e.g. PRIu64) or limit (e.g. UINT32_MAX) macros without the
  # definition, and some libcs (e.g. glibc2.17 and earlier) follow that.
  add_definitions(-D__STDC_LIMIT_MACROS=1 -D__STDC_FORMAT_MACROS=1)

  if (MINGW)
    # _POSIX is needed to ensure we use mingw printf
    # instead of the VC runtime one.
    add_definitions(-D_POSIX)
  endif ()

  if (COMPILER_IS_GNU)
    # disable -Wclobbered: it seems to be guessing incorrectly about a local
    # variable being clobbered by longjmp.
    add_definitions(-Wno-clobbered)
  endif ()


  # try to get the target architecture by compiling a dummy.c file and
  # checking the architecture using the file command.
  file(WRITE ${project_template_BINARY_DIR}/dummy.c "main(){}")
  try_compile(
    COMPILE_OK
    ${project_template_BINARY_DIR}
    ${project_template_BINARY_DIR}/dummy.c
    COPY_FILE ${project_template_BINARY_DIR}/dummy
  )
  if (COMPILE_OK)
    execute_process(
      COMMAND file ${project_template_BINARY_DIR}/dummy
      RESULT_VARIABLE FILE_RESULT
      OUTPUT_VARIABLE FILE_OUTPUT
      ERROR_QUIET
    )

    if (FILE_RESULT EQUAL 0)
      if (${FILE_OUTPUT} MATCHES "x86[-_]64")
        set(TARGET_ARCH "x86-64")
      elseif (${FILE_OUTPUT} MATCHES "Intel 80386")
        set(TARGET_ARCH "i386")
      elseif (${FILE_OUTPUT} MATCHES "ARM")
        set(TARGET_ARCH "ARM")
      else ()
        message(WARNING "Unknown target architecture!")
      endif ()
    else ()
      message(WARNING "Error running file on dummy executable")
    endif ()
  else ()
    message(WARNING "Error compiling dummy.c file")
  endif ()

endif ()


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
    message(FATAL_ERROR "GCC version must be at least 6.0!")
  endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.0)
    message(FATAL_ERROR "Clang version must be at least 4.0!")
  endif()
endif()


set( PROGRAM1_EXECUTABLE_NAME program1 )
set( PROGRAM2_EXECUTABLE_NAME program2 )

add_subdirectory( libraries )
add_subdirectory( programs )

install_directory_permissions(DIRECTORY ${CMAKE_INSTALL_FULL_SYSCONFDIR}/project_namespace)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/project.version.in ${CMAKE_CURRENT_BINARY_DIR}/project.version.hpp)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/project.version.hpp DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})

include(package)