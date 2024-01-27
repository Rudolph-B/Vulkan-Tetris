cmake_minimum_required(VERSION 3.25)
project(Vulkan-Tetris)

set(CMAKE_CXX_STANDARD 23)

add_executable(Vulkan-Tetris main.cpp
        src/window.cpp
        src/window.h
        src/engine.cpp
        src/engine.h
        src/structs.h
        src/constants.h
            src/sce/tetris.cpp
            src/sce/tetris.h

            src/inp/keyboard.cpp
            src/inp/keyboard.h
            src/inp/gamepad.cpp
            src/inp/gamepad.h
        src/scene.cpp
        src/scene.h 
            src/sce/intro.cpp
            src/sce/intro.h

            src/tls/svg.cpp
            src/tls/svg.h
)
target_link_libraries(Vulkan-Tetris glfw ${GLFW_LIBRARIES})

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

# GLM
add_subdirectory(lib/glm)
target_link_libraries(${PROJECT_NAME} glm)

# GLFW
add_subdirectory(lib/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

# TINY-XML-2
add_subdirectory(lib/tinyxml2)
target_link_libraries(${PROJECT_NAME} tinyxml2)

# ---------------------- SHADERS ----------------------
if (NOT LINUX)
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
else()
    set(GLSL_VALIDATOR "glslangValidator")
endif()

# Copies the assets to the build directory
add_custom_target(Assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/ass ${PROJECT_BINARY_DIR}/ass
)

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "shd/*.frag"
    "shd/*.vert"
)

# Complies shaders on change
foreach (GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_BINARY_DIR}/shd/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shd/"
        COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})

endforeach (GLSL)

# Adds the shaders as a project target
add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)

# Adds the shaders as a dependency of the main project
add_dependencies(Vulkan-Tetris Shaders)
add_dependencies(Vulkan-Tetris Assets)

# Copies the shaders to the build directory
add_custom_command(TARGET Vulkan-Tetris POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Vulkan-Tetris>/shd/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/shd" "$<TARGET_FILE_DIR:Vulkan-Tetris>/shd"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Vulkan-Tetris>/ass/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/ass" "$<TARGET_FILE_DIR:Vulkan-Tetris>/ass"
)
