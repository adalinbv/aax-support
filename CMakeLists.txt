cmake_minimum_required(VERSION 2.4.7)
SET( cmake_verbose_makefile TRUE)

PROJECT(aaxconfig)

# read 'version' file into a variable (stripping any newlines or spaces)
file(READ version versionFile)
string(STRIP ${versionFile} AAXCONFIG_VERSION)

# read 'description` file into a variable
file(STRINGS description descriptionFile)
STRING(REGEX REPLACE ";" " " descriptionFile "${descriptionFile}")

# split version string into components, note CMAKE_MATCH_0 is the entire regexp match
STRING(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" CPACK_PACKAGE_VERSION ${AAXCONFIG_VERSION} )
SET(CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1})
SET(CPACK_PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2})
SET(CPACK_PACKAGE_VERSION_PATCH ${CMAKE_MATCH_3})
SET(CPACK_PACKAGE_NAME "AeonWave-Config")
SET(CPACK_PACKAGE_CONTACT "tech@adalin.org")
SET(CPACK_PACKAGE_VENDOR "tech@adalin.org")
SET(CPACK_PACKAGE_DESCRIPTION "Graphical AeonWave Configuration utility")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${descriptionFile})
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/description")

SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING")
SET(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README")

# DEBIAN
IF (X86_64_SUPPORTED)
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
ELSE (X86_64_SUPPORTED)
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
ENDIF (X86_64_SUPPORTED)
SET(CPACK_DEBIAN_PACKAGE_SECTION "sound")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.1), libaax2 (>= 2.0), libqt4-core (>= 4.6.2), libqt4-gui (>= 4.6.2)")

# RPM
IF (X86_64_SUPPORTED)
  SET(CPACK_RPM_PACKAGE_ARCHITECTURE "amd64")
ELSE (X86_64_SUPPORTED)
  SET(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")
ENDIF (X86_64_SUPPORTED)
SET(CPACK_RPM_PACKAGE_GROUP "System/Libraries")


SET(CPACK_SOURCE_GENERATOR TBZ2 ZIP)
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${AAXCONFIG_VERSION}" CACHE INTERNAL "tarball basename")
SET(CPACK_SOURCE_IGNORE_FILES
  "^${PROJECT_SOURCE_DIR}/.git;\\\\.gitignore;Makefile.am;~$;${CPACK_SOURCE_IGNORE_FILES}")

SET(CPACK_GENERATOR "DEB;RPM")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Adalin B.V.")

INCLUDE (CPack)

# We have some custom .cmake scripts not in the official distribution.
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

SET( SOURCES src/main.cpp src/aaxconfig.cpp )

SET( HEADERS src/aaxconfig.h src/aaxconfig_ui.h )

SET( GUI src/aaxconfig.ui )

ADD_DEFINITIONS( -Wall)

FIND_PACKAGE(Qt4 COMPONENTS  QtCore QtGui REQUIRED)
FIND_PACKAGE(AAX COMPONENTS aax REQUIRED)
FIND_PACKAGE(XML COMPONENTS xml REQUIRED)

INCLUDE( ${QT_USE_FILE} )

QT4_WRAP_UI( GUI_HEADERS ${GUI} )
QT4_WRAP_CPP( MOC_HEADERS ${HEADERS} )

INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} ${AAX_INCLUDE_DIR} ${XML_INCLUDE_DIR} )

ADD_EXECUTABLE(aaxconfig ${SOURCES} ${MOC_HEADERS} ${GUI_HEADERS} )

TARGET_LINK_LIBRARIES( aaxconfig ${QT_LIBRARIES} ${XML_LIBRARY} ${AAX_LIBRARY})

INSTALL( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/aaxconfig DESTINATION bin)

#-----------------------------------------------------------------------------
### uninstall target
#-----------------------------------------------------------------------------
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

