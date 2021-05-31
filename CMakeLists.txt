cmake_minimum_required(VERSION 3.20)
project(danmaku-editor
  VERSION 0.0.1
  LANGUAGES CXX
)

include(FetchContent)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG 7.1.3
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.8.5
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(entt
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(entt)

FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG 3.3.2
)
FetchContent_MakeAvailable(glfw)

set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile" FORCE)
set(GLAD_API     "gl=3.3" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest" FORCE)
FetchContent_Declare(glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad
  GIT_TAG v0.1.33
)
FetchContent_MakeAvailable(glad)

FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG origin/docking
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

file(GLOB imgui_SOURCES
  ${imgui_SOURCE_DIR}/*.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC)
target_sources(imgui PRIVATE ${imgui_SOURCES})
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR}/ ${imgui_SOURCE_DIR}/backends/)
target_link_libraries(imgui PRIVATE glfw glad)

FetchContent_Declare(implot
  GIT_REPOSITORY https://github.com/epezent/implot
  GIT_TAG v0.9
)
FetchContent_GetProperties(implot)
if(NOT implot_POPULATED)
  FetchContent_Populate(implot)
endif()

file(GLOB implot_SOURCES ${implot_SOURCE_DIR}/*.cpp)
add_library(implot STATIC)
target_sources(implot PRIVATE ${implot_SOURCES})
target_include_directories(implot PUBLIC ${implot_SOURCE_DIR}/)
target_link_libraries(implot PRIVATE imgui)

FetchContent_Declare(stb
  GIT_REPOSITORY https://github.com/nothings/stb
)
FetchContent_GetProperties(stb)
if(NOT stb_POPULATED)
  FetchContent_Populate(stb)
endif()

file(GLOB stb_HEADERS ${stb_SOURCE_DIR}/*.h)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR}/)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)