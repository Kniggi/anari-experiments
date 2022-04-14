#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "pbrtParser_shared" for configuration "Debug"
set_property(TARGET pbrtParser_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(pbrtParser_shared PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libpbrtParser_shared.so.2.4.1"
  IMPORTED_SONAME_DEBUG "libpbrtParser_shared.so.2.4.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS pbrtParser_shared )
list(APPEND _IMPORT_CHECK_FILES_FOR_pbrtParser_shared "${_IMPORT_PREFIX}/lib/libpbrtParser_shared.so.2.4.1" )

# Import target "pbrtParser" for configuration "Debug"
set_property(TARGET pbrtParser APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(pbrtParser PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C;CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libpbrtParser.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS pbrtParser )
list(APPEND _IMPORT_CHECK_FILES_FOR_pbrtParser "${_IMPORT_PREFIX}/lib/libpbrtParser.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
