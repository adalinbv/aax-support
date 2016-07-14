
ENABLE_LANGUAGE(CXX)

CHECK_LIBRARY_EXISTS(m floorf "" HAVE_LIBM)
IF(HAVE_LIBM)
  SET(EXTRA_LIBS m ${EXTRA_LIBS})
ENDIF(HAVE_LIBM)

FIND_PACKAGE(AAX COMPONENTS aax REQUIRED)

FILE( GLOB GUI *.ui )
FILE( GLOB SOURCES *.cpp *.c )
file( GLOB RC *.rc *.qrc )

ADD_EXECUTABLE(aaxrecorder ${SOURCES} ${RC} ${GUI} )

TARGET_LINK_LIBRARIES( aaxrecorder
    ${AAX_LIBRARY}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)

INSTALL(TARGETS aaxrecorder
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Media
)

IF(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "aaxrecorder;AeonWave Audio Recorder")
  SET(CPACK_PACKAGE_ICON "admin\\\\aaxrecorder.xpm")
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
  INSTALL(FILES admin/aaxrecorder.desktop
          DESTINATION share/applications
          COMPONENT Media)
  INSTALL(FILES admin/aaxrecorder.xpm
          DESTINATION share/pixmaps
          COMPONENT Media)
ENDIF(UNIX AND NOT APPLE)

