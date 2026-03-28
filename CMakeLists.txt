cmake_minimum_required(VERSION 3.16)
project(UltraViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)

add_executable(UltraViewer main.cpp)
target_link_libraries(UltraViewer PRIVATE Qt6::Widgets Qt6::Gui)

install(TARGETS UltraViewer DESTINATION /usr/local/bin)
