cmake_minimum_required( VERSION 3.31 )

project(
  inOneWeekend
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(thirdparty/glm)

add_executable(
  ${PROJECT_NAME}
  main.cpp
)

target_compile_features(
  ${PROJECT_NAME} 
  PRIVATE 
  cxx_std_17)

target_include_directories(
  ${PROJECT_NAME} 
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include/
  ${CMAKE_SOURCE_DIR}/thirdparty/glm/
)

target_link_libraries(
  ${PROJECT_NAME} 
  PRIVATE
  glm
)
