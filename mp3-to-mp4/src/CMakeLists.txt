cmake_minimum_required (VERSION 3.17)
project (mp3-to-mp4)

set (project_SRC
  ffmpegAdapter.cpp
  main.cpp
  mpxTool.cpp
  playerPage.cpp
  rendering.cpp
  ffmpeg/cmdutils.c
  ffmpeg/ffmpeg.c
  ffmpeg/ffmpeg_hook.c
  ffmpeg/ffmpeg_lib.c
  ffmpeg/ffmpeg_opt.c
)
set (ffmpeg_DIR
  ../../../ffmpeg-4.2.2
)
set (ffmpeg_SRC
  ${ffmpeg_DIR}/fftools/ffmpeg_filter.c
  ${ffmpeg_DIR}/fftools/ffmpeg_hw.c
)
include_directories (
  .
  ${ffmpeg_DIR}
  ${ffmpeg_DIR}/fftools
  /usr/include/qt
)
link_libraries (
  avcodec
  avdevice
  avfilter
  avformat
  avutil
  swresample
  swscale
  postproc
  Qt5WebKitWidgets
  Qt5WebKit
  Qt5Widgets
  Qt5Core
  Qt5Gui
  pthread
  z
)
set (CMAKE_CXX_STANDARD
  17
)
set (CMAKE_CXX_STANDARD_REQUIRED
  ON
)
set (CMAKE_POSITION_INDEPENDENT_CODE
  ON
)
add_compile_options (
  -fPIC
)
add_executable (
  ${PROJECT_NAME}
  ${project_SRC}
  ${ffmpeg_SRC}
)
include (
  GNUInstallDirs
)
set (DATA_DIR
  ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)
set (DATA_PATH
  ${CMAKE_INSTALL_PREFIX}/${DATA_DIR}
)
configure_file (
  config_mp34.h.in
  config_mp34.h
)
install (
  TARGETS ${PROJECT_NAME}
)
install (
  DIRECTORY ../playerTemplate
  DESTINATION ${DATA_DIR}
)
