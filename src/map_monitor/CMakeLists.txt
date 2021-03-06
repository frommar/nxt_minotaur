cmake_minimum_required(VERSION 2.8.3)
project(map_monitor)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake-modules")

find_package(catkin REQUIRED COMPONENTS
	qt_build
	roscpp
	minotaur_common
    minotaur_common_qt
	minotaur_map
)

find_package(Qwt REQUIRED)

catkin_package()

###########
## Build ##
###########

#######
## QT #
#######

# Add the appropriate components to the component list here
rosbuild_prepare_qt4(
    QtCore
    QtGui
) 

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc)
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/${PROJECT_NAME}/*.hpp)

QT4_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

############
## Sources #
############

file(GLOB MAP_MONITOR_PC_SOURCES src/${PROJECT_NAME}/*.cpp)

include_directories(include ${catkin_INCLUDE_DIRS} ${QWT_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${MAP_MONITOR_PC_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${catkin_LIBRARIES} ${QWT_LIBRARIES})
add_dependencies(${PROJECT_NAME} minotaur_common_generate_messages_cpp)
