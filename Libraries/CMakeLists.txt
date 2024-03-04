# this cmake file is tasked with building all libraries,
# so that then they get added as 3rd party packages by
# their respective Find<package_name>.cmake files

cmake_minimum_required(VERSION 3.20)

project(
	curl-extern
	VERSION 0.1.0
	LANGUAGES CXX
)

# tell cmake to have our binaries where we need them
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/Packages/curl/curl)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_SYSTEM_NAME})

# force a profile build config. We do not need anything
# else for now
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

# build and "install" libcurl
add_subdirectory(curl)

# Run post-build script, which will copy relevant
# files to be used by each package's cmake files.
message("Declaring post install script")
install (SCRIPT "${CMAKE_CURRENT_LIST_DIR}/PostBuild.cmake")