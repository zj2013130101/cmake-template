
file(GLOB HEADERS "include/project_namespace/lib2_namespace/*.hpp"
                  )

## SORT .cpp by most likely to change / break compile
# default archive static library
add_library( lib2
             lib2.cpp
             )

#target_link_libraries( lib2 ... ... ... )

target_include_directories( lib2
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
                            )

install( TARGETS lib2
   RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/project_namespace/lib2_namespace/
      DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/project_namespace/lib2_namespace
      FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h" PATTERN "other" EXCLUDE
)