cmake_minimum_required (VERSION 3.8)

project ("rtl")

string(APPEND CMAKE_CXX_FLAGS " /std:c++20")

add_library (
	rtl 
	
	"animator.cpp"
	"clock.cpp"
	"dependant.cpp"
	"dependency.cpp"
	"dependency_visitor.cpp"
	"easing.cpp"
	"log.cpp"
)

add_executable(
	test
	
	"tests/0-dependency.cpp" 
	"tests/1-dependant.cpp" 
 	"tests/2-dependency_visitor.cpp"
	"tests/3-clock.cpp" 
	"tests/4-animated.cpp"
	"tests/5-constant.cpp" 
	"tests/6-delegate.cpp" 
	"tests/7-var.cpp"
	"tests/8-animator.cpp" 
	"tests/test.cpp"
	"tests/main.cpp"  
)

target_link_libraries(test rtl)

