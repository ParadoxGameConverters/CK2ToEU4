cmake_minimum_required(VERSION 3.5)
#set(CMAKE_VERBOSE_MAKEFILE on)

project(EU4ToVic2)
#SET(CMAKE_CXX_COMPILER g++-8)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Source)
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Source)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Release-Linux)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Release-Linux)

add_compile_options("-std=c++17")
add_compile_options("-lstdc++fs")
add_compile_options("-O2")
add_compile_options("-pthread")

include_directories("../common_items")
include_directories("../ZipLib")
include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(../ZipLib [binary_dir])

file(GLOB MAIN_SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB MAPPER_SOURCES "${PROJECT_SOURCE_DIR}/Mappers/*.cpp")
file(GLOB MAPPER_VERSIONPARSER_SOURCES "${PROJECT_SOURCE_DIR}/Mappers/VersionParser/*.cpp")
set(COMMON_SOURCES "../common_items/CardinalToOrdinal.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../common_items/Color.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../common_items/CommonUtils.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../common_items/Date.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../common_items/LinuxUtils.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../common_items/Log.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../common_items/Object.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../common_items/newParser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../common_items/ParserHelpers.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../common_items/StringUtils.cpp")

add_executable(EU4ToVic2 
	${MAIN_SOURCES}
	${MAPPER_SOURCES}
	${MAPPER_VERSIONPARSER_SOURCES}
	${COMMON_SOURCES}
)

target_link_libraries(EU4ToVic2 LINK_PUBLIC ZIPLIB stdc++fs)

add_custom_command(TARGET EU4ToVic2 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND chmod u+x Copy_Files.sh)
add_custom_command(TARGET EU4ToVic2 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ./Copy_Files.sh)
