cmake_minimum_required(VERSION 3.27.6)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(WebEditor)

file(GLOB SOURCES 
    "src/*.cpp"
    "include/stb/*.cpp"
    "include/plist/*.cpp" 
    "include/libdeflate/lib/**.c"
    "include/imgui/*.cpp"
    "include/imgui/backends/imgui_impl_glfw.cpp"
    "include/imgui/backends/imgui_impl_opengl3.cpp"
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    configure_file("${CMAKE_SOURCE_DIR}/dist/WebEditor.html" "${CMAKE_SOURCE_DIR}/build/WebEditor.html" COPYONLY)
else()
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

add_executable(WebEditor ${SOURCES})

file(GLOB RESOURCES 
    "static/**/*"
)

target_compile_options(WebEditor PUBLIC -pthread)

set_target_properties(WebEditor PROPERTIES LINK_DEPENDS "${RESOURCES}")

# We cannot use offscreen canvas with pthreads
set_target_properties(WebEditor PROPERTIES LINK_FLAGS "${LINK_FLAGS} 
    -sALLOW_MEMORY_GROWTH 
    -sWASM=1 -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=4 
    -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sUSE_WEBGL2=1 
    -sFULL_ES3=1 -sUSE_GLFW=3 
    --preload-file ../static"
)

target_include_directories(WebEditor PRIVATE 
    src
    include
    include/stb
    include/glm
    include/plist
    include/json/include
    include/libdeflate
    include/imgui
)

add_subdirectory(include/json)