cmake_minimum_required(VERSION 3.1)
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
    "data/*.scn"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") #-DCMAKE_BUILD_TYPE=Debug
    else ()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") #-DCMAKE_BUILD_TYPE=Release
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${SCENES_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DPLUGIN_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")
target_link_libraries(${PROJECT_NAME} glut GL GLU GLEW png Qt5::Core Qt5::Gui microSofaImpl)

