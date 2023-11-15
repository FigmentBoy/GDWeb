cmake_minimum_required(VERSION 3.27.6)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(WebEditor)

file(GLOB SOURCES 
    "src/*.cpp"
    "include/stb/*.cpp"
    "include/plist/*.cpp" 
    "include/libdeflate/lib/*.c"
    "include/libdeflate/lib/**/*.c"
    "include/imgui/*.cpp"
    "include/imgui/backends/imgui_impl_glfw.cpp"
    "include/imgui/backends/imgui_impl_opengl3.cpp"
    "include/easing-functions/src/*.cpp"
)

if(EMSCRIPTEN)
    configure_file("${CMAKE_SOURCE_DIR}/dist/WebEditor.html" "${CMAKE_SOURCE_DIR}/build/WebEditor.html" COPYONLY)
endif()

add_executable(WebEditor ${SOURCES})

file(GLOB RESOURCES 
    "static/**/*"
)

if (EMSCRIPTEN)
    target_compile_options(WebEditor PUBLIC -pthread)

    set_target_properties(WebEditor PROPERTIES LINK_DEPENDS "${RESOURCES}")

    # We cannot use offscreen canvas with pthreads
    set_target_properties(WebEditor PROPERTIES LINK_FLAGS "${LINK_FLAGS} 
        -sALLOW_MEMORY_GROWTH=1 
        -sWASM=1 -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=2 
        -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sUSE_WEBGL2=1 
        -sUSE_GLFW=3 
        --preload-file ../static"
    )
else()
    find_package(OpenGL REQUIRED)
    add_subdirectory(include/glfw)

    target_link_libraries(WebEditor PRIVATE 
        glfw
        glew32
        OpenGL32
    )

    add_custom_command(
        TARGET WebEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${CMAKE_SOURCE_DIR}/static
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/static
    )
endif()


target_include_directories(WebEditor PRIVATE 
    src
    include
    include/stb
    include/glm
    include/plist
    include/json/include
    include/libdeflate
    include/imgui
    include/glfw/include
    include/easing-functions/src
)

add_subdirectory(include/json)