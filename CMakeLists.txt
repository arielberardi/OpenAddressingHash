cmake_minimum_required(VERSION 3.30.3)
project(OpenAddressingHash VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### BUILD PROJECT ####

include_directories(${PROJECT_SOURCE_DIR}/Source)
include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
)

set(LIBRARIES
)

# Main Build
add_executable(${PROJECT_NAME} ${SOURCES} Source/Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})
