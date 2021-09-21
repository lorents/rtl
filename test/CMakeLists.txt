cmake_minimum_required (VERSION 3.8)

project ("test")

string(APPEND CMAKE_CXX_FLAGS " /std:c++20")

add_executable(
	test
	
	"0-graph/0-dependency.cpp" 
	"0-graph/1-dependant.cpp" 
 	"0-graph/2-dependency_visitor.cpp"
	"1-clock.cpp" 
	"2-timer.cpp"
	"3-animator.cpp" 
	"4-var/0-animated.cpp"
	"4-var/1-constant.cpp" 
	"4-var/2-delegate.cpp"
	"4-var/3-var.cpp"
	"6-populator.cpp"
	"test.cpp"
	"main.cpp"  
 )

target_link_libraries(test rtl)

