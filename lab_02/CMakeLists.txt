cmake_minimum_required(VERSION 3.17)
project(lab_02)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets PrintSupport)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

include_directories(ui/)
include_directories(ui/widgets)
include_directories(data)

add_executable(${PROJECT_NAME} main.cpp ui/widgets/qcustomplot.h
        ui/widgets/qcustomplot.cpp ui/UiMainWindow.h ui/MainWindow.cpp ui/MainWindow.h data/Satellite.cpp data/Satellite.h data/RotatableEllipse.cpp data/RotatableEllipse.h data/SatelliteBody.cpp data/SatelliteBody.h data/SatelliteWings.cpp data/SatelliteWings.h data/SatelliteHead.cpp data/SatelliteHead.h data/Instruments.cpp data/Instruments.h)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
