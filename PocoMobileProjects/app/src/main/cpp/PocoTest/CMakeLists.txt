cmake_minimum_required(VERSION 3.6)

set(PocoTest ${CMAKE_CURRENT_SOURCE_DIR}/PocoTest)

add_library(PocoLibs
        STATIC
        Logger/LoggerTest.cpp
        Net/NetTest.cpp)

target_include_directories(PocoLibs
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# 소스 코드에서 include 탐색 경로 추가
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Logger)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Net)

add_library(PocoFoundation STATIC IMPORTED)
set_target_properties(PocoFoundation  PROPERTIES IMPORTED_LOCATION ${POCO_LIB}/libPocoFoundation.a)

add_library(PocoNet STATIC IMPORTED)
set_target_properties(PocoNet  PROPERTIES IMPORTED_LOCATION ${POCO_LIB}/libPocoNet.a)

add_library(PocoUtil STATIC IMPORTED)
set_target_properties(PocoUtil  PROPERTIES IMPORTED_LOCATION ${POCO_LIB}/libPocoUtil.a)

add_library(PocoCrypto STATIC IMPORTED)
set_target_properties(PocoCrypto  PROPERTIES IMPORTED_LOCATION ${POCO_LIB}/libPocoCrypto.a)

target_link_libraries(PocoLibs PocoFoundation PocoNet PocoUtil PocoCrypto)