cmake_minimum_required (VERSION 3.8)

project ("test")

# string(APPEND CMAKE_CXX_FLAGS " /std:c++20")

add_executable(
	test
	
	"0-graph/0-dependency.cpp" 
	"0-graph/1-dependant.cpp" 
 	"0-graph/2-visitor.cpp"
	"1-clock.cpp" 
	"2-timer.cpp"
	"3-animator.cpp" 
	"4-evaluators/animated.cpp" 
	"4-evaluators/constant.cpp" 
 	"4-evaluators/delegate.cpp"
	"5-var.cpp"
	"test.cpp"
	"main.cpp"  
 )

target_link_libraries(test rtl)

