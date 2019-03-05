configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/project_namespace/lib1_namespace/core_symbol.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/project_namespace/lib1_namespace/core_symbol.hpp)

file(GLOB HEADERS "include/project_namespace/lib1_namespace/*.hpp"
                  "include/project_namespace/lib1_namespace/other/*.hpp"
                  )

## SORT .cpp by most likely to change / break compile
add_library( lib1
             lib1.cpp
             ${HEADERS}
             )

#target_link_libraries( lib1 ... ... ... )

target_include_directories( lib1
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
                         )


message("cmake install full libdir : ${CMAKE_INSTALL_FULL_LIBDIR}")
message("cmake install full bindir : ${CMAKE_INSTALL_FULL_BINDIR}")

install( TARGETS lib1
   RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)
install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/project_namespace/lib1_namespace/
      DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/project_namespace/lib1_namespace
      FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h" PATTERN "other" EXCLUDE
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/project_namespace/lib1_namespace/core_symbol.hpp DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/project_namespace/lib1_namespace)

