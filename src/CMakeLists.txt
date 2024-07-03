cmake_minimum_required(VERSION 3.18)
cmake_policy(SET CMP0135 NEW)
project(search_engine)
enable_testing()
set(CMAKE_CXX_STANDARD 20)
set(BUILD_CPR_TESTS OFF)
set(CMAKE_USE_OPENSSL OFF)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY{$CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY{$CMAKE_BINARY_DIR})
set(${CMAKE_SOURCE_DIR})
set(gtest_disable_pthreads ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include(FetchContent)
FetchContent_Declare(json URL  https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)

FetchContent_MakeAvailable(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(search_engine 
                    engine.cpp 
                    ${MY_INCLUDE_DIR}/convertjson.h
                    ${MY_INCLUDE_DIR}/invertedindex.h
                    ${MY_INCLUDE_DIR}/searchserver.h                    
                    invertedindex.cpp 
                    convertjson.cpp
                    searchserver.cpp                      
                     gtest.cpp                                                  
                    ) 
target_link_libraries(search_engine  PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})
target_include_directories(search_engine PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(search_engine PRIVATE gtest_main)
target_link_libraries(gtest PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(search_engine)






