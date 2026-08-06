cmake_minimum_required(VERSION 3.18)
project(
    mxgui
    LANGUAGES CXX
)



set(SDL_SHARED          ON CACHE BOOL "" FORCE)
set(SDL_STATIC          OFF CACHE BOOL "" FORCE)
set(SDL_TESTS           OFF CACHE BOOL "" FORCE)
set(SDL_DISABLE_INSTALL ON CACHE BOOL "" FORCE)

# raylib setup
set(BUILD_EXAMPLES      OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES         OFF CACHE BOOL "" FORCE)
set(PLATFORM            SDL CACHE STRING "" FORCE) 


# ===================== FetchContent ===========================
include(FetchContent)
set(FETCHCONTENT_QUIET OFF) # show terminal progress 

# sdl 
FetchContent_Declare(
    sdl
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.10
    GIT_SHALLOW    TRUE
)

# freetype 
FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
    GIT_TAG 5336c0d4da22a13dab3389eb153b12672fdf841c
    GIT_SHALLOW    FALSE
)

# raylib 
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib
    GIT_TAG 6.0
    GIT_SHALLOW    TRUE
    )

# sfml 
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)



FetchContent_MakeAvailable(
    SFML
    sdl
    raylib
    freetype
)


add_executable(${PROJECT_NAME} 
    "main.cpp"
    "mxgui.hpp"
)

if(MSVC)
    target_compile_options(${PROJECT_NAME}  PRIVATE /W4 $<$<CONFIG:Debug>:/fsanitize=address>)
else()
    target_compile_options(${PROJECT_NAME}  PRIVATE -Wall -Wextra -Wempty-body $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer -g>)
    target_link_options(${PROJECT_NAME}  PRIVATE $<$<CONFIG:Debug>:-fsanitize=address>)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:Debug>:DEBUG_MODE>
)

set_target_properties(${PROJECT_NAME} 
    PROPERTIES
    CMAKE_CXX_STANDARD 23
    CMAKE_CXX_STANDARD_REQUIRED ON
    CMAKE_CXX_EXTENSIONS OFF
)

# target_include_directories(${PROJECT_NAME} PUBLIC 
#     "include" 
# )

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    freetype
    m
    SFML::Graphics
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(SCRIPT_MXGUI_SINGLE "${CMAKE_CURRENT_SOURCE_DIR}/build_mxgui_single.py")
set(OUTPUT_SINGLE "${CMAKE_CURRENT_SOURCE_DIR}/mxgui_single.hpp")

add_custom_target(genmxgui
    COMMAND Python3::Interpreter "${SCRIPT_MXGUI_SINGLE}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Generate mxgui_single.hpp..."
    VERBATIM
)


add_dependencies(mxgui genmxgui)








