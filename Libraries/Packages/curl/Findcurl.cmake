set(LIB_NAME "curl")
set(TARGET_WITH_NAMESPACE "3rdParty::${LIB_NAME}")
# Short circuit if this file was invoked while building
# the target that it defines
if (TARGET ${TARGET_WITH_NAMESPACE})
	return()
endif()

# add_library(${TARGET_WITH_NAMESPACE} INTERFACE IMPORTED GLOBAL)

set(${LIB_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/${LIB_NAME}/include)
set(${LIB_NAME}_LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/${LIB_NAME}/lib/${CMAKE_SYSTEM_NAME})

if (${PAL_PLATFORM_NAME} STREQUAL "Windows")
    file(GLOB ${LIB_NAME}_LIBRARY ${${LIB_NAME}_LIBS_DIR}/*.lib)
endif()

add_library(${TARGET_WITH_NAMESPACE} INTERFACE IMPORTED GLOBAL)

ly_add_target_files(TARGETS ${TARGET_WITH_NAMESPACE} FILES ${${LIB_NAME}_LIB_DIR})
ly_target_include_system_directories(TARGET ${TARGET_WITH_NAMESPACE} INTERFACE ${${LIB_NAME}_INCLUDE_DIR})
set(${LIB_NAME}_FOUND True)