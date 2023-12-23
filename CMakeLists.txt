cmake_minimum_required(VERSION 3.27)

project(OpenCL-Tutorial LANGUAGES C)

add_compile_definitions(CL_TARGET_OPENCL_VERSION=300)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/inc/")

add_subdirectory(lib)
add_subdirectory(01)
add_subdirectory(02)

find_package(OpenCL REQUIRED)

target_link_libraries(deviceInfo PRIVATE OpenCL::OpenCL errorHandling)
target_link_libraries(vadd PRIVATE OpenCL::OpenCL errorHandling)

set_target_properties(errorHandling deviceInfo PROPERTIES C_STANDARD 23 C_STANDARD_REQUIRED ON)
