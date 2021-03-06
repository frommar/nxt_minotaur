cmake_minimum_required(VERSION 2.8.3)
project(mouse_monitor_pc)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake-modules")

find_package(catkin REQUIRED COMPONENTS
	qt_build
	roscpp
	mouse_monitor_beagle
	minotaur_common
)

find_package(Qwt REQUIRED)

catkin_package()

###########
## Build ##
###########

#################
## Mouse sensor #
#################

get_filename_component(PLN_MINOTAUR_SVN_DIRECTORY ../../../pln_minotaur ABSOLUTE)

if (NOT IS_DIRECTORY ${PLN_MINOTAUR_SVN_DIRECTORY})
	message(FATAL_ERROR "Check out pln_minotaur svn repository first!" )
endif (NOT IS_DIRECTORY ${PLN_MINOTAUR_SVN_DIRECTORY})

set(PLN_MINOTAUR_LIB_NAME pln_minotaur_x86)

file(GLOB PLN_MINOTAUR_LIB ${PLN_MINOTAUR_SVN_DIRECTORY}/trunk/build)
file(GLOB PLN_MINOTAUR_INCLUDE_DIRECTORY ${PLN_MINOTAUR_SVN_DIRECTORY}/trunk/src)

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

file(GLOB MOUSE_MONITOR_PC_SOURCES src/${PROJECT_NAME}/*.cpp)

link_directories(${PLN_MINOTAUR_LIB})
include_directories(include ${catkin_INCLUDE_DIRS} ${PLN_MINOTAUR_INCLUDE_DIRECTORY} ${QWT_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${MOUSE_MONITOR_PC_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${catkin_LIBRARIES} ${PLN_MINOTAUR_LIB_NAME} ${QWT_LIBRARIES})
add_dependencies(${PROJECT_NAME} mouse_monitor_beagle_generate_messages_cpp)
