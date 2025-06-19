set(THALIA_SYNTAX_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(THALIA_SYNTAX_TST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(THALIA_SYNTAX_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/public")

file(
  GLOB THALIA_SYNTAX_PUBLIC
  "${THALIA_SYNTAX_INC_DIR}/*.hpp"
)

file(
  GLOB THALIA_SYNTAX_SOURCES
  "${THALIA_SYNTAX_SRC_DIR}/*.cpp"
  "${THALIA_SYNTAX_SRC_DIR}/**/*.cpp"
)

file(
  GLOB THALIA_SYNTAX_TESTS
  "${THALIA_SYNTAX_TST_DIR}/*.cpp"
  "${THALIA_SYNTAX_TST_DIR}/**/*.cpp"
)

find_package(Catch2 CONFIG REQUIRED)

add_library(thalia-syntax "${THALIA_SYNTAX_SOURCES}")
target_include_directories(thalia-syntax PRIVATE "${THALIA_SYNTAX_SRC_DIR}")
target_include_directories(thalia-syntax PUBLIC "${THALIA_SYNTAX_INC_DIR}")

add_executable(thalia-syntax-test "${THALIA_SYNTAX_TESTS}")
target_include_directories(thalia-syntax-test PRIVATE)
target_link_libraries(thalia-syntax-test PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(thalia-syntax-test PRIVATE thalia-syntax)

install(FILES ${THALIA_SYNTAX_PUBLIC} DESTINATION include/thalia-syntax)
install(TARGETS thalia-syntax ARCHIVE DESTINATION lib)

