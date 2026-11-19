cmake_minimum_required(VERSION 3.21)
project(robot_kinematics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Quick3D Test)

add_library(kinematics_core STATIC
    src/core/dhparams.h
    src/core/robot_models.h
    src/core/robot_models.cpp
    src/core/kinematics_solver.h
    src/core/kinematics_solver.cpp
)

target_include_directories(kinematics_core PUBLIC src)
target_link_libraries(kinematics_core PUBLIC Qt6::Core Qt6::Gui)

source_group("Core" FILES
    src/core/dhparams.h
    src/core/robot_models.h
    src/core/robot_models.cpp
    src/core/kinematics_solver.h
    src/core/kinematics_solver.cpp
)

qt_add_executable(robot_kinematics
    src/main.cpp
    src/viewmodel/kinematics.h
    src/viewmodel/kinematics.cpp
)

target_link_libraries(robot_kinematics PRIVATE
    kinematics_core
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Quick3D
)

qt_add_qml_module(robot_kinematics
    URI KinematicsApp
    VERSION 1.0
    QML_FILES qml/Main.qml
)

source_group("ViewModel" FILES
    src/viewmodel/kinematics.h
    src/viewmodel/kinematics.cpp
)
source_group("QML" FILES qml/Main.qml)
source_group("Root" FILES src/main.cpp)

enable_testing()
add_executable(tst_kinematics
    tests/tst_kinematics.cpp
)

target_link_libraries(tst_kinematics PRIVATE kinematics_core Qt6::Test)
add_test(NAME tst_kinematics COMMAND tst_kinematics)

source_group("Tests" FILES tests/tst_kinematics.cpp)
