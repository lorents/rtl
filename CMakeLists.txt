cmake_minimum_required (VERSION 3.8)

project ("rtl")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DRTL_USE_SOURCE_LOCATION)

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