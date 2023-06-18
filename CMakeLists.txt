cmake_minimum_required(VERSION 3.8)

project(BetaC)

find_package(SDL2 REQUIRED)

if(APPLE OR WIN32)
	find_package(SDL2_image REQUIRED)
endif()

if(APPLE)
	find_package(OpenGL REQUIRED)
	set(FLAGS ${FLAGS} "-Wno-deprecated")
endif()

file(COPY ${CMAKE_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src/rd-132211)
file(COPY ${CMAKE_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src/rd-132328)
file(COPY ${CMAKE_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src/rd-160052)

if(WIN32)
	link_libraries(SDL2::SDL2 SDL2_image::SDL2_image opengl32 glu32 z)
elseif(APPLE)
	add_compile_options(${FLAGS})
	link_libraries(SDL2::SDL2 SDL2_image::SDL2_image z "-framework OpenGL")
elseif(UNIX)
	link_libraries(SDL2::SDL2 SDL2_image m GL GLU z)
endif()

add_subdirectory(src)