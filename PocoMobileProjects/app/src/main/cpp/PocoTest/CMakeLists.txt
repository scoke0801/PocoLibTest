cmake_minimum_required(VERSION 3.6)

set(PocoTest ${CMAKE_CURRENT_SOURCE_DIR}/PocoTest)

add_library(PocoLibs
        STATIC
        Logger/LoggerTest.cpp
        Net/NetTest.cpp)

target_include_directories(PocoLibs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include/Poco/Net)
target_include_directories(PocoLibs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_library(PocoFoundation STATIC IMPORTED)
set_target_properties(PocoFoundation  PROPERTIES IMPORTED_LOCATION ${POCO_LIB}/libPocoFoundation.a)

add_library(PocoNet STATIC IMPORTED)
set_target_properties(PocoNet  PROPERTIES IMPORTED_LOCATION ${POCO_LIB}/libPocoNet.a)

add_library(PocoUtil STATIC IMPORTED)
set_target_properties(PocoUtil  PROPERTIES IMPORTED_LOCATION ${POCO_LIB}/libPocoUtil.a)

target_link_libraries(PocoLibs PocoFoundation PocoNet PocoUtil)