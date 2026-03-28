cmake_minimum_required(VERSION 3.16)
project(UltraHDRViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# Добавлен Network
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Network)

add_executable(UltraHDRViewer main.cpp viewer.h)
# Линкуем Network
target_link_libraries(UltraHDRViewer PRIVATE Qt6::Widgets Qt6::Gui Qt6::Network)

install(TARGETS UltraHDRViewer DESTINATION /usr/local/bin)
