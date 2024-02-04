cmake_minimum_required(VERSION 3.5)

project(openglapp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
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
message(STATUS "Found GLM in ${GLM_INCLUDE_DIRS}")

find_package(assimp REQUIRED)
message(STATUS "Found assimp in ${ASSIMP_INCLUDE_DIRS}")
# GLFW3
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIRS}")

if(APPLE)
    find_package(Freetype REQUIRED)
    message(STATUS "Found Freetype in {FREETYPE_INCLUDE_DIRS}")
endif(APPLE)

if(WIN32)
    set(LIBS glfw3 opengl32 freetype ${ASSIMP_LIBRARIES})
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
    find_package(OpenGL REQUIRED)
    add_definitions(${OPENGL_DEFINITIONS})
    find_package(X11 REQUIRED)
    set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerema Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARIES})
    set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
endif(WIN32)

set(CHAPTERS
    1.getting_started
    2.lighting
    3.model_loading
    4.advanced_opengl
    5.advanced_lighting
    )

set(1.getting_started
    1.1.triangle
    4.1.textures
    4.2.textures_combined
    5.1.transformations
    6.1.coordinate_system
    7.1.camera
    )

set(2.lighting
    1.colors
    2.1.basic_lighting_ambient
    2.2.basic_lighting_diffuse
    2.3.materials
    2.4.maps
    2.5.directional_light
    2.6.point_light
    2.7.spot_light
    2.8.multiple_lights
    )

set(3.model_loading
    3.1.model_loading
    )
set(4.advanced_opengl
    4.1.depth_testing
    4.2.stencil_testing
    4.3.blending
    4.3.blending-func
    4.4.face_culling
    4.5.frame_buffers
    4.6.post_processing
    4.7.skybox
    4.8.glsl_ubo
    4.9.geometry_shader
    4.10.normal_visualization
    4.11.instancing
    4.12.instanced_arrays
    4.13.asteroids
    4.14.instanced_asteroids
    4.15.msaa
    4.16.msaa_background
    )
set(5.advanced_lighting
    5.1.blinn_phong
    5.2.gamma_correction
    5.3.1.shadow_mapping_depth
    5.3.2.shadow_mapping_base
    5.4.point_shadows
    5.5.1.normal_mapping
    5.5.2.normal_mapping_model
    5.6.1.parallax_mapping
    5.6.2.steep_parallax_mapping
    )
configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)
add_library(STB_IMAGE "includes/vendor/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "includes/vendor/glad/glad.c")
set(LIBS ${LIBS} GLAD)


# copy resources folder
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_SOURCE_DIR}/bin)
# copy shader folder
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_SOURCE_DIR}/bin)


function(create_project_from_sources chapter demo)
    
    # copy source files
    file(GLOB SOURCE
            "src/${chapter}/${demo}/*.h"
            "src/${chapter}/${demo}/*.cpp"
            "src/${chapter}/${demo}/*.vs"
            "src/${chapter}/${demo}/*.fs"
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
        if(CMAKE_BUILD_TYPE MATCHES Debug)
            set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/debug/${chapter}")
            set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/debug/{chapter}")
        elseif(CMAKE_BUILD_TYPE MATCHES Release)
            set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/release/${chapter}")
            set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/release/${chapter}")
        endif(CMAKE_BUILD_TYPE MATCHES Debug)

    elseif(UNIX AND NOT APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
    endif(WIN32)
    
    # # copy shader files to build directory
    # file(GLOB SHADERS
    #     "src/${chapter}/${demo}/*.vs"
    #     "src/${chapter}/${demo}/*.fs"
    # )
    # foreach(SHADER ${SHADERS})
    #     if(WIN32)
    #         add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
    #     elseif(UNIX AND NOT APPLE)
    #         file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/${chapter})
    #     endif(WIN32)
    # endforeach(SHADER)
endfunction()

# create a project file per tutorial
foreach(CHAPTER ${CHAPTERS})
    foreach(DEMO ${${CHAPTER}})
        create_project_from_sources(${CHAPTER} ${DEMO})
    endforeach(DEMO)
endforeach(CHAPTER)
include_directories(${CMAKE_SOURCE_DIR}/includes
                    ${CMAKE_SOURCE_DIR}/includes/vendor)

