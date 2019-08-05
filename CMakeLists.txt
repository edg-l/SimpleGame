cmake_minimum_required(VERSION 3.13)

project(SimpleGame LANGUAGES C)


# Needed so that cmake uses our find modules.

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


find_package(SDL2 REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(SHARED_SOURCE_FILES
	src/color.h
	src/protocol.h
	src/list.h
	src/list.c
	)

set(CLIENT_SOURCE_FILES
	src/client.c
	src/renderer.c
	src/renderer.h
	${SHARED_SOURCE_FILES}
	)

set(SERVER_SOURCE_FILES
	src/server.c
	${SHARED_SOURCE_FILES}
	)


include_directories(src)

file(COPY resources DESTINATION .)

add_executable(SimpleGame ${CLIENT_SOURCE_FILES})

target_link_libraries(SimpleGame SDL2::Main SDL2::Net SDL2::Mixer SDL2::Image SDL2::TTF SDL2::GFX)

add_executable(SimpleGameServer ${SERVER_SOURCE_FILES})

target_link_libraries(SimpleGameServer SDL2::Main SDL2::Net)
