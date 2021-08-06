cmake_minimum_required(VERSION 3.6)

set(POCO_INC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(POCO_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/Poco/${ANDROID_ABI})

add_library(PocoTestMain
        SHARED
        PocoTestMain.cpp)

target_include_directories(PocoTestMain
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PocoTest)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/PocoTest)

target_link_libraries(PocoTestMain PocoLibs)