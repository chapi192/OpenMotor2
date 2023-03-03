# have a `set(SFML_PATH "filepath/to/sfml")` and another line for TGUI_PATH in a .env.cmake file
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
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/src/*.h)  # to have the headers in the VS Header Files filter
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

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
	tgui-s
	sfml-graphics-s
	sfml-window-s
	sfml-system-s
	freetype
	opengl32
	winmm
	gdi32
)
