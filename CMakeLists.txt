cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(
  THALIA
	VERSION 0.0.0
	DESCRIPTION "A fast, statically typed, general-purpose, procedural programming language"
	LANGUAGES CXX
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)

	file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
	if(EXISTS "${LOC_PATH}")
		message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
	endif()

	include(CheckIPOSupported)
	check_ipo_supported(RESULT IPO_SUPPORTED)
	if(IPO_SUPPORTED)
		message(STATUS "Interprocedural optimization supported")
	else()
		message(STATUS "Interprocedural optimization not supported")
	endif()

	if(MSVC)
		add_compile_options(/W4 /WX)
	else()
		add_compile_options(-Wall -Wextra -pedantic -Werror)
	endif()
endif()

add_subdirectory(syntax)

# thalia::thalia
set(THALIA_ROOT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(
  GLOB THALIA_ROOT_SOURCES
  "${THALIA_ROOT_SRC_DIR}/*.cpp"
  "${THALIA_ROOT_SRC_DIR}/**/*.cpp"
)

add_executable(thalia "${THALIA_ROOT_SOURCES}")
target_include_directories(thalia PRIVATE "${THALIA_ROOT_SRC_DIR}")
target_link_libraries(thalia PRIVATE thalia-syntax)
if(IPO_SUPPORTED)
  set_target_properties(thalia PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

install(TARGETS thalia DESTINATION bin)

