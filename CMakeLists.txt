cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(ntkernel-error-category VERSION 1.0 LANGUAGES CXX)
enable_testing()
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(PROJECT_IS_DEPENDENCY OFF)
else()
  set(PROJECT_IS_DEPENDENCY ON)
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

add_library(ntkernel-error-category_dl SHARED "src/ntkernel_category.cpp")
target_compile_features(ntkernel-error-category_dl PUBLIC cxx_std_11)
target_include_directories(ntkernel-error-category_dl INTERFACE "include")
set_target_properties(ntkernel-error-category_dl PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
add_library(ntkernel-error-category::dl ALIAS ntkernel-error-category_dl)
install(FILES ntkernel-error-category_dl}
  DESTINATION "lib"
)

add_library(ntkernel-error-category_sl STATIC "src/ntkernel_category.cpp")
target_compile_features(ntkernel-error-category_sl PUBLIC cxx_std_11)
target_include_directories(ntkernel-error-category_sl INTERFACE "include")
target_compile_definitions(ntkernel-error-category_sl PRIVATE NTKERNEL_ERROR_CATEGORY_STATIC)
set_target_properties(ntkernel-error-category_sl PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
add_library(ntkernel-error-category::sl ALIAS ntkernel-error-category_sl)
install(FILES ntkernel-error-category_sl}
  DESTINATION "lib"
)

add_library(ntkernel-error-category_hl INTERFACE)
target_compile_features(ntkernel-error-category_hl INTERFACE cxx_std_11)
target_include_directories(ntkernel-error-category_hl INTERFACE "include")
target_compile_definitions(ntkernel-error-category_hl INTERFACE
  NTKERNEL_ERROR_CATEGORY_INLINE
  NTKERNEL_ERROR_CATEGORY_STATIC
)
add_library(ntkernel-error-category::hl ALIAS ntkernel-error-category_hl)

export(
  TARGETS ntkernel-error-category_dl ntkernel-error-category_sl ntkernel-error-category_hl
  FILE "NtKernelErrorCategoryTargets.cmake"
  EXPORT_LINK_INTERFACE_LIBRARIES
)

if(NOT PROJECT_IS_DEPENDENCY)
  add_executable(test-dl "test/main.cpp")
  target_link_libraries(test-dl PRIVATE ntkernel-error-category::dl)
  set_target_properties(test-dl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
  add_test(NAME test-dl COMMAND $<TARGET_FILE:test-dl>)

  add_executable(test-sl "test/main.cpp")
  target_link_libraries(test-sl PRIVATE ntkernel-error-category::sl)
  set_target_properties(test-sl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
  add_test(NAME test-sl COMMAND $<TARGET_FILE:test-sl>)

  add_executable(test-hl "test/main.cpp")
  target_link_libraries(test-hl PRIVATE ntkernel-error-category::hl)
  set_target_properties(test-hl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
  add_test(NAME test-hl COMMAND $<TARGET_FILE:test-hl>)
endif()
