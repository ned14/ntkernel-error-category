cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(ntkernel-error-category VERSION 1.0 LANGUAGES CXX)
enable_testing()
include(GNUInstallDirs)
if(NOT DEFINED ntkernel-error-category_IS_DEPENDENCY)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(ntkernel-error-category_IS_DEPENDENCY OFF)
  else()
    set(ntkernel-error-category_IS_DEPENDENCY ON)
  endif()
endif()

if(WIN32)
  # Create the table generating program
  add_executable(generate-table "src/generate-table.cpp")
  target_compile_features(generate-table PRIVATE cxx_noexcept)
  target_compile_definitions(generate-table PRIVATE _UNICODE UNICODE)
  set_target_properties(generate-table PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
endif()

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/ntkernel-error-category"
  DESTINATION "include"
)

add_library(${PROJECT_NAME}_dl SHARED "src/ntkernel_category.cpp")
target_compile_features(${PROJECT_NAME}_dl PUBLIC cxx_std_11)
target_include_directories(${PROJECT_NAME}_dl PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)
set_target_properties(${PROJECT_NAME}_dl PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
add_library(${PROJECT_NAME}::dl ALIAS ${PROJECT_NAME}_dl)
if(CMAKE_GENERATOR MATCHES "Visual Studio")
  set_target_properties(${PROJECT_NAME}_dl PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}_dl-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}-$<PLATFORM_ID>-${CMAKE_SYSTEM_PROCESSOR}-$<CONFIG>"
  )
elseif(CMAKE_GENERATOR MATCHES "Xcode")
  set_target_properties(${PROJECT_NAME}_dl PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}_dl-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}-$<PLATFORM_ID>-${CMAKE_SYSTEM_PROCESSOR}-$<CONFIG>"
  )
else()
  set_target_properties(${PROJECT_NAME}_dl PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}_dl-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE}"
  )
endif()
install(TARGETS ${PROJECT_NAME}_dl
  EXPORT ${PROJECT_NAME}Exports
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
set_target_properties(${PROJECT_NAME}_dl PROPERTIES EXPORT_NAME dl)

add_library(${PROJECT_NAME}_sl STATIC "src/ntkernel_category.cpp")
target_compile_features(${PROJECT_NAME}_sl PUBLIC cxx_std_11)
target_include_directories(${PROJECT_NAME}_sl PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)
target_compile_definitions(${PROJECT_NAME}_sl PRIVATE NTKERNEL_ERROR_CATEGORY_STATIC)
set_target_properties(${PROJECT_NAME}_sl PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
if(CMAKE_GENERATOR MATCHES "Visual Studio")
  set_target_properties(${PROJECT_NAME}_sl PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}_sl-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}-$<PLATFORM_ID>-${CMAKE_SYSTEM_PROCESSOR}-$<CONFIG>"
  )
elseif(CMAKE_GENERATOR MATCHES "Xcode")
  set_target_properties(${PROJECT_NAME}_sl PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}_sl-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}-$<PLATFORM_ID>-${CMAKE_SYSTEM_PROCESSOR}-$<CONFIG>"
  )
else()
  set_target_properties(${PROJECT_NAME}_sl PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}_sl-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE}"
  )
endif()
add_library(${PROJECT_NAME}::sl ALIAS ${PROJECT_NAME}_sl)
install(TARGETS ${PROJECT_NAME}_sl
  EXPORT ${PROJECT_NAME}Exports
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
set_target_properties(${PROJECT_NAME}_sl PROPERTIES EXPORT_NAME sl)

add_library(${PROJECT_NAME}_hl INTERFACE)
target_compile_features(${PROJECT_NAME}_hl INTERFACE cxx_std_11)
target_include_directories(${PROJECT_NAME}_hl INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)
target_compile_definitions(${PROJECT_NAME}_hl INTERFACE
  NTKERNEL_ERROR_CATEGORY_INLINE
  NTKERNEL_ERROR_CATEGORY_STATIC
)
add_library(${PROJECT_NAME}::hl ALIAS ${PROJECT_NAME}_hl)
install(TARGETS ${PROJECT_NAME}_hl
  EXPORT ${PROJECT_NAME}Exports
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
set_target_properties(${PROJECT_NAME}_hl PROPERTIES EXPORT_NAME hl)

configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/ProjectConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  @ONLY
)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
install(EXPORT ${PROJECT_NAME}Exports
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)


if(NOT ntkernel-error-category_IS_DEPENDENCY AND (NOT DEFINED BUILD_TESTING OR BUILD_TESTING))
  add_executable(test-dl "test/main.cpp")
  target_link_libraries(test-dl PRIVATE ${PROJECT_NAME}::dl)
  set_target_properties(test-dl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
  add_test(NAME test-dl COMMAND $<TARGET_FILE:test-dl>)

  add_executable(test-sl "test/main.cpp")
  target_link_libraries(test-sl PRIVATE ${PROJECT_NAME}::sl)
  set_target_properties(test-sl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
  add_test(NAME test-sl COMMAND $<TARGET_FILE:test-sl>)

  add_executable(test-hl "test/main.cpp")
  target_link_libraries(test-hl PRIVATE ${PROJECT_NAME}::hl)
  set_target_properties(test-hl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
  add_test(NAME test-hl COMMAND $<TARGET_FILE:test-hl>)
endif()
