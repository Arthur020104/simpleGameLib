cmake_minimum_required(VERSION 3.10)

project(SimpleGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-pg)
add_link_options(-pg)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

set(SOURCES 
  ui.cpp
  main.cpp 
  window.cpp
  transform.cpp
  program.cpp
  utils.cpp
  gameObject.cpp
  mesh.cpp
  camera.cpp
  scene.cpp
  examples/exampleObject.cpp
  examples/exampleScene.cpp
  examples/exampleCamera.cpp
  meshBvhNode.cpp
  ray.cpp
  light.cpp
  material.cpp
  texture.cpp
  cubeMap.cpp
  queueItem.cpp
  timeQueue.cpp
  fpsCamera.cpp
)

set(HEADERS
  ui.h
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
  component.h
  examples/exampleObject.h
  examples/exampleScene.h
  examples/exampleCamera.h
  meshBvhNode.h
  light.h
  material.h
  texture.h
  cubeMap.h
  queueItem.h
  timeQueue.h
  fpsCamera.h
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