cmake_minimum_required(VERSION 3.10)
project(Poker VERSION 1.0.0)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(Freetype REQUIRED)

add_executable(Poker
    src/main.c
    src/glad.c
    src/IndexBuffer.c
    src/VertexBuffer.c
    src/Shader.c
    src/Texture.c
    src/GameState.c
    src/App.c
    src/textTexture.c
    src/UserInterface.c
        "src/PlayerDataProcessing.c"
        "src/PlayerDataProcessing.h"
        src/ProcessUserInputs.c
        src/ProcessUserInputs.h
        src/Player.c
        src/Player.h
        src/Macro.h
        src/Game.h
        src/Deck.c
        src/Deck.h
        src/HandSolver.c
        src/HandSolver.h
        src/Prediction.c
        src/Prediction.h
        src/UIColourChange.c
        src/UIColourChange.h
        src/MainMenuFunctions.c
        src/MainMenuFunctions.h
        src/GameLoopFunctions.c
        src/GameLoopFunctions.h

)

target_include_directories(Poker PRIVATE dependencies res ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(Poker ${GLFW3_LIBRARY} ${FREETYPE_LIBRARIES} OpenGL::GL)