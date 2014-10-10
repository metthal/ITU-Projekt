#file(GLOB srcs *.cpp include/*.h)
#
#include(${QT_USE_FILE})
#add_definitions(${QT_DEFINITIONS})
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#
#set(${QT_DONT_USE_QTGUI}, TRUE)
#
#add_library(${PROJECT_NAME}Core SHARED ${srcs})
#target_link_libraries(${PROJECT_NAME}Core ${QT_LIBRARIES})
