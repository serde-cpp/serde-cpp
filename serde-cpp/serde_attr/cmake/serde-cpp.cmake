include(CMakeParseArguments)

#########################################################################################
# serde_generate_target(<target> [SUFFIX <suffix>]
# SUFFIX default: "_serde.h"
#########################################################################################
function(serde_generate_target TARGET)

  # Parse function arguments
  set(prefix ARG)
  set(flags)
  set(singleValues SUFFIX)
  set(multiValues)
  cmake_parse_arguments(PARSE_ARGV 1
    "${prefix}"
    "${flags}"
    "${singleValues}"
    "${multiValues}")

  # Args validation and set default values
  if(NOT DEFINED ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "TARGET ${TARGET}: missing sources")
  endif()
  if(NOT DEFINED ARG_SUFFIX)
    set(ARG_SUFFIX "_serde.h")
  endif()

  set(ARG_SOURCES ${ARG_UNPARSED_ARGUMENTS})

  # Generate list of serde header file names
  foreach(SOURCE IN LISTS ARG_SOURCES)
    get_filename_component(FILEPATH ${SOURCE} ABSOLUTE)
    cmake_path(IS_PREFIX CMAKE_CURRENT_SOURCE_DIR "${FILEPATH}" NORMALIZE IS_PREFIX)
    if(NOT IS_PREFIX)
      message(FATAL_ERROR "${TARGET}: ${FILEPATH} is external to current directory")
    endif()
    cmake_path(RELATIVE_PATH FILEPATH OUTPUT_VARIABLE FILE_REALPATH)
    #message(STATUS "FILE_REALPATH:${FILE_REALPATH}")

    # Add "_serde.h" suffix to source file
    string(REGEX REPLACE "(.+)[.][^.]+$" "\\1${ARG_SUFFIX}" SERDE_HEADER ${FILE_REALPATH})
    if("${SERDE_HEADER}" STREQUAL "${FILE_REALPATH}")
      string(REGEX REPLACE "(.+)" "\\1${ARG_SUFFIX}" SERDE_HEADER ${FILE_REALPATH})
    endif()
    string(PREPEND SERDE_HEADER "${CMAKE_CURRENT_BINARY_DIR}/")

    list(APPEND SERDE_HEADERS ${SERDE_HEADER})
    list(APPEND SOURCES ${FILEPATH})
  endforeach()

  #message(STATUS "${TARGET}_SERDE_HEADERS: ${SERDE_HEADERS}")
  #message(STATUS "${TARGET}_SOURCES: ${SOURCES}")

  # Export targets' serde header to parent scope
  set("${TARGET}_SERDE_FILES" ${SERDE_HEADERS} PARENT_SCOPE)

  # Pre-create the headers for libclang to find them upon #include when parsing the sources.
  file(TOUCH ${SERDE_HEADERS})

  # Command to generate the serde headers
  add_custom_command(
    OUTPUT ${SERDE_HEADERS}
    # TODO: pass suffix to serde_gen
    COMMAND $<TARGET_FILE:serde_gen> --database_dir=${CMAKE_BINARY_DIR} --database_file=compile_commands.json ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS serde_gen ${SOURCES})

  # Dependency target
  add_custom_target("${TARGET}_dep" ALL DEPENDS ${SERDE_HEADERS})

  # Final target with include dir set
  add_library(${TARGET} INTERFACE IMPORTED)
  target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_BINARY_DIR})
  add_dependencies(${TARGET} "${TARGET}_dep")

endfunction(serde_generate_target)

