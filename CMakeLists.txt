cmake_minimum_required(VERSION 3.1)
include_directories( "head_tracking" )
project( HeadTracking )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
#find_package( OpenCV REQUIRED )

file(GLOB HEADTRACKING_SOURCES head_tracking/*.cpp head_tracking/*/*.cpp)

add_executable( HeadTracking ${HEADTRACKING_SOURCES})
target_link_libraries( HeadTracking Threads::Threads)#${OpenCV_LIBS} )

