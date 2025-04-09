set(THALIA_SYNTAX_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
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

add_library(thalia-syntax "${THALIA_SYNTAX_SOURCES}")
target_include_directories(thalia-syntax PRIVATE "${THALIA_SYNTAX_SRC_DIR}")
target_include_directories(thalia-syntax PUBLIC "${THALIA_SYNTAX_INC_DIR}")

install(FILES ${THALIA_SYNTAX_PUBLIC} DESTINATION include/thalia-syntax)
install(TARGETS thalia-syntax ARCHIVE DESTINATION lib)

