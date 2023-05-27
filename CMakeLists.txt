cmake_minimum_required(VERSION 3.25)
project(Vulkan-Tut)

set(CMAKE_CXX_STANDARD 23)

add_executable(Vulkan-Tut main.cpp)
target_link_libraries(Vulkan-Tut glfw ${GLFW_LIBRARIES})

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

add_subdirectory(lib/glm)
target_link_libraries(${PROJECT_NAME} glm)

add_subdirectory(lib/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

include_directories(lib/stb)
include_directories(lib/tinyobjloader)

set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")

add_custom_target(Textures ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/tex ${PROJECT_BINARY_DIR}/tex
)

add_custom_target(Models ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/mdl ${PROJECT_BINARY_DIR}/mdl
)
file(GLOB_RECURSE GLSL_SOURCE_FILES
        "shd/*.frag"
        "shd/*.vert"
        )

foreach (GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_BINARY_DIR}/shd/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shd/"
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach (GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(Vulkan-Tut Shaders)
add_dependencies(Vulkan-Tut Textures)
add_dependencies(Vulkan-Tut Models)

add_custom_command(TARGET Vulkan-Tut POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Vulkan-Tut>/shd/"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Vulkan-Tut>/tex/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/shd" "$<TARGET_FILE_DIR:Vulkan-Tut>/shd"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/tex" "$<TARGET_FILE_DIR:Vulkan-Tut>/tex"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/mdl" "$<TARGET_FILE_DIR:Vulkan-Tut>/mdl"
        )
