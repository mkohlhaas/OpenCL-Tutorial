cmake_minimum_required(VERSION 3.27)

project(OpenCL-Tutorial LANGUAGES C)

add_compile_definitions(CL_TARGET_OPENCL_VERSION=300)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/inc/")

add_subdirectory(lib)
add_subdirectory(01)

find_package(OpenCL REQUIRED)

target_link_libraries(DeviceInfo PRIVATE OpenCL::OpenCL ErrorHandling)

set_target_properties(ErrorHandling DeviceInfo PROPERTIES C_STANDARD 23 C_STANDARD_REQUIRED ON)
