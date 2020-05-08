cmake_minimum_required (VERSION 3.17)
project (minidlna-mp34)

set (project_SRC
  src/minidlna.c
  src/scanner.c
  src/upnpglobalvars.c
  src/upnphttp.c
  src/upnpsoap.c
)
set (minidlna_DIR
  ../minidlna-1.2.1
)
set (minidlna_SRC
  ${minidlna_DIR}/albumart.c
  ${minidlna_DIR}/avahi.c
  ${minidlna_DIR}/clients.c
  ${minidlna_DIR}/containers.c
  ${minidlna_DIR}/getifaddr.c
  ${minidlna_DIR}/image_utils.c
  ${minidlna_DIR}/log.c
  ${minidlna_DIR}/metadata.c
  ${minidlna_DIR}/minissdp.c
  ${minidlna_DIR}/minixml.c
  ${minidlna_DIR}/monitor.c
  ${minidlna_DIR}/options.c
  ${minidlna_DIR}/playlist.c
  ${minidlna_DIR}/process.c
  ${minidlna_DIR}/sql.c
  ${minidlna_DIR}/tivo_beacon.c
  ${minidlna_DIR}/tivo_commands.c
  ${minidlna_DIR}/tivo_utils.c
  ${minidlna_DIR}/upnpdescgen.c
  ${minidlna_DIR}/upnpevents.c
  ${minidlna_DIR}/upnpreplyparse.c
  ${minidlna_DIR}/utils.c
  ${minidlna_DIR}/uuid.c
  ${minidlna_DIR}/tagutils/tagutils.c
)
include_directories (
  ${minidlna_DIR}
)
link_libraries (
  jpeg
  avformat
  avutil
  vorbis
  ogg
  id3tag
  FLAC
  exif
  sqlite3
  pthread
)
add_executable (
  ${PROJECT_NAME}
  ${project_SRC}
  ${minidlna_SRC}
)
include (
  GNUInstallDirs
)
set (DATA_PATH
  ${CMAKE_INSTALL_FULL_DATADIR}/${PROJECT_NAME}
)
configure_file (
  src/config_mp34.h.in
  src/config_mp34.h
)
install (
  TARGETS ${PROJECT_NAME}
)
install (
  DIRECTORY etc/${PROJECT_NAME} etc/systemd
  TYPE SYSCONF
  USE_SOURCE_PERMISSIONS
)
install (
  FILES "Conversion Info.mp4"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)
install (
  SCRIPT post_install.cmake
)
