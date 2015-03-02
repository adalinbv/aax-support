
ENABLE_LANGUAGE(CXX)

CHECK_LIBRARY_EXISTS(m floorf "" HAVE_LIBM)

FIND_PACKAGE(AAX COMPONENTS aax REQUIRED)
FIND_PACKAGE(XML COMPONENTS xml REQUIRED)

SET( SOURCES main.cpp aaxconfig.cpp api.c )
SET( PREPROCESS_HEADERS aaxconfig.h widgets.h )
SET( HEADERS aaxconfig_ui.h api.h types.h )
SET( GUI aaxconfig.ui )
SET( RCS ${CMAKE_SOURCE_DIR}/admin/aaxconfig.qrc )
SET( WIN_RESOURCE_FILE ${CMAKE_SOURCE_DIR}/admin/aaxconfig.rc)

QT4_ADD_RESOURCES(RC_SRCS ${RCS})
QT4_WRAP_UI( GUI_HDRS ${GUI} )
QT4_WRAP_CPP( MOC_HEADERS ${PREPROCESS_HEADERS} )

ADD_EXECUTABLE(aaxconfig ${SOURCES} ${MOC_HEADERS} ${RC_SRCS} ${GUI_HEADERS}  ${WIN_RESOURCE_FILE} )

TARGET_LINK_LIBRARIES(aaxconfig ${QT_LIBRARIES} ${AAX_LIBRARY}  ${XML_LIBRARY} ${EXTRA_LIBS})

INSTALL(TARGETS aaxconfig
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Support
)

IF(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "aaxconfig;AeonWave Config")
  SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/admin\\\\aaxconfig.xpm")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\aaxconfig.exe")

ELSE(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "aaxconfig;AeonWave Config")
  SET(CPACK_STRIP_FILES "bin/aaxconfig")
ENDIF(WIN32)

IF(APPLE)
  SET(PLATFORM_SPECIFIC "aaxconfig.icns")
  SET_SOURCE_FILES_PROPERTIES(
      "aaxconfig.icns"
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
  )
  SET(GUI_TYPE MACOSX_BUNDLE)
  SET(MACOSX_BUNDLE_ICON_FILE MacIcon.icns)
ENDIF()

IF(UNIX AND NOT APPLE)
  INSTALL(FILES ${CMAKE_SOURCE_DIR}/admin/aaxconfig.desktop
          DESTINATION share/applications
          COMPONENT Support)
  INSTALL(FILES ${CMAKE_SOURCE_DIR}/admin/aaxconfig.xpm
          DESTINATION share/pixmaps
          COMPONENT Support)
ENDIF(UNIX AND NOT APPLE)

