cmake_minimum_required(VERSION 3.5)
project(LatticePhysics)

execute_process(WORKING_DIRECTORY GameEngine
        COMMAND make)

include_directories("GameEngine/include")
link_directories("GameEngine/lib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("include")

set(SOURCE_FILES
    include/BallManager.h
    include/MainGame.h
    include/ObjectLoader.h
    src/BallManager.cpp
    src/main.cpp
    src/MainGame.cpp
    src/ObjectLoader.cpp)

add_executable(LatticePhysics ${SOURCE_FILES})

find_library(OPENGL_LIB OpenGL)
target_link_libraries(LatticePhysics "gameEngine" "SDL2" "SDL2_ttf" "SDL2_mixer" ${OPENGL_LIB})

