
ENABLE_LANGUAGE(CXX)

CHECK_LIBRARY_EXISTS(m floorf "" HAVE_LIBM)
IF(HAVE_LIBM)
  SET(EXTRA_LIBS m ${EXTRA_LIBS})
ENDIF(HAVE_LIBM)

FIND_PACKAGE(AAX COMPONENTS aax REQUIRED)

FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui)
INCLUDE( ${QT_USE_FILE} )

IF(WIN32)
  SET (QT_USE_QTMAIN TRUE)
ENDIF(WIN32)

SET( SOURCES main.cpp aaxrecorder.cpp record.cpp setup.cpp )
SET( PREPROCESS_HEADERS aaxrecorder.h record.h setup.h )
SET( HEADERS aaxrecorder_ui.h record_ui.h setup_ui.h )
SET( GUI aaxrecorder.ui record.ui setup.ui )
SET( RCS ${CMAKE_SOURCE_DIR}/admin/aaxplayer.qrc )

QT4_ADD_RESOURCES(RC_SRCS ${RCS})
QT4_WRAP_UI( GUI_HDRS ${GUI} )
QT4_WRAP_CPP( MOC_HEADERS ${PREPROCESS_HEADERS} )

ADD_EXECUTABLE(aaxrecorder ${SOURCES} ${MOC_HEADERS} ${RC_SRCS} ${GUI_HEADERS} )

TARGET_LINK_LIBRARIES( aaxrecorder ${QT_LIBRARIES} ${AAX_LIBRARY})

INSTALL(TARGETS aaxrecorder
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Media
)

IF(WIN32)
  # Include Qt libraries
  CMP_QT_LIBRARIES_INSTALL_RULES("QtCore;QtGui" "bin")
  INSTALL_CUSTOM_LIB(${AAX_LIBRARY} "bin")

  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "aaxrecorder" "AeonWave Audio Recorder")
  SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/admin\\\\aaxrecorder.xpm")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\aaxrecorder.exe")
# SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\admin\aaxrecorder.xpm")

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

