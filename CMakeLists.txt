cmake_minimum_required(VERSION 3.0)

project(openglapp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#if build type is not defined then use debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)")
endif(NOT CMAKE_BUILD_TYPE)

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)    
endif(WIN32)


# find the required packages

# OpenGL
find_package(OpenGL REQUIRED)
# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")


# GLFW3
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

if(APPLE)
    find_package(Freetype REQUIRED)
    message(STATUS "Found Freetype in {FREETYPE_INCLUDE_DIRS}")
endif(APPLE)

if(WIN32)
    set(LIBS glfw3 opengl32 freetype)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
    find_package(OpenGL REQUIRED)
    add_definitions(${OPENGL_DEFINITIONS})
    find_package(X11 REQUIRED)
    set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerema Xi Xxf86vm Xcursor GL dl pthread freetype)
    set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
else()
    set(LIBS)
endif(WIN32)

set(CHAPTERS
    1.getting_started)

set(1.getting_started
    1.1.triangle
    4.1.textures
    4.2.textures_combined
    5.1.transformations)


add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

function(create_project_from_sources chapter demo)
    
    # copy source files
    file(GLOB SOURCE
            "src/${chapter}/${demo}/*.h"
            "src/${chapter}/${demo}/*.cpp"
    )
    if(demo STREQUAL "")
        set(replaced "")
        string(REPLACE "/" "_" replaced ${chapter})
    else()
        set(NAME "${chapter}__${demo}")
    endif()
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} ${LIBS})
    if(WIN32)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
        set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}/Debug")
    elseif(UNIX AND NOT APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
    endif(WIN32)
    
    # copy shader files to build directory
    file(GLOB SHADERS
        "src/${chapter}/${demo}/*.vs"
        "src/${chapter}/${demo}/*.fs"
    )
    foreach(SHADER ${SHADERS})
        if(WIN32)
        # configure_file(${SHADER} "test")
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
        elseif(UNIX AND NOT APPLE)
            file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/${chapter})
        endif(WIN32)
    endforeach(SHADER)
endfunction()

# create a project file per tutorial
foreach(CHAPTER ${CHAPTERS})
    foreach(DEMO ${${CHAPTER}})
        create_project_from_sources(${CHAPTER} ${DEMO})
    endforeach(DEMO)
endforeach(CHAPTER)
include_directories(${CMAKE_SOURCE_DIR}/includes)