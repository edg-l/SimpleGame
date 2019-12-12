cmake_minimum_required(VERSION 3.13)

project(SimpleGame LANGUAGES C)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_REV 0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wundef -Wpointer-arith -Wwrite-strings -ftrapv")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -pg")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -pg")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -pg")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Freetype REQUIRED)

pkg_check_modules(CGLM cglm REQUIRED)

set(ENGINE_SOURCES
	src/engine/color.h
	src/engine/camera.c
	src/engine/camera.h
	src/engine/color.h
	src/engine/engine.c
	src/engine/engine.h
	src/engine/entity.c
	src/engine/entity.h
	src/engine/graphics/renderer.c
	src/engine/graphics/renderer.h
	src/engine/graphics/shader.c
	src/engine/graphics/shader.h
	src/engine/input.c
	src/engine/input.h
	src/engine/io.c
	src/engine/io.h
	src/engine/list.c
	src/engine/list.h
	src/engine/logger.c
	src/engine/logger.h
	src/engine/math/constants.h
	src/engine/math/vector.h
	src/engine/settings.c
	src/engine/settings.h
	src/engine/textbuffer.c
	src/engine/textbuffer.h
	src/engine/tilemap.c
	src/engine/tilemap.h
	src/engine/ui/button.c
	src/engine/ui/button.h
	src/engine/ui/progress_bar.c
	src/engine/ui/progress_bar.h
	src/engine/ui/switch.c
	src/engine/ui/switch.h
	src/engine/ui/textbox.c
	src/engine/ui/textbox.h
	src/engine/util.c
	src/engine/util.h
	src/engine/util_colors.h
	)

set(CLIENT_SOURCE_FILES
	src/client.c
	)


execute_process(
	COMMAND git rev-parse HEAD
	RESULT_VARIABLE GIT_REV_HASH_RESULT
	OUTPUT_VARIABLE GIT_REV_HASH
	ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
	)

configure_file(config.h.in src/config.h)

include_directories(src ${CGLM_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

add_library(GameEngine STATIC ${ENGINE_SOURCES})

target_link_libraries(GameEngine SDL2::Main SDL2::Mixer OpenGL::GL GLEW::GLEW ${CGLM_LIBRARIES} Freetype::Freetype)

add_executable(SimpleGame ${CLIENT_SOURCE_FILES} ${SHARED_SOURCE_FILES})

target_link_libraries(SimpleGame GameEngine)

file(COPY resources DESTINATION .)
