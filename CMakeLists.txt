cmake_minimum_required(VERSION 3.5.1)

project(newbies_prog1)


# Force norme C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

# CMake compilation option
set(CMAKE_CXX_FLAGS ${GCC_C_NORME})


find_package(Qt5 COMPONENTS Core Network Gui Xml REQUIRED)
find_package(Tesseract 4.00 REQUIRED)
find_package(Leptonica REQUIRED)

include_directories(${Tesseract_INCLUDE_DIRS})
include_directories(${Leptonica_INCLUDE_DIRS})

file(GLOB_RECURSE CPP_LIST "*.cpp")
file(GLOB_RECURSE H_LIST "*.h*")
# set(PRINCIPAL_MAIN main.cpp)

add_executable(${PROJECT_NAME} ${CPP_LIST} ${H_LIST})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Gui Qt5::Xml ${Tesseract_LIBRARIES} ${Leptonica_LIBRARIES})
