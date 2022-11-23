cmake_minimum_required(VERSION 3.5)

project(energo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS
    Widgets
    Xml
    Network
    Sql
    REQUIRED)


add_executable(${PROJECT_NAME}
    global_defs.h
    Configurator.h
    Configurator.cpp
    DbController.h
    DbController.cpp
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    NewPropertyForm.cpp
    NewPropertyForm.h
    NewPropertyForm.ui
    Models.h
    Models.cpp
    TableWidget.h
    TableWidget.cpp
    ToolButton.h
    ToolButton.cpp
    UnitForm.cpp
    UnitForm.h
    UnitForm.ui
    XmlController.h
    XmlController.cpp
    energo.qrc
    )


target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt5::Widgets
    Qt5::Xml
    Qt5::Sql
    Qt5::Network)


configure_file(energo.conf energo.conf COPYONLY)
