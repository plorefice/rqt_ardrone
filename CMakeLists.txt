cmake_minimum_required(VERSION 2.8.3)
project(rqt_ardrone)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  ardrone_autonomy
  roscpp
  rqt_gui
  rqt_gui_cpp
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
find_package(Qwt REQUIRED)

include(${QT_USE_FILE})

set(rqt_ardrone_SRCS
  src/rqt_ardrone/navdata/navdata.cpp
  src/rqt_ardrone/navdata/navball.cpp
  src/rqt_ardrone/navdata/status_panel.cpp

  src/rqt_ardrone/commands/commands.cpp
)

set(rqt_ardrone_HDRS
  include/rqt_ardrone/navdata/navdata.h
  include/rqt_ardrone/navdata/navball.h
  include/rqt_ardrone/navdata/status_panel.h

  include/rqt_ardrone/commands/commands.h
)

set(rqt_ardrone_UIS
  src/rqt_ardrone/navdata/navdata.ui

  src/rqt_ardrone/commands/commands.ui
)

set(rqt_ardrone_QRC
  resources/resources.qrc
)

set(rqt_ardrone_INCLUDE_DIRECTORIES
  include
  ${QWT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

###################################
## catkin specific configuration ##
###################################

catkin_package(
#  INCLUDE_DIRS ${rqt_ardrone_INCLUDE_DIRECTORIES}
#  LIBRARIES ${PROJECT_NAME}
#  CATKIN_DEPENDS ardrone_autonomy roscpp rqt_gui rqt_gui_cpp
)

###########
## Build ##
###########

qt4_wrap_cpp(rqt_ardrone_MOCS 
  ${rqt_ardrone_HDRS}
)

qt4_wrap_ui(rqt_ardrone_UIS_H 
  ${rqt_ardrone_UIS}
)

qt4_add_resources(rqt_ardrone_QRC_RCC
  ${rqt_ardrone_QRC}
)

include_directories(
  ${rqt_ardrone_INCLUDE_DIRECTORIES}
  ${catkin_INCLUDE_DIRS}
)

## Declare libraries
add_library(${PROJECT_NAME}
  ${rqt_ardrone_SRCS}
  ${rqt_ardrone_MOCS}
  ${rqt_ardrone_UIS_H}
  ${rqt_ardrone_QRC_RCC}
)

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QWT_LIBRARY}
)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS rqt_ardrone_navdata rqt_ardrone_navdata_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_rqt_ardrone_navdata.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
