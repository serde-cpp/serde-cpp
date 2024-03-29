include(CMakeParseArguments)

#########################################################################################
# Function to generate serde headers for source files.
# serde_generate_target(<target> [SUFFIX <suffix>])
# ARGS:
#   SUFFIX default: "_serde.h"
#   OUTPUT_DIRECTORY default: "${CMAKE_CURRENT_BINARY_DIR}/"
#########################################################################################
function(serde_generate_target TARGET)

  # Parse arguments
  set(prefix ARG)
  set(flags)
  set(singleValues SUFFIX OUTPUT_DIRECTORY)
  set(multiValues)
  cmake_parse_arguments(PARSE_ARGV 1 "${prefix}" "${flags}" "${singleValues}" "${multiValues}")

  # Args validation and default values
  if(NOT DEFINED ARG_UNPARSED_ARGUMENTS OR ARG_UNPARSED_ARGUMENTS STREQUAL "")
    message(FATAL_ERROR "TARGET ${TARGET}: missing sources")
  endif()
  set(ARG_SOURCES ${ARG_UNPARSED_ARGUMENTS})

  if(NOT DEFINED ARG_SUFFIX OR ARG_SUFFIX STREQUAL "")
    set(ARG_SUFFIX "_serde.h")
  endif()

  if(NOT DEFINED ARG_OUTPUT_DIRECTORY OR ARG_OUTPUT_DIRECTORY STREQUAL "")
    set(ARG_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
  endif()

  # Generate new list of serde header file names
  foreach(FILE IN LISTS ARG_SOURCES)
    # Get file absolute path
    get_filename_component(FILE_ABSPATH ${FILE} ABSOLUTE)
    # Validate path within current source dir
    cmake_path(IS_PREFIX CMAKE_CURRENT_SOURCE_DIR "${FILE_ABSPATH}" NORMALIZE IS_PREFIX)
    if(NOT IS_PREFIX)
      message(FATAL_ERROR "${TARGET}: ${FILE} is external to current directory")
    endif()
    # Get file relative path
    cmake_path(RELATIVE_PATH FILE_ABSPATH OUTPUT_VARIABLE FILE_REALPATH)
    # Check if file exists (throws error)
    file(READ "${FILE_ABSPATH}" FILE_READ)

    # Add "_serde.h" suffix to source file
    string(REGEX REPLACE "(.+)[.][^.]+$" "\\1${ARG_SUFFIX}" SERDE_HEADER ${FILE_REALPATH})
    if("${SERDE_HEADER}" STREQUAL "${FILE_REALPATH}")
      string(CONCAT SERDE_HEADER "${FILE_REALPATH}" "${ARG_SUFFIX}")
    endif()
    string(PREPEND SERDE_HEADER "${ARG_OUTPUT_DIRECTORY}/")

    # Add file to control lists
    list(APPEND SERDE_HEADERS ${SERDE_HEADER})
    list(APPEND SOURCES ${FILE_ABSPATH})
  endforeach()

  # Export target's serde headers variable
  set("${TARGET}_SERDE_FILES" ${SERDE_HEADERS} PARENT_SCOPE)

  # Get length of the list of files
  list(LENGTH SERDE_HEADERS LENGTH)
  math(EXPR MAX_IDX "${LENGTH} - 1")

  # Generate a serde header for each source file
  foreach(IDX RANGE ${MAX_IDX})
    list(GET SERDE_HEADERS ${IDX} SERDE_HEADER)
    list(GET SOURCES ${IDX} SOURCE)
    add_custom_command(
      OUTPUT ${SERDE_HEADER}
      COMMAND $<TARGET_FILE:serde_gen>
                --source=${SOURCE}
                --output=${SERDE_HEADER}
                --database_dir=${CMAKE_BINARY_DIR}
                --database_file=compile_commands.json
                --include_directory=${ARG_OUTPUT_DIRECTORY}
                --verbose
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      DEPENDS serde_gen ${SOURCE})
  endforeach()

  # Build dependency target
  add_custom_target("${TARGET}_custom" ALL DEPENDS ${SERDE_HEADERS})

  # Final target with include dir exported
  add_library(${TARGET} INTERFACE IMPORTED)
  target_include_directories(${TARGET} INTERFACE ${ARG_OUTPUT_DIRECTORY})
  add_dependencies(${TARGET} "${TARGET}_custom")

endfunction(serde_generate_target)
