
file(GLOB HEADERS "include/project_namespace/lib3_namespace/*.hpp"
                  )

## SORT .cpp by most likely to change / break compile
# default archive static library
add_library( lib3
             lib3.cpp
             )

#target_link_libraries( lib3 ... ... ... )

target_include_directories( lib3
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
                            )

install( TARGETS lib3
   RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/project_namespace/lib3_namespace/
      DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/project_namespace/lib3_namespace
      FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h" PATTERN "other" EXCLUDE
)