#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Boost::process" for configuration "Debug"
set_property(TARGET Boost::process APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Boost::process PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/debug/lib/boost_process-vc143-mt-gd-x64-1_86.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/bin/boost_process-vc143-mt-gd-x64-1_86.dll"
  )

list(APPEND _cmake_import_check_targets Boost::process )
list(APPEND _cmake_import_check_files_for_Boost::process "${_IMPORT_PREFIX}/debug/lib/boost_process-vc143-mt-gd-x64-1_86.lib" "${_IMPORT_PREFIX}/debug/bin/boost_process-vc143-mt-gd-x64-1_86.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
