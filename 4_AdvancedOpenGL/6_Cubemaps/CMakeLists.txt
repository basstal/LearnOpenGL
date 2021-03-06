cmake_minimum_required(VERSION 3.10)

set(ROOT_PATH 
    "${CMAKE_CURRENT_SOURCE_DIR}/../..")
get_filename_component(ROOT_PATH ${ROOT_PATH} REALPATH)
# message("${ROOT_PATH}")

set(PROJECT_NAME "Cubemaps")

# Project name
project(${PROJECT_NAME})

# lib source glad
add_library(glad
    "${ROOT_PATH}/Plugins/glad/glad.c")

target_include_directories(glad PUBLIC
    "${ROOT_PATH}/Plugins")

# lib source imgui
add_library(imgui
    "${ROOT_PATH}/Plugins/imgui/imgui.cpp"
    "${ROOT_PATH}/Plugins/imgui/imgui_widgets.cpp"
    "${ROOT_PATH}/Plugins/imgui/imgui_draw.cpp"
    "${ROOT_PATH}/Plugins/imgui/imgui_demo.cpp"
    "${ROOT_PATH}/Plugins/imgui/imgui_impl_glfw.cpp"
    "${ROOT_PATH}/Plugins/imgui/imgui_impl_opengl3.cpp")

target_include_directories(imgui PUBLIC
    "${ROOT_PATH}/Plugins/imgui/"
    "${ROOT_PATH}/Plugins/glfw-3.3.4/include"
    "${ROOT_PATH}/Plugins")

# project source
add_executable(${PROJECT_NAME} 
    Main.cpp
    Cubemaps.cpp
    Reflection.cpp
    Refraction.cpp
    "${ROOT_PATH}/Scripts/Shader.cpp"
    "${ROOT_PATH}/Scripts/Utils.cpp"
    "${ROOT_PATH}/Scripts/Model.cpp"
    "${ROOT_PATH}/Scripts/Mesh.cpp"
    "${ROOT_PATH}/Scripts/STBImage.cpp")

target_link_libraries(${PROJECT_NAME}
    "${ROOT_PATH}/Plugins/glfw-3.3.4/lib-vc2019/glfw3_mt.lib"
    "${ROOT_PATH}/Plugins/assimp-5.0.1/lib/assimp-vc142-mtd.lib"
    "${ROOT_PATH}/Plugins/assimp-5.0.1/lib/zlibstaticd.lib"
    "${ROOT_PATH}/Plugins/assimp-5.0.1/lib/IrrXMLd.lib"
    glad
    imgui)

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${ROOT_PATH}/Plugins/glfw-3.3.4/include"
    "${ROOT_PATH}/Plugins/imgui"
    "${ROOT_PATH}/Plugins/assimp-5.0.1/include"
    "${ROOT_PATH}/Plugins"
    "${ROOT_PATH}/Scripts/include")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD                    # Adds a post-build event to project
    COMMAND ${CMAKE_COMMAND} -E copy_if_different                       # which executes "cmake - E copy_if_different..."
    "${ROOT_PATH}/Plugins/assimp-5.0.1/lib/assimp-vc142-mtd.dll"        # <--this is in-file
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)                                 # <--this is out-file path
    
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT.LIB")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} /wd4244") # Suppress 'warning C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data'
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} /wd4305") # Suppress 'warning C4244: 'argument': truncation from 'double' to 'float''
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG
        "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT.LIB")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
endif()