cmake_minimum_required(VERSION 3.28)
project(exception LANGUAGES CXX)

enable_testing()

file(GLOB test_sources *.cc)
foreach(test_file ${test_sources})
  get_filename_component(test_name ${test_file} NAME_WE)
  add_executable(${test_name} EXCLUDE_FROM_ALL ${test_file})
  target_sources(${test_name} PRIVATE
    FILE_SET CXX_MODULES
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../include
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/../include/exception/exception.cppm)
  target_include_directories(${test_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  target_compile_features(${test_name} PRIVATE cxx_std_23)

  set_target_properties(${test_name} PROPERTIES
    CXX_MODULE_STD OFF
    CXX_SCAN_FOR_MODULES ON)

  if(WIN32)
    target_link_libraries(${test_name} PRIVATE ntdll)
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_link_options(${test_name} PRIVATE -fuse-ld=lld)
  endif()

  add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
