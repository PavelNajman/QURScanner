cmake_minimum_required(VERSION 3.1.0)

project(qurscanner VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_library(BC_LIFEHASH_LIB liblifehash lifehash REQUIRED)
find_library(BC_UR_LIB libbc-ur bc-ur REQUIRED)

find_path(BC_LIFEHASH_INCLUDE_DIR lifehash.hpp REQUIRED)
find_path(BC_UR_INCLUDE_DIR bc-ur.hpp REQUIRED)

find_package(OpenCV 4.5.3 REQUIRED)
find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

include_directories(${BC_LIFEHASH_INCLUDE_DIR} ${BC_UR_INCLUDE_DIR})

add_executable(qurscanner
    main.cpp
    multiparturstatus.cpp
    resultspage.cpp
    scanpage.cpp
    qurscanner.cpp
    urutils.cpp
    videosurface.cpp
)

target_link_libraries(qurscanner Qt5::Widgets Qt5::Multimedia ${OpenCV_LIBS} ${BC_LIFEHASH_LIB} ${BC_UR_LIB})
