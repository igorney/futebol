project(viewer1)
add_executable(${PROJECT_NAME} main.cpp model.cpp openglwindow.cpp
                               camera.cpp)
enable_abcg(${PROJECT_NAME})
