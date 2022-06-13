cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

project(gz-python)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT APPLE)
  set(CMAKE_CXX_FLAGS "-fPIC")
endif()

#============================================================================
# Find protobuf
# 
# macOS: $ brew install protobuf
# 
find_package(Protobuf REQUIRED)

#============================================================================
# Find pybind11
# 
# macOS: $ brew install python
# macOS: $ brew install pybind11
# 
find_package(Python COMPONENTS Interpreter Development)
# find_package(pybind11 CONFIG)

#============================================================================
# Find Gazebo dependencies

# Default versions to Gazebo Garden
set(GZ_MSGS_VER 9)
set(GZ_TRANSPORT_VER 12)

if("$ENV{GZ_VERSION}" STREQUAL "edifice")
  # Edifice
  set(GZ_MSGS_VER 7)
  set(GZ_TRANSPORT_VER 10)
  message(STATUS "Compiling against Gazebo Edifice")
elseif("$ENV{GZ_VERSION}" STREQUAL "fortress")
  # Fortress
  set(GZ_MSGS_VER 8)
  set(GZ_TRANSPORT_VER 11)
  message(STATUS "Compiling against Gazebo Fortress")
else() 
  # Default to Garden
  message(STATUS "Compiling against Gazebo Garden")
endif()

find_package(ignition-msgs${GZ_MSGS_VER} REQUIRED)
find_package(ignition-transport${GZ_TRANSPORT_VER} REQUIRED)

#============================================================================
# subdirectories for external dependencies

add_subdirectory(external)

#============================================================================
# gz_msgs_extras_lib C++ library

add_library(gz_msgs_extras_lib SHARED
  src/pybind11/gz/msgs/extras.cc
)

target_link_libraries(gz_msgs_extras_lib
  ignition-msgs${GZ_MSGS_VER}::ignition-msgs${GZ_MSGS_VER}
)

