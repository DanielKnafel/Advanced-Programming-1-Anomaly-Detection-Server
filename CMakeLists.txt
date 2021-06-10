cmake_minimum_required(VERSION 3.0.0)
project(ex6 VERSION 0.1.0)

include(CTest)
enable_testing()

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
add_executable(ex6 MainTrain.cpp timeseries.cpp anomaly_detection_util.cpp SimpleAnomalyDetector.cpp minCircle.cpp HybridAnomalyDetector.cpp CLI.cpp Server.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
