cmake_minimum_required(VERSION 3.27)

project(deviceInfo LANGUAGES C)

add_executable(${PROJECT_NAME} deviceInfo.c device.c)
