cmake_minimum_required (VERSION 3.8)

project ("rtl")

# string(APPEND CMAKE_CXX_FLAGS " /std:c++20")

add_library (
	rtl 
	
	"src/graph/dependant.cpp"
	"src/graph/dependency.cpp"
	"src/graph/visitor.cpp"
	"src/log.cpp"

	"src/animator.cpp"
	"src/clock.cpp"
	"src/easing.cpp"
	"src/timer.cpp"
)

target_include_directories(rtl PUBLIC "include")

add_subdirectory("test")