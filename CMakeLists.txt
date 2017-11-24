# Reference: http://qt-project.org/doc/qt-5.0/qtdoc/cmake-manual.html

cmake_minimum_required(VERSION 2.8)
project(microSofa)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_BUILD_TYPE=Release -O3")
#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_BUILD_TYPE=Debug -g")

file(GLOB_RECURSE HEADER_FILES
    "src/*.h"
    "src/*.inl"
)

file(GLOB_RECURSE SOURCE_FILES
    "src/*.cpp"
)

file(GLOB_RECURSE SCENES_FILES
    "scenes/*.scn"
    "scenes/*.xml"
)

set(OTHER_FILES
    data/config.xml
)

#find_package(Qt5Core REQUIRED)
#find_package(Qt5OpenGL REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

include_directories(/usr/include/libxml2/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${OTHER_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DPLUGIN_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")

#target_link_libraries(${PROJECT_NAME} glut GL GLU dl GLEW png xml2 microSofaFactory microSofaImpl Qt5::Core Qt5::OpenGL)
target_link_libraries(${PROJECT_NAME} glut GL GLU dl GLEW png xml2 microSofaImpl)

