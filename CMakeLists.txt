cmake_minimum_required(VERSION 3.20)

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -std=c11")

project(
	obaman
	LANGUAGES	C
	VERSION		0.0.1
	DESCRIPTION	"Obamium themed pacman"
)

file(GLOB_RECURSE _sources ./src/*.s ./src/*.c)

psn00bsdk_add_executable(
	obaman STATIC
	${_sources}
)

psn00bsdk_add_cd_image(
	iso				#Target name
	../iso/OBAMAN	#Image name
	./cd/iso.xml	#Config
	DEPENDS obaman
)
