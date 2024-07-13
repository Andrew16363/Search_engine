cmake_minimum_required (VERSION 3.18)
cmake_policy(SET CMP0135 NEW)
project(GtestTests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(SrcFiles
	${MY_SRC_DIR}/invertedindex.cpp
	${MY_SRC_DIR}/searchserver.cpp
	${MY_SRC_DIR}/convertjson.cpp	
)

set(TestsFiles
	convertjson_test.cpp
	invertedindex_test.cpp
	searchserver_test.cpp 	
) 

add_executable(GtestTests ${TestsFiles} ${SrcFiles})
#target_link_libraries(GtestTests nlohmann_json::nlohmann_json Gtest::gtest_main)
target_link_libraries(GtestTests  PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(GtestTests GTest::gtest GTest::gtest_main)
target_link_libraries(GtestTests   PUBLIC gtest gtest_main)
#target_link_libraries(your_test_target ${GTEST_BOTH_LIBRARIES} pthread)
include(GoogleTest)
target_include_directories(GtestTests PUBLIC ${MY_INCLUDE_DIR})
gtest_discover_tests(GtestTests)

