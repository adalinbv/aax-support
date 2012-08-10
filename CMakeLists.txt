cmake_minimum_required(VERSION 2.8.3)
SET( cmake_verbose_makefile TRUE)

PROJECT(aaxconfig)
SET(PACKAGE_NAME "AeonWave-Config")
SET(PACKAGE "aaxconfig")

OPTION(WERROR "Treat compile warnings as errors"   OFF)

# read 'version' file into a variable (stripping any newlines or spaces)
FILE(READ version versionFile)
STRING(STRIP ${versionFile} AAXCONFIG_VERSION)
SET(VERSION AAXCONFIG_VERSION)

# split version string into components, note CMAKE_MATCH_0 is the entire regexp match
STRING(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" CPACK_PACKAGE_VERSION ${AAXCONFIG_VERSION})

SET(CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1})
SET(CPACK_PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2})
SET(CPACK_PACKAGE_VERSION_PATCH ${CMAKE_MATCH_3})

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "AeonWave 4D audio library configuration applet")

# We have some custom .cmake scripts not in the official distribution.
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

INCLUDE(CPack_Modules)
INCLUDE(CPack_Defaults)

FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui)
INCLUDE( ${QT_USE_FILE} )

IF(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "${PACKAGE}" "${PACKAGE_NAME}")
  SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/admin\\\\aaxconfig.png")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\aaxconfig.exe")

  SET(CPACK_NSIS_MODIFY_PATH "ON")
  SET(CMAKE_INSTALL_PREFIX ${PACKAGE_NAME} CACHE PATH "Final directory"FORCE)
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_INSTALL_PREFIX}")
  ADD_DEFINITIONS("-D_WIN32")
#  SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\admin\aaxconfig.png")
  ADD_DEFINITIONS(/NODEFAULTLIB:msvcrt /NODEFAULTLIB:msvcrtd)

ELSE(WIN32)

  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  SET(CPACK_PACKAGE_EXECUTABLES "${PACKAGE};${PACKAGE_NAME}")
  SET(CPACK_STRIP_FILES "bin/${PACKAGE}")

  # DEBIAN
  SET(CPACK_DEBIAN_PACKAGE_SECTION "Sound")
  SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.1), libaax2-bin (>= 2.2), libqt4-core (>= 4.6.2), libqt4-gui (>= 4.6.2)")

  # RPM
  SET(CPACK_RPM_PACKAGE_LICENSE "GPL v3")
  SET(CPACK_RPM_PACKAGE_GROUP "System Environment/Base")
  SET(CPACK_RPM_PACKAGE_REQUIRES "libaax2-bin >= 2.2")
ENDIF(WIN32)

# ZIP
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${AAXCONFIG_VERSION}" CACHE INTERNAL "tarball basename")



# Required libraries
FIND_PACKAGE(AAX COMPONENTS aax REQUIRED)
FIND_PACKAGE(XML COMPONENTS xml REQUIRED)

ADD_DEFINITIONS(-DHAVE_CONFIG_H=1)
IF(WERROR)
  ADD_DEFINITIONS(-Werror)
ENDIF(WERROR)

IF(MSVC)
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -D_DEBUG /LD")
    SET(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -DNDEBUG /LD")
    SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG /LD")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG /LD")

    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -D_DEBUG /LD")
    SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -DNDEBUG /LD")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG /LD")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG /LD")

    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE)

    OPTION(FORCE_STATIC_VCRT "Force /MT for static VC runtimes" OFF)
    IF(FORCE_STATIC_VCRT)
        FOREACH(flag_var
                CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
                CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            IF(${flag_var} MATCHES "/MD")
                STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
            ENDIF()
        ENDFOREACH(flag_var)
    ENDIF()
ELSE(MSVC)
    IF(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
       SET(C_FLAGS "${C_FLAGS} -march=i586")
       SET(CXX_FLAGS "${CXX_FLAGS} -march=i586")
    ENDIF(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)

    SET(C_FLAGS "${C_FLAGS} -Os -Wall -Werror=format-security" CACHE STRING
        "Flags used by al build types."
        FORCE)

    SET(CXX_FLAGS "${CXX_FLAGS} -Os -Wall -Werror=format-security" CACHE STRING
        "Flags used by al build types."
        FORCE)

    SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${C_FLAGS} -g -O2 -D_DEBUG" CACHE STRING
        "Flags used by the compiler during Release with Debug Info builds."
        FORCE)
    SET(CMAKE_C_FLAGS_MINSIZEREL "${C_FLAGS} -Os -DNDEBUG" CACHE STRING
        "Flags used by the compiler during release minsize builds."
        FORCE)
    SET(CMAKE_C_FLAGS_RELEASE "${C_FLAGS} -Os -fomit-frame-pointer -DNDEBUG"
        CACHE STRING "Flags used by the compiler during release builds"
        FORCE)
    SET(CMAKE_C_FLAGS_DEBUG "${C_FLAGS} -g3 -D_DEBUG" CACHE STRING
        "Flags used by the compiler during debug builds."
        FORCE)

    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${C_FLAGS} -g -O2 -D_DEBUG" CACHE STRING
        "Flags used by the compiler during Release with Debug Info builds."
        FORCE)
    SET(CMAKE_CXX_FLAGS_MINSIZEREL "${C_FLAGS} -Os -DNDEBUG" CACHE STRING
        "Flags used by the compiler during release minsize builds."
        FORCE)
    SET(CMAKE_CXX_FLAGS_RELEASE "${C_FLAGS} -Os -fomit-frame-pointer -DNDEBUG"
        CACHE STRING "Flags used by the compiler during release builds"
        FORCE)
    SET(CMAKE_CXX_FLAGS_DEBUG "${C_FLAGS} -g3 -D_DEBUG" CACHE STRING
        "Flags used by the compiler during debug builds."
        FORCE)