target_include_directories(gz_msgs_extras_lib
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

#============================================================================
# msgs extras Python extension module

pybind11_add_module(extras MODULE
  src/pybind11/gz/msgs/_gz_msgs_extras_pybind11.cc
)

target_link_libraries(extras
  PRIVATE
    gz_msgs_extras_lib
    ignition-msgs${GZ_MSGS_VER}::ignition-msgs${GZ_MSGS_VER}
    ${Protobuf_LIBRARY}
    pybind11_native_proto_caster
)

target_include_directories(extras
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/pybind11_protobuf
)

add_dependencies(extras
  gz_msgs_extras_lib
)

set_target_properties(extras
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python/gz/msgs"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python/gz/msgs"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python/gz/msgs"
)

#============================================================================
# transport Python extension module

pybind11_add_module(transport MODULE
  src/pybind11/gz/transport/_gz_transport_pybind11.cc
)

target_link_libraries(transport
  PRIVATE
    ignition-msgs${GZ_MSGS_VER}::ignition-msgs${GZ_MSGS_VER}
    ignition-transport${GZ_TRANSPORT_VER}::ignition-transport${GZ_TRANSPORT_VER}
    ${Protobuf_LIBRARY}
    pybind11_native_proto_caster
)

target_include_directories(transport
  PRIVATE
    ${Protobuf_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/external/pybind11_protobuf
)

set_target_properties(transport
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python/gz"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python/gz"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python/gz"
)

#============================================================================
# main

add_executable(main src/main.cc)

target_link_libraries(main
  gz_msgs_extras_lib
  ignition-msgs${GZ_MSGS_VER}::ignition-msgs${GZ_MSGS_VER}
)

target_include_directories(main
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_dependencies(main
  gz_msgs_extras_lib
)

#============================================================================
# msg_example

add_executable(msg_example src/msg_example.cc)

target_link_libraries(msg_example
  ignition-msgs${GZ_MSGS_VER}::ignition-msgs${GZ_MSGS_VER}
)

#============================================================================
# publisher

add_executable(publisher src/publisher.cc)

target_link_libraries(publisher
  ignition-msgs${GZ_MSGS_VER}::ignition-msgs${GZ_MSGS_VER}
  ignition-transport${GZ_TRANSPORT_VER}::ignition-transport${GZ_TRANSPORT_VER}
)

#============================================================================
# subscriber

add_executable(subscriber src/subscriber.cc)

target_link_libraries(subscriber
  ignition-msgs${GZ_MSGS_VER}::ignition-msgs${GZ_MSGS_VER}
  ignition-transport${GZ_TRANSPORT_VER}::ignition-transport${GZ_TRANSPORT_VER}
)

#============================================================================
# rover_publisher

add_executable(rover_publisher src/rover_publisher.cc)

target_link_libraries(rover_publisher
  ignition-msgs${GZ_MSGS_VER}::ignition-msgs${GZ_MSGS_VER}
  ignition-transport${GZ_TRANSPORT_VER}::ignition-transport${GZ_TRANSPORT_VER}
)

#============================================================================
# rover_subscriber

add_executable(rover_subscriber src/rover_subscriber.cc)

target_link_libraries(rover_subscriber
  ignition-msgs${GZ_MSGS_VER}::ignition-msgs${GZ_MSGS_VER}
  ignition-transport${GZ_TRANSPORT_VER}::ignition-transport${GZ_TRANSPORT_VER}
)

#============================================================================
# gz_topic_echo

add_executable(gz_topic_echo src/gz_topic_echo.cc)

target_link_libraries(gz_topic_echo
  ignition-msgs${GZ_MSGS_VER}::ignition-msgs${GZ_MSGS_VER}
  ignition-transport${GZ_TRANSPORT_VER}::ignition-transport${GZ_TRANSPORT_VER}
)

#============================================================================
# generate protobuf Python bindings

# Get the proto files from the installed location of gz-msgs
set(PROTO_PATH ${ignition-msgs${GZ_MSGS_VER}_INCLUDE_DIRS})
file(GLOB PROTO_FILES ${PROTO_PATH}/gz/msgs/*.proto)

#============================================================================
# Modified from gz-msgs/src/CMakeLists.txt for Python
#============================================================================
# A function that calls protoc on a protobuf file
# Options:
#   GENERATE_RUBY       - generates ruby code for the message if specified
#   GENERATE_CPP        - generates c++ code for the message if specified
#   GENERATE_PY         - generates Python code for the message if specified
# One value arguments:
#   PROTO_PACKAGE       - Protobuf package the file belongs to (e.g. ".gz.msgs")
#   PROTOC_EXEC         - Path to protoc
#   INPUT_PROTO         - Path to the input .proto file
#   OUTPUT_CPP_DIR      - Path where C++ files are saved
#   OUTPUT_RUBY_DIR     - Path where Ruby files are saved
#   OUTPUT_PY_DIR       - Path where Python files are saved
#   OUTPUT_CPP_HH_VAR   - A CMake variable name containing a list that the C++ header path should be appended to
#   OUTPUT_CPP_CC_VAR   - A CMake variable name containing a list that the C++ source path should be appended to
#   OUTPUT_RUBY_VAR     - A CMake variable name containing a list that the ruby file should be appended to
#   OUTPUT_PY_VAR       - A CMake variable name containing a list that the Python file should be appended to
# Multi value arguments
#   PROTO_PATH          - Passed to protoc --proto_path
function(gz_msgs_protoc)
  set(options GENERATE_RUBY GENERATE_CPP GENERATE_PY)
  set(oneValueArgs PROTO_PACKAGE PROTOC_EXEC INPUT_PROTO OUTPUT_CPP_DIR OUTPUT_RUBY_DIR OUTPUT_PY_DIR OUTPUT_CPP_HH_VAR OUTPUT_CPP_CC_VAR OUTPUT_RUBY_VAR OUTPUT_PY_VAR)
  set(multiValueArgs PROTO_PATH)

  cmake_parse_arguments(gz_msgs_protoc "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  get_filename_component(ABS_FIL ${gz_msgs_protoc_INPUT_PROTO} ABSOLUTE)
  get_filename_component(FIL_WE ${gz_msgs_protoc_INPUT_PROTO} NAME_WE)

  set(protoc_args)
  set(output_files)

  foreach(proto_path ${gz_msgs_protoc_PROTO_PATH})
    list(APPEND protoc_args "--proto_path=${proto_path}")
  endforeach()

  set(proto_package_dir ".")
  if(gz_msgs_protoc_PROTO_PACKAGE)
    string(REPLACE "." "/" proto_package_dir ${gz_msgs_protoc_PROTO_PACKAGE})
  endif()

  if(gz_msgs_protoc_GENERATE_CPP)
    set(output_header "${gz_msgs_protoc_OUTPUT_CPP_DIR}${proto_package_dir}/${FIL_WE}.pb.h")
    set(output_source "${gz_msgs_protoc_OUTPUT_CPP_DIR}${proto_package_dir}/${FIL_WE}.pb.cc")
    list(APPEND ${gz_msgs_protoc_OUTPUT_CPP_HH_VAR} ${output_header})
    list(APPEND ${gz_msgs_protoc_OUTPUT_CPP_CC_VAR} ${output_source})
    list(APPEND output_files ${output_header})
    list(APPEND output_files ${output_source})
    list(APPEND protoc_args "--plugin=protoc-gen-ignmsgs=${GZ_MSGS_GEN_EXECUTABLE}")
    list(APPEND protoc_args "--cpp_out=dllexport_decl=GZ_MSGS_VISIBLE:${gz_msgs_protoc_OUTPUT_CPP_DIR}")
    list(APPEND protoc_args "--ignmsgs_out" "${gz_msgs_protoc_OUTPUT_CPP_DIR}")
    set(${gz_msgs_protoc_OUTPUT_CPP_HH_VAR} ${${gz_msgs_protoc_OUTPUT_CPP_HH_VAR}} PARENT_SCOPE)
    set(${gz_msgs_protoc_OUTPUT_CPP_CC_VAR} ${${gz_msgs_protoc_OUTPUT_CPP_CC_VAR}} PARENT_SCOPE)
  endif()

  if(gz_msgs_protoc_GENERATE_RUBY)
    file(MAKE_DIRECTORY ${gz_msgs_protoc_OUTPUT_RUBY_DIR})
    set(output_ruby "${gz_msgs_protoc_OUTPUT_RUBY_DIR}${proto_package_dir}/${FIL_WE}_pb.rb")
    list(APPEND ${gz_msgs_protoc_OUTPUT_RUBY_VAR} ${output_ruby})
    list(APPEND output_files ${output_ruby})
    list(APPEND protoc_args "--ruby_out=${gz_msgs_protoc_OUTPUT_RUBY_DIR}")
    set(${gz_msgs_protoc_OUTPUT_RUBY_VAR} ${${gz_msgs_protoc_OUTPUT_RUBY_VAR}} PARENT_SCOPE)
  endif()

  if(gz_msgs_protoc_GENERATE_PY)
    file(MAKE_DIRECTORY ${gz_msgs_protoc_OUTPUT_PY_DIR})
    set(output_py "${gz_msgs_protoc_OUTPUT_PY_DIR}${proto_package_dir}/${FIL_WE}_pb2.py")
    list(APPEND ${gz_msgs_protoc_OUTPUT_PY_VAR} ${output_py})
    list(APPEND output_files ${output_py})
    list(APPEND protoc_args "--python_out=${gz_msgs_protoc_OUTPUT_PY_DIR}")
    set(${gz_msgs_protoc_OUTPUT_PY_VAR} ${${gz_msgs_protoc_OUTPUT_PY_VAR}} PARENT_SCOPE)
  endif()

  #get_cmake_property(_variableNames VARIABLES)
  #foreach (_variableName ${_variableNames})
  #    message(STATUS "${_variableName}=${${_variableName}}")
  #endforeach()

  add_custom_command(
    OUTPUT
      ${output_files}
    COMMAND
      ${gz_msgs_protoc_PROTOC_EXEC}
    ARGS
      ${protoc_args}
      ${ABS_FIL}
    DEPENDS
      ${ABS_FIL}
      # gz_msgs_gen
    COMMENT "Running protoc on ${gz_msgs_protoc_INPUT_PROTO}"
    VERBATIM)
endfunction()

#============================================================================
# run the code generation
foreach(proto_file ${PROTO_FILES})
  gz_msgs_protoc(
    PROTO_PACKAGE
      .gz.msgs
    # GENERATE_CPP
    # GENERATE_RUBY
    GENERATE_PY
    INPUT_PROTO
      ${proto_file}
    PROTOC_EXEC
      protobuf::protoc
    OUTPUT_CPP_DIR
      "${PROJECT_BINARY_DIR}/include"
    OUTPUT_RUBY_DIR
      "${PROJECT_BINARY_DIR}/ruby"
    OUTPUT_PY_DIR
      "${PROJECT_BINARY_DIR}/python"
    OUTPUT_CPP_HH_VAR
      gen_headers
    OUTPUT_CPP_CC_VAR
      gen_sources
    OUTPUT_RUBY_VAR
      gen_ruby_scripts
    OUTPUT_PY_VAR
      gen_py_scripts
    PROTO_PATH
      "${PROTO_PATH}")

endforeach()

#============================================================================
# add custom target to force .proto compilation

set_source_files_properties(
  ${gen_headers}
  ${gen_sources}
  ${gen_ruby_scripts}
  ${gen_py_scripts}
  PROPERTIES GENERATED TRUE
)

foreach(gen_py_script ${gen_py_scripts})
  get_filename_component(FIL_WE ${gen_py_script} NAME_WE)
  add_custom_target(${FIL_WE} ALL DEPENDS ${gen_py_script})
endforeach()
