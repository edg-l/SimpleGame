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
find_package(SDL2_ttf REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(CGLM cglm REQUIRED)

set(SHARED_SOURCE_FILES
	src/color.h
	src/io.c
	src/io.h
	src/list.c
	src/list.h
	src/logger.c
	src/logger.h
	src/protocol.h
	)

set(CLIENT_SOURCE_FILES
	src/client.c
	src/graphics/renderer.c
	src/graphics/renderer.h
	src/graphics/shader.c
	src/graphics/shader.h
	src/ui/button.c
	src/ui/button.h
	${SHARED_SOURCE_FILES}
	)

set(SERVER_SOURCE_FILES
	src/server.c
	${SHARED_SOURCE_FILES}
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

file(COPY resources DESTINATION .)

add_executable(SimpleGame ${CLIENT_SOURCE_FILES})

target_link_libraries(SimpleGame SDL2::Main SDL2::Net SDL2::Mixer 
	SDL2::Image SDL2::TTF OpenGL::GL GLEW::GLEW ${CGLM_LIBRARIES})

add_executable(SimpleGameServer ${SERVER_SOURCE_FILES})

target_link_libraries(SimpleGameServer SDL2::Main SDL2::Net)
