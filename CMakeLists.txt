cmake_minimum_required(VERSION 3.1)
include_directories( "head_tracking" )
project( HeadTracking )

#add_subdirectory("rpi_camera")
add_subdirectory("rpi_camera/host_applications/linux/apps/raspicam")

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )
file(GLOB HEADTRACKING_SOURCES head_tracking/*.cpp head_tracking/*/*.cpp)

add_executable( HeadTracking ${HEADTRACKING_SOURCES})
target_link_libraries( HeadTracking Threads::Threads ${OpenCV_LIBS} )

