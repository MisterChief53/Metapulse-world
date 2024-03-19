add_definitions(-DCURL_STATICLIB)
set(LIB_NAME "curl")
set(TARGET_WITH_NAMESPACE "3rdParty::${LIB_NAME}")

# Short circuit if this file was invoked while building
# the target that it defines
if (TARGET ${TARGET_WITH_NAMESPACE})
    return()
endif()

set(${LIB_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/${LIB_NAME}/include)
set(${LIB_NAME}_LIBS_DIR ${CMAKE_CURRENT_LIST_DIR}/${LIB_NAME}/lib/${CMAKE_SYSTEM_NAME}/)

# Specify the import library file
if (MSVC)
    set(${LIB_NAME}_LIBRARY ${CMAKE_CURRENT_LIST_DIR}/curl/lib/Windows/libcurl_imp.lib)
else()
    # Handle other platforms here if necessary
endif()

# Specify the DLL file
set(${LIB_NAME}_DLL ${CMAKE_CURRENT_LIST_DIR}/curl/bin/libcurl.dll)

# this is a shared library, ie, a dll
add_library(${TARGET_WITH_NAMESPACE} SHARED IMPORTED GLOBAL)

set_target_properties(${TARGET_WITH_NAMESPACE}
    PROPERTIES
        IMPORTED_LOCATION ${${LIB_NAME}_DLL} # where the dll is
        IMPORTED_LOCATION_PROFILE ${${LIB_NAME}_DLL}
        #IMPORTED_LOCATION_DEBUG XXXXX # optional - if theres a differnt one in debug
        #IMPORTED_LOCATION_RELEASE XXXX # also optional but for release
        IMPORTED_LIB ${${LIB_NAME}_LIBRARY} # where the import library file is (ignored on non-windows)
        IMPORTED_IMPLIB_PROFILE ${${LIB_NAME}_LIBRARY}
        #IMPORTED_IMPLIB_DEBUG xxxxxxxxxxx # optional line, for if theres a different one in debug
)
# anyone who depends on this library should get thes include dir added to their include list.
# treat it as a system directory.  This has an impact on intellisense, code compilation speed, etc, 
# as it will assume it won't change very often.
ly_target_include_system_directories(TARGET ${TARGET_WITH_NAMESPACE} INTERFACE ${${LIB_NAME}_INCLUDE_DIR})

set(${LIB_NAME}_FOUND True)
