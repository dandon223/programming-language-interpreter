cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#add_compile_options(-fconcepts -O2)
file(GLOB TARGET_SRC "./src/*.cpp" "./modules/*/*.cpp" )
project(tkom_21z_daniel)
include_directories(
        ${CMAKE_HOME_DIRECTORY}/modules/lexer
        ${CMAKE_HOME_DIRECTORY}/modules/parser
        ${CMAKE_HOME_DIRECTORY}/modules/token
        ${CMAKE_HOME_DIRECTORY}/modules/interpreter
)

add_executable(tkom_21z_daniel ${TARGET_SRC} )

target_compile_options(tkom_21z_daniel PUBLIC -Werror -Wall )

add_subdirectory(tests)
