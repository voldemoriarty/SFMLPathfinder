cmake_minimum_required(VERSION 2.8)

# set project name
project(SFMLPathfinder)

# Set the sfml install path
set (SFML_INSTALL_PATH "C:\\SFML-2.5.1")
set (SFML_DIR "${SFML_INSTALL_PATH}/lib/cmake/SFML")

# locate SFML libs
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

# create the executable
add_executable(${PROJECT_NAME} "src/main.cpp")

# link options
target_link_libraries(${PROJECT_NAME} sfml-graphics)

# copy dlls (windows)
IF (WIN32)
  file (GLOB DLLS "${SFML_INSTALL_PATH}/bin/*.dll")
  file (COPY ${DLLS} DESTINATION ${PROJECT_BINARY_DIR})
ENDIF()