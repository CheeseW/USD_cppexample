# 1. Requirements
cmake_minimum_required(VERSION 2.8.11)

project(authoringVariants)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2.Configuration
add_compile_options(-Wno-deprecated)  # This is just to avoid some spammy warnings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # This line is optional. It's just for certain IDEs to use

# 3. Boilerplate
set(USD_LIBRARY_DIRECTORY $ENV{USD_INSTALL_ROOT}/lib)
set(USD_INCLUDE_DIRECTORY $ENV{USD_INSTALL_ROOT}/include)

find_library(USD_BOOST_PYTHON boost_python boost_python37 REQUIRED HINTS ${USD_LIBRARY_DIRECTORY})
find_library(USD_GF gf REQUIRED HINTS ${USD_LIBRARY_DIRECTORY})
find_library(USD_SDF sdf REQUIRED HINTS ${USD_LIBRARY_DIRECTORY})
find_library(USD_TF tf REQUIRED HINTS ${USD_LIBRARY_DIRECTORY})
find_library(USD_USD usd REQUIRED HINTS ${USD_LIBRARY_DIRECTORY})
find_library(USD_USDGEOM usdGeom REQUIRED HINTS ${USD_LIBRARY_DIRECTORY})
find_library(USD_VT vt REQUIRED HINTS ${USD_LIBRARY_DIRECTORY})

find_package(PythonLibs REQUIRED)

# 4. Include/Link Everything
add_executable(${CMAKE_PROJECT_NAME}
  main.cpp
  )

target_include_directories(${CMAKE_PROJECT_NAME}
  PUBLIC
  ${PYTHON_INCLUDE_PATH}
  ${USD_INCLUDE_DIRECTORY}
  )

target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  ${PYTHON_LIBRARY}
  ${USD_BOOST_PYTHON}
  ${USD_GF}
  ${USD_SDF}
  ${USD_TF}
  ${USD_USDGEOM}
  ${USD_USD}
  ${USD_VT}
  )

add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/HelloWorld.usda
        ${CMAKE_CURRENT_BINARY_DIR}/HelloWorld.usda
        )
