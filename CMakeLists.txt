cmake_minimum_required(VERSION 2.8)

# set project name
project(SFMLPathfinder)

# Set the sfml install path
if (DEFINED ENV{SFML_INSTALL})
  set (SFML_INSTALL_PATH "$ENV{SFML_INSTALL}")
else()
  set (SFML_INSTALL_PATH "C:\\SFML\\SFML-2.5.1")
endif()

set (SFML_DIR "${SFML_INSTALL_PATH}/lib/cmake/SFML")

# locate SFML libs
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

# find openGL
find_package(OpenGL REQUIRED)

# create the executable
add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/imgui/imgui.cpp
  src/imgui/imgui_widgets.cpp
  src/imgui/imgui_draw.cpp
  src/imgui/imgui-SFML.cpp
  src/GridPanel.cpp
  src/GridPanel.h
  src/ControlPanel.cpp
  src/ControlPanel.h
)

# include directories
target_include_directories(${PROJECT_NAME} PRIVATE "src/imgui")

# link options
target_link_libraries(${PROJECT_NAME} sfml-graphics ${OPENGL_LIBRARIES})

# copy any fonts needed to the build directory
file (GLOB FONTS "fonts/*.ttf")
file (COPY ${FONTS} DESTINATION ${PROJECT_BINARY_DIR})

# copy dlls (windows)
IF (WIN32)
  file (GLOB DLLS "${SFML_INSTALL_PATH}/bin/*.dll")
  file (COPY ${DLLS} DESTINATION ${PROJECT_BINARY_DIR})
ENDIF()