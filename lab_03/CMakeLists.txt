cmake_minimum_required(VERSION 3.17)
project(lab_03)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets Charts)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Charts)

set(UI_DOMAIN ./ui)
set(LOGIC_DOMAIN ./logic)

include_directories(${UI_DOMAIN})
include_directories(${UI_DOMAIN}/widgets)
include_directories(${LOGIC_DOMAIN})
include_directories(${LOGIC_DOMAIN}/mediator)
include_directories(${LOGIC_DOMAIN}/request)
include_directories(${LOGIC_DOMAIN}/line_algs)

add_executable(
        ${PROJECT_NAME}
        ${UI_DOMAIN}/main.cpp
        ${UI_DOMAIN}/MainWindow.h
        ${UI_DOMAIN}/MainWindow.cpp
        ${LOGIC_DOMAIN}/mediator/drawer_mediator.h
        ${LOGIC_DOMAIN}/mediator/drawer_mediator.cpp
        ${LOGIC_DOMAIN}/request/request.h
        ${LOGIC_DOMAIN}/request/request.cpp
        ${LOGIC_DOMAIN}/datatypes.h
        ${LOGIC_DOMAIN}/line_algs/bresenham.h
        ${LOGIC_DOMAIN}/line_algs/bresenham.cpp
        ${LOGIC_DOMAIN}/line_algs/dda.h
        ${LOGIC_DOMAIN}/line_algs/dda.cpp
        ${LOGIC_DOMAIN}/line_algs/wu.h
        ${LOGIC_DOMAIN}/line_algs/wu.cpp
        ${LOGIC_DOMAIN}/line_algs/utils.h
        ${LOGIC_DOMAIN}/line_algs/utils.cpp)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
