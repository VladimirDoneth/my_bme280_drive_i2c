cmake_minimum_required(VERSION 3.13)
project(my_bme280_drive_i2c)

set(CMAKE_CXX_STANDARD 20)

add_executable(my_bme main.cpp
        lib_i2c/i2c_driver.cpp lib_i2c/i2c_driver.h
        lib_bme280/BME280CalibrationData.h lib_bme280/BME280Data.h lib_bme280/BME280RawData.h lib_bme280/bme280.h lib_bme280/bme280.cpp)