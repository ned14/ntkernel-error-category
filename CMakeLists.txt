cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(ntkernel-error-category VERSION 1.0 LANGUAGES CXX)

if(WIN32)
  # Create the table generating program
  add_executable(generate-table "src/generate-table.cpp")
  target_compile_features(generate-table PRIVATE cxx_noexcept)
  target_compile_definitions(generate-table PRIVATE _UNICODE UNICODE)
  set_target_properties(generate-table PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
endif()
