
ENABLE_LANGUAGE(CXX)

CHECK_LIBRARY_EXISTS(m floorf "" HAVE_LIBM)

FIND_PACKAGE(AAX COMPONENTS aax REQUIRED)
FIND_PACKAGE(XML COMPONENTS xml REQUIRED)

# generate a list of all files that match the globbing expressions
FILE( GLOB GUI *.ui )
FILE( GLOB SOURCES *.cpp *.c )
file( GLOB RC *.rc *.qrc )

ADD_EXECUTABLE(aaxplayer ${SOURCES} ${RC} ${GUI} )
qt5_use_modules(aaxplayer Core Gui Widgets Network)

TARGET_LINK_LIBRARIES( aaxplayer
    ${AAX_LIBRARY}
    ${XML_LIBRARY}
    ${EXTRA_LIBS}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Network
)

INSTALL(TARGETS aaxplayer
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Media
)

IF(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "aaxplayer;AeonWave Audio Player")
  SET(CPACK_PACKAGE_ICON "admin\\\\aaxplayer.xpm")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\aaxplayer.exe")

ELSE(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "aaxplayer;AeonWave Audio Player")
  SET(CPACK_STRIP_FILES "bin/aaxplayer")
ENDIF(WIN32)

IF(APPLE)
  SET(PLATFORM_SPECIFIC "aaxplayer.icns")
  SET_SOURCE_FILES_PROPERTIES(
      "aaxplayer.icns"
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
  )
  SET(GUI_TYPE MACOSX_BUNDLE)
  SET(MACOSX_BUNDLE_ICON_FILE MacIcon.icns)
ENDIF()

IF(UNIX AND NOT APPLE)
  INSTALL(FILES admin/aaxplayer.desktop
          DESTINATION share/applications
          COMPONENT Media)
  INSTALL(FILES admin/aaxplayer.xpm
          DESTINATION share/pixmaps
          COMPONENT Media)
ENDIF(UNIX AND NOT APPLE)

