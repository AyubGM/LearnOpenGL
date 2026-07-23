cmake_minimum_required(VERSION 3.24)
project(OpenGL LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files
file(GLOB_RECURSE SOURCES 
CONFIGURE_DEPENDS
"src/*.cpp" 
"src/*.c" 
"vendors/*.c"
"vendors/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

# --- BUILD ASSIMP WITH YOUR PROJECT ---
# Force Assimp to build statically so we don't need DLLs
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# --- BUILD ASSIMP WITH YOUR PROJECT ---
# Turn off Assimp extras (tests/tools) to speed up your build time
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

# Add the assimp folder so CMake builds it automatically
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/assimp)

add_executable(${PROJECT_NAME} ${SOURCES})

# --- Include directories ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src        # for #include "Shader.h" etc.

    # Assimp source headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assimp/include 
    
    # Assimp generated headers (like config.h) which live in the build output folder
    ${CMAKE_CURRENT_BINARY_DIR}/include/assimp/include
)

# --- Link Directories ---
# Point to where the .lib files actually are
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GLFW/lib
)

# --- Link libraries ---
target_link_libraries(${PROJECT_NAME} PRIVATE
    assimp
    glfw3
    opengl32
)

# --- Windows-specific ---
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

# Copy shaders to output directory on build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders
)

# Copy textures to output directory on build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/Resources/Textures $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources/Textures
)