file(GLOB srcs *.cpp *.h)
file(GLOB coreSrcs ../core/*.cpp ../core/*.h)
file(GLOB uis ../../gui/*.ui)

include_directories(${PROJECT_SOURCE_DIR}/src/core)

kde4_add_ui_files(srcs ${uis})
kde4_add_executable(${PROJECT_NAME} ${coreSrcs} ${srcs})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${QT_QTSQL_LIBRARIES} ${KDE4_KDEUI_LIBS})
