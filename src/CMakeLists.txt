cmake_minimum_required(VERSION 3.18)
cmake_policy(SET CMP0135 NEW)
project(search_engine)
set(CMAKE_CXX_STANDARD 20)
set(BUILD_CPR_TESTS OFF)
set(CMAKE_USE_OPENSSL OFF)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY{$CMAKE_BINARY_DIR})

set(Src 
                    engine.cpp 
                    invertedindex.cpp 
                    convertjson.cpp
                    searchserver.cpp 
    )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY{$CMAKE_SOURCE_DIR})
add_executable(search_engine  ${Src} ) 
target_link_libraries(search_engine  PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})








