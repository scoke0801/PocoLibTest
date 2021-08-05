cmake_minimum_required(VERSION 3.6)

set(POCO_INC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(POCO_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/Poco/${ANDROID_ABI})

add_library(native-lib
        SHARED
        native-lib.cpp)

target_include_directories(native-lib
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PocoTest
        )

# 소스 코드에서 include 탐색 경로 추가
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/PocoTest)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/PocoTest/Logger)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/PocoTest/Net)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/PocoTest)

target_link_libraries(native-lib PocoLibs)