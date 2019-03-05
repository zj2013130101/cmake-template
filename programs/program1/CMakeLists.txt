add_executable( ${PROGRAM1_EXECUTABLE_NAME} main.cpp )
if( UNIX AND NOT APPLE )
    set(rt_library rt )
endif()

#find_package( Gperftools QUIET )
#if( GPERFTOOLS_FOUND )
#    message( STATUS "Found gperftools; compiling with TCMalloc")
#    list( APPEND PLATFORM_SPECIFIC_LIBS tcmalloc )
#endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../.git)
    find_package(Git)
    if(GIT_FOUND)
        execute_process(
                COMMAND ${GIT_EXECUTABLE} rev-parse --short=8 HEAD
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../.."
                OUTPUT_VARIABLE "program1_BUILD_VERSION"
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE)
        message(STATUS "Git commit revision: ${program1_BUILD_VERSION}")
    else()
        set(program1_BUILD_VERSION 0)
    endif()
else()
    set(program1_BUILD_VERSION 0)
endif()

#find_package(Intl REQUIRED)

set(LOCALEDIR ${CMAKE_INSTALL_PREFIX}/share/locale)
set(LOCALEDOMAIN ${PROGRAM1_EXECUTABLE_NAME})
configure_file(config.hpp.in config.hpp ESCAPE_QUOTES)

#target_include_directories(${PROGRAM1_EXECUTABLE_NAME} PUBLIC ${Intl_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROGRAM1_EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries( ${PROGRAM1_EXECUTABLE_NAME}
        PRIVATE lib1 lib2 lib3 ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS} )


copy_bin( ${PROGRAM1_EXECUTABLE_NAME} )
install( TARGETS
        ${PROGRAM1_EXECUTABLE_NAME}

        RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
        )