ENDIF(MSVC)

INCLUDE( ${QT_USE_FILE} )

SET( SOURCES src/main.cpp src/aaxconfig.cpp src/api.c )
SET( HEADERS src/aaxconfig.h src/aaxconfig_ui.h src/api.h src/types.h )
SET( GUI src/aaxconfig.ui )
SET( RCS src/aaxconfig.qrc )

QT4_ADD_RESOURCES(RC_SRCS ${RCS})
QT4_WRAP_UI( GUI_HDRS ${GUI} )
QT4_WRAP_CPP( MOC_HEADERS ${HEADERS} )

INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} ${AAX_INCLUDE_DIR} ${XML_INCLUDE_DIR} )

ADD_EXECUTABLE(aaxconfig ${SOURCES} ${MOC_HEADERS} ${RC_SRCS} ${GUI_HEADERS} )

TARGET_LINK_LIBRARIES( aaxconfig ${QT_LIBRARIES} ${XML_LIBRARY} ${AAX_LIBRARY})

INSTALL(TARGETS aaxconfig
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT Applications
)

IF(WIN32)
  # Include Qt libraries
  SET(plugin_dest_dir bin)
  SET(qtconf_dest_dir bin)

  # Install needed Qt plugins by copying directories from the qt installation
  # One can cull what gets copied by using 'REGEX "..." EXCLUDE'
  # Exclude debug libraries
# INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" 
#         DESTINATION ${plugin_dest_dir}/plugins 
#         COMPONENT runtime 
#         REGEX ".*d4.dll" EXCLUDE
#         REGEX ".*d4.a" EXCLUDE)

  # install a qt.conf file
  # this inserts some cmake code into the install script to write the file
  SET(QT_CONF_FILE [Paths]\nPlugins=plugins)
  INSTALL(CODE "
    file(WRITE \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"${QT_CONF_FILE}\")
    " COMPONENT Applications)

  # directories to look for dependencies
  SET(DIRS ${QT_LIBRARY_DIRS} ${PROJECT_SOURCE_DIR}/bin)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/aaxconfig.exe")

  # Now the work of copying dependencies into the bundle/package
  # The quotes are escaped and variables to use at install time have their $ escaped
  # An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
  # Note that the image plugins depend on QtSvg and QtXml, and it got those copied
  # over.
  # To find dependencies, cmake use "otool" on Apple and "dumpbin" on Windows (make sure you have one of them).
#  INSTALL(CODE "
#   file(GLOB_RECURSE QTPLUGINS \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
#    include(\"BundleUtilities\")
#    fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
#  " COMPONENT runtime)

   CMP_QT_LIBRARIES_INSTALL_RULES("QtCore;QtGui" "bin")
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
  INSTALL(FILES admin/${PACKAGE}.desktop DESTINATION share/applications
          COMPONENT Applications)
  INSTALL(FILES admin/${PACKAGE}.png DESTINATION share/pixmaps
          COMPONENT Applications)
ENDIF(UNIX AND NOT APPLE)

INCLUDE(CPack)

#-----------------------------------------------------------------------------
### uninstall target
#-----------------------------------------------------------------------------
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

