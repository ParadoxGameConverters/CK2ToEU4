cmake_minimum_required(VERSION 3.5)

project(CK2ToEU4)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Source)
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Source)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Release-Linux)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Release-Linux)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_compile_options("-lstdc++fs")
add_compile_options("-O2")
add_compile_options("-pthread")
add_link_options(-no-pie)

include_directories("../commonItems")
include_directories("../commonItems/ZipLib")
include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(../commonItems/ZipLib [binary_dir])

file(GLOB MAIN_SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB CONFIGURATION_SOURCES "${PROJECT_SOURCE_DIR}/Configuration/*.cpp")
file(GLOB CK2WORLD_SOURCES "${PROJECT_SOURCE_DIR}/CK2World/*.cpp")
file(GLOB CK2WORLD_SUBDIR_SOURCES "${PROJECT_SOURCE_DIR}/CK2World/*/*.cpp")
file(GLOB MAPPER_SOURCES "${PROJECT_SOURCE_DIR}/Mappers/*/*.cpp")
file(GLOB EU4WORLD_SOURCES "${PROJECT_SOURCE_DIR}/EU4World/*.cpp")
file(GLOB EU4WORLD_SUBDIR_SOURCES "${PROJECT_SOURCE_DIR}/EU4World/*/*.cpp")
set(COMMON_SOURCES "../commonItems/CommonFunctions.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/Color.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/ConvenientParser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/ConverterVersion.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/Date.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/GameVersion.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/LinuxUtils.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/Log.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/ModLoader/ModLoader.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/ModLoader/ModParser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/Parser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/ParserHelpers.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/OSCommonLayer.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/StringUtils.cpp")

add_executable(CK2ToEU4 
	${MAIN_SOURCES}
	${CONFIGURATION_SOURCES}
	${CK2WORLD_SOURCES}
	${CK2WORLD_SUBDIR_SOURCES}
	${MAPPER_SOURCES}
	${EU4WORLD_SOURCES}
	${EU4WORLD_SUBDIR_SOURCES}
	${COMMON_SOURCES}
)

target_link_libraries(CK2ToEU4 LINK_PUBLIC ZIPLIB stdc++fs)

add_custom_command(TARGET CK2ToEU4 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND git rev-parse HEAD > Release-Linux/commit_id.txt)
add_custom_command(TARGET CK2ToEU4 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND chmod u+x Copy_Files.sh)
add_custom_command(TARGET CK2ToEU4 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ./Copy_Files.sh)
