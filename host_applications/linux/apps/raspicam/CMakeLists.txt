
# raspistill/raspivid/raspiyuv

SET(COMPILE_DEFINITIONS -Werror)

include_directories(${PROJECT_SOURCE_DIR}/host_applications/linux/libs/bcm_host/include)
include_directories(${PROJECT_SOURCE_DIR}/host_applications/linux/apps/raspicam/)
include_directories(${PROJECT_SOURCE_DIR}/host_applications/linux/libs/sm)

set (COMMON_SOURCES
   RaspiCamControl.c
   RaspiCLI.c
   RaspiPreview.c)

add_executable(raspividyuv  ${COMMON_SOURCES} RaspiVidYUV.c)

set (MMAL_LIBS mmal_core mmal_util mmal_vc_client)

target_link_libraries(raspividyuv   ${MMAL_LIBS} vcos bcm_host)

install(TARGETS raspividyuv RUNTIME DESTINATION bin)
