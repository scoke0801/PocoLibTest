cmake_minimum_required(VERSION 3.6)

set(POCO ${CMAKE_CURRENT_SOURCE_DIR}/Poco)
set(POCO_INC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(POCO_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/Poco/${ANDROID_ABI})

add_library(native-lib SHARED native-lib.cpp LoggerTest.h LoggerTest.cpp )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(PocoLib STATIC IMPORTED)

set_target_properties(PocoLib
        PROPERTIES
        IMPORTED_LOCATION
        ${POCO_LIB}/libPocoFoundation.a)

target_link_libraries(native-lib PocoLib)