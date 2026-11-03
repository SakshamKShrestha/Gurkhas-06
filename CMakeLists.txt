cmake_minimum_required(VERSION 3.15)

project(GreenGuardian LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build Paho MQTT C and C++ as static libraries
set(PAHO_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(PAHO_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(PAHO_WITH_SSL OFF CACHE BOOL "" FORCE)
set(PAHO_WITH_MQTT_C ON CACHE BOOL "" FORCE)

# Disable unnecessary Paho components
set(PAHO_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(PAHO_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(PAHO_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# Include the Paho MQTT dependency
add_subdirectory(libraries/paho.mqtt.cpp)

# Green Guardian MQTT publisher
add_executable(mqtt_publisher
    simulator/mqtt_publisher.cpp
)

target_link_libraries(mqtt_publisher PRIVATE
    PahoMqttCpp::paho-mqttpp3-static
)

# Green Guardian Edge Data Processor (Task 4)
add_executable(edge_processor
    simulator/edge_processor.cpp
)

target_link_libraries(edge_processor PRIVATE
    PahoMqttCpp::paho-mqttpp3-static
)
