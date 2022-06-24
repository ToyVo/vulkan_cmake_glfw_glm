cmake_minimum_required(VERSION 3.20)
project(lve)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan REQUIRED)

set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)
file(GLOB SHADERS ${SHADER_SOURCE_DIR}/*.vert ${SHADER_SOURCE_DIR}/*.frag ${SHADER_SOURCE_DIR}/*.comp ${SHADER_SOURCE_DIR}/*.geom ${SHADER_SOURCE_DIR}/*.tesc ${SHADER_SOURCE_DIR}/*.tese ${SHADER_SOURCE_DIR}/*.mesh ${SHADER_SOURCE_DIR}/*.task ${SHADER_SOURCE_DIR}/*.rgen ${SHADER_SOURCE_DIR}/*.rchit ${SHADER_SOURCE_DIR}/*.rmiss)

add_custom_target(make-shader-dir ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_BINARY_DIR})

foreach (SHADER IN LISTS SHADERS)
    get_filename_component(FILENAME ${SHADER} NAME)
    add_custom_command(OUTPUT ${SHADER_BINARY_DIR}/${FILENAME}.spv
            COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${SHADER_BINARY_DIR}/${FILENAME}.spv
            DEPENDS ${SHADER}
            COMMENT "Compiling ${FILENAME}")
    list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILENAME}.spv)
endForeach ()

add_custom_target(shaders ALL DEPENDS ${SPV_SHADERS} make-shader-dir)

add_executable(lve main.cpp ${SHADERS} LveWindow.cpp LveWindow.hpp FirstApp.cpp FirstApp.hpp LvePipeline.cpp LvePipeline.hpp LveDevice.cpp LveDevice.hpp)
add_dependencies(lve shaders)
target_link_libraries(lve glfw Vulkan::Vulkan)
