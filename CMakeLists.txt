cmake_minimum_required(VERSION 3.25)
project(DesirePaths)

# Web Configuration
if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

set(CMAKE_CXX_STANDARD 23)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # or games
add_subdirectory(thirdparty/raylib/)

# RayGUI (Assumes source is in thirdparty/raygui/src)
set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build raygui examples
add_subdirectory(thirdparty/raygui/projects/CMake)

# LDtkLoader
add_subdirectory(thirdparty/LDtkLoader)

# Project
add_executable(${PROJECT_NAME} src/main.cpp
        src/TileGrid.cpp
        src/TileGrid.h
        src/Pathfinder.cpp
        src/Pathfinder.h
        src/Grid.h
        src/Agent.cpp
        src/Agent.h
        src/AgentSpawner.cpp
        src/AgentSpawner.h
        src/TreeRenderer.cpp
        src/TreeRenderer.h)
target_link_libraries(${PROJECT_NAME} raylib raygui LDtkLoader::LDtkLoader)

if ("${PLATFORM}" STREQUAL "Web")
    add_custom_command(
            TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
    #DEPENDS ${PROJECT_NAME}
else()
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
    #DEPENDS ${PROJECT_NAME}
endif()

# Web Configurations
if ("${PLATFORM}" STREQUAL "Web")
    target_link_options(${PROJECT_NAME} PUBLIC -sUSE_GLFW=3 PUBLIC -sALLOW_MEMORY_GROWTH PUBLIC --preload-file resources)
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()