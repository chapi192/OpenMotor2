# have a `set(SFML_PATH "filepath/to/sfml")` in a .env.cmake file
include(./.env.cmake REQUIRED RESULT_VARIABLE LOCAL_ENV)
cmake_minimum_required(VERSION 3.16)

project("PhysSim")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE "")

add_compile_definitions(SFML_STATIC)

# executable creation
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
	src
	${SFML_PATH}/include
	${TGUI_PATH}/include
)
target_link_directories(${PROJECT_NAME} PUBLIC
	${SFML_PATH}/lib
	${TGUI_PATH}/lib
)
target_link_libraries(${PROJECT_NAME}
	sfml-graphics-s
	sfml-window-s
	sfml-system-s
	freetype
	opengl32
	winmm
	gdi32
	tgui-s
)
