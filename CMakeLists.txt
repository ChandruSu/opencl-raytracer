
# Setup project properties
cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_STANDARD 20)
set(EXEC app)
set(CL "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v11.7")

project(engine-test CXX)

# Fetches source code
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h ${CMAKE_SOURCE_DIR}/src/*/*.h)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*/*.cpp)

add_executable(${EXEC} ${HEADER_FILES} ${SOURCE_FILES})

# Fetches built-in libraries
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Fetches dependencies
include_directories(${CL}/include/)
include_directories(${CMAKE_SOURCE_DIR}/include/)
set(GLFW_LIBRARIES ${CMAKE_SOURCE_DIR}/libs/GLFW/glfw3.lib)
set(GLEW_LIBRARIES ${CMAKE_SOURCE_DIR}/libs/GLEW/glew32s.lib)
set(CL_LIBS ${CL}/lib/x64/OpenCL.lib)

# Links project with dependencies
link_directories(${CL}/bin;${CL}/lib/x64/;)
target_link_libraries(${EXEC} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${CL_LIBS})
