cmake_minimum_required(VERSION 3.13)

project(SimpleGame LANGUAGES C)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_REV 0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Freetype REQUIRED)

pkg_check_modules(CGLM cglm REQUIRED)

set(ENGINE_SHARED_SOURCES
	src/engine/color.h
	src/engine/io.c
	src/engine/io.h
	src/engine/list.c
	src/engine/list.h
	src/engine/logger.c
	src/engine/logger.h
	src/engine/settings.c
	src/engine/settings.h
	)

set(ENGINE_CLIENT_SOURCES
	src/engine/graphics/renderer.c
	src/engine/graphics/renderer.h
	src/engine/graphics/shader.c
	src/engine/graphics/shader.h
	src/engine/ui/button.c
	src/engine/ui/button.h
	)

set(SHARED_SOURCE_FILES
	src/protocol.h
	)

set(CLIENT_SOURCE_FILES
	src/client.c
	)

set(SERVER_SOURCE_FILES
	src/server.c
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

add_library(ClientEngine STATIC ${ENGINE_SHARED_SOURCES} ${ENGINE_CLIENT_SOURCES})
add_library(ServerEngine STATIC ${ENGINE_SHARED_SOURCES})

target_link_libraries(ClientEngine SDL2::Main SDL2::Net SDL2::Mixer 
	SDL2::Image OpenGL::GL GLEW::GLEW ${CGLM_LIBRARIES} Freetype::Freetype)

target_link_libraries(ServerEngine SDL2::Main SDL2::Net)

file(COPY resources DESTINATION .)

add_executable(SimpleGame ${CLIENT_SOURCE_FILES} ${SHARED_SOURCE_FILES})

target_link_libraries(SimpleGame ClientEngine ${CGLM_LIBRARIES})

add_executable(SimpleGameServer ${SERVER_SOURCE_FILES} ${SHARED_SOURCE_FILES})

target_link_libraries(SimpleGameServer ServerEngine)
