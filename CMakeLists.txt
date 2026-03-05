cmake_minimum_required(VERSION 3.10)
project(CTSBuild)
set(DEBUG, true)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG -Wall")
else()
    set(CMAKE_BUILD_TYPE Release)
endif()


# Ścieżki do nagłówków
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    /usr/include/freetype2
)

#set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Zbierz wszystkie pliki źródłowe
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Dodaj plik glad.c
set(GLAD_SRC ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c)

# Dodaj bibliotekę GLAD
add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/external/glad/include)

# Dodaj plik stb_image.cpp do projektu
set(STB_IMAGE_SRC ${CMAKE_SOURCE_DIR}/external/stb/stb_image.cpp)

# Tworzymy executable - dołączamy też stb_image.cpp
add_executable(CTSBuild ${SOURCES} ${STB_IMAGE_SRC})

# Znajdź biblioteki
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
find_package(assimp REQUIRED)

# Dołącz include'y do targetu CBuild
target_include_directories(CTSBuild PRIVATE
    ${GLFW_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
)

# Linkowanie bibliotek
target_link_libraries(CTSBuild PRIVATE
    glad
    ${OPENGL_gl_LIBRARY}
    ${GLFW_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    GLU
    m
    pthread
    dl
)
