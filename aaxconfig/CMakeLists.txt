
ENABLE_LANGUAGE(CXX)

CHECK_LIBRARY_EXISTS(m floorf "" HAVE_LIBM)
IF(HAVE_LIBM)
  SET(EXTRA_LIBS m ${EXTRA_LIBS})
ENDIF(HAVE_LIBM)

FIND_PACKAGE(AAX COMPONENTS aax REQUIRED)
FIND_PACKAGE(XML COMPONENTS xml REQUIRED)

FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui)
INCLUDE( ${QT_USE_FILE} )

IF(WIN32)
  SET (QT_USE_QTMAIN TRUE)
ENDIF(WIN32)

SET( SOURCES main.cpp aaxconfig.cpp api.c )
SET( PREPROCESS_HEADERS aaxconfig.h widgets.h )
SET( HEADERS aaxconfig_ui.h api.h types.h )
SET( GUI aaxconfig.ui )
SET( RCS ${aaxconfig_SOURCE_DIR}/admin/aaxconfig.qrc )

QT4_ADD_RESOURCES(RC_SRCS ${RCS})
QT4_WRAP_UI( GUI_HDRS ${GUI} )
QT4_WRAP_CPP( MOC_HEADERS ${PREPROCESS_HEADERS} )

INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} ${AAX_INCLUDE_DIR} ${XML_INCLUDE_DIR} )

ADD_EXECUTABLE(aaxconfig ${SOURCES} ${MOC_HEADERS} ${RC_SRCS} ${GUI_HEADERS} )

TARGET_LINK_LIBRARIES( aaxconfig ${QT_LIBRARIES} ${XML_LIBRARY} ${AAX_LIBRARY})

INSTALL(TARGETS aaxconfig
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Applications
)

IF(WIN32)
  # Include Qt libraries
  CMP_QT_LIBRARIES_INSTALL_RULES("QtCore;QtGui" "bin")
  INSTALL_CUSTOM_LIB(${AAX_LIBRARY} "bin")

  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "${PACKAGE}" "${PACKAGE_NAME}")
  SET(CPACK_PACKAGE_ICON "${aaxconfig_SOURCE_DIR}/admin\\\\aaxconfig.xpm")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\aaxconfig.exe")
# SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\admin\aaxconfig.xpm")

ELSE(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "${PACKAGE};${PACKAGE_NAME}")
  SET(CPACK_STRIP_FILES "bin/${PACKAGE}")
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
  INSTALL(FILES ${aaxconfig_SOURCE_DIR}/admin/${PACKAGE}.desktop
          DESTINATION share/applications
          COMPONENT Applications)
  INSTALL(FILES ${aaxconfig_SOURCE_DIR}/admin/${PACKAGE}.xpm
          DESTINATION share/pixmaps
          COMPONENT Applications)
ENDIF(UNIX AND NOT APPLE)
