cmake_minimum_required(VERSION 3.10)

project(SimpleGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

set(SOURCES 
  main.cpp 
  window.cpp
  transform.cpp
  program.cpp
  utils.cpp
  gameObject.cpp
  mesh.cpp
  camera.cpp
  scene.cpp
  examples/exampleObject.h
  examples/exampleScene.h
)

set(HEADERS 
  window.h
  transform.h
  program.h
  gameObject.h
  utils.h
  mesh.h
  camera.h
  vertex.h
  ray.h
  scene.h
  examples/exampleObject.cpp
  examples/exampleScene.cpp
)

add_executable(Renderer ${SOURCES} ${HEADERS})

target_include_directories(
  Renderer PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/examples
  )

target_link_libraries(Renderer PRIVATE
  glfw
  OpenGL::GL
  GLEW::GLEW
)