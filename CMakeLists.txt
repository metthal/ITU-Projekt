project(WifiMgr C CXX)

find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)
include(KDE4Defaults)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall -Wextra")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories(${KDE4_INCLUDES})

add_subdirectory(src)
