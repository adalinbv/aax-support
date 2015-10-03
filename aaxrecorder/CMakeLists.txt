
ENABLE_LANGUAGE(CXX)

CHECK_LIBRARY_EXISTS(m floorf "" HAVE_LIBM)
IF(HAVE_LIBM)
  SET(EXTRA_LIBS m ${EXTRA_LIBS})
ENDIF(HAVE_LIBM)

FIND_PACKAGE(AAX COMPONENTS aax REQUIRED)

SET( PREPROCESS_HEADERS aaxrecorder.h record.h setup.h )
SET( HEADERS aaxrecorder_ui.h record_ui.h setup_ui.h )
SET( RCS ${CMAKE_SOURCE_DIR}/admin/aaxplayer.qrc )
SET( WIN_RESOURCE_FILE ${CMAKE_SOURCE_DIR}/admin/aaxrecorder.rc)

FILE( GLOB GUI *.ui )
FILE( GLOB SOURCES *.cpp *.c )

QT4_WRAP_UI( GUI_SRCS ${GUI} )
QT4_WRAP_CPP( MOC_HEADERS ${PREPROCESS_HEADERS} )
QT4_ADD_RESOURCES(RC_SRCS ${RCS})

ADD_EXECUTABLE(aaxrecorder ${SOURCES} ${MOC_HEADERS} ${RC_SRCS} ${GUI_SRCS}  ${WIN_RESOURCE_FILE} )

TARGET_LINK_LIBRARIES( aaxrecorder ${QT_LIBRARIES} ${AAX_LIBRARY})

INSTALL(TARGETS aaxrecorder
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Media
)

IF(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "aaxrecorder;AeonWave Audio Recorder")
  SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/admin\\\\aaxrecorder.xpm")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\aaxrecorder.exe")

ELSE(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "aaxrecorder;AeonWave Audio Recorder")
  SET(CPACK_STRIP_FILES "bin/aaxrecorder")
ENDIF(WIN32)

IF(APPLE)
  SET(PLATFORM_SPECIFIC "aaxrecorder.icns")
  SET_SOURCE_FILES_PROPERTIES(
      "aaxrecorder.icns"
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
  )
  SET(GUI_TYPE MACOSX_BUNDLE)
  SET(MACOSX_BUNDLE_ICON_FILE MacIcon.icns)
ENDIF()

IF(UNIX AND NOT APPLE)
  INSTALL(FILES ${CMAKE_SOURCE_DIR}/admin/aaxrecorder.desktop
          DESTINATION share/applications
          COMPONENT Media)
  INSTALL(FILES ${CMAKE_SOURCE_DIR}/admin/aaxrecorder.xpm
          DESTINATION share/pixmaps
          COMPONENT Media)
ENDIF(UNIX AND NOT APPLE)

