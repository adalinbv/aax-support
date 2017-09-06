cmake_minimum_required(VERSION 2.8.11)
SET( cmake_verbose_makefile TRUE)

PROJECT(aaxsupport)
SET(PACKAGE_NAME "AeonWave-Support")
SET(PACKAGE "aaxsupport")

OPTION(WERROR "Treat compile warnings as errors"   OFF)

IF(WIN32)
  IF (EXISTS "C:/Windows/SysWOW64/msvcp100.dll")
    SET(MSVC10_MSVCP100 "C:/Windows/SysWOW64/msvcp100.dll")
  ELSEIF(EXISTS "C:/Windows/System32/msvcp100.dll")
    SET(MSVC10_MSVCP100 "C:/Windows/System32/msvcp100.dll")
  ENDIF()
  IF (EXISTS "C:/Windows/SysWOW64/msvcr100.dll")
    SET(MSVC10_MSVCR100 "C:/Windows/SysWOW64/msvcr100.dll")
  ELSEIF(EXISTS "C:/Windows/System32/msvcr100.dll")
    SET(MSVC10_MSVCR100 "C:/Windows/System32/msvcr100.dll")
  ENDIF()

  # By default, do not warn when built on machines using only VS Express:
  # and define the library location ourselves.
  IF(NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
    SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
    SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS
        "${MSVC10_MSVCR100}"
        "${MSVC10_MSVCP100}")
  MESSAGE("Installing: ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}")
  ENDIF(NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)

  # InstallRequiredSystemLibraries installs the runtime libraries without
  # specifying COMPONENT Runtime. We have to do it manually here:
  SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
  INSTALL(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin
         COMPONENT Support)
  INSTALL(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin
         COMPONENT Media)
  INCLUDE(InstallRequiredSystemLibraries)
ENDIF(WIN32)

# read 'version' file into a variable (stripping any newlines or spaces)
FILE(READ version versionFile)
STRING(STRIP ${versionFile} AAXSUPPORT_VERSION)
SET(VERSION AAXSUPPORT_VERSION)

# split version string into components, note CMAKE_MATCH_0 is the entire regexp match
STRING(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" CPACK_PACKAGE_VERSION ${AAXSUPPORT_VERSION})

SET(AAXSUPPORT_MAJOR_VERSION ${CMAKE_MATCH_1})
SET(AAXSUPPORT_MINOR_VERSION ${CMAKE_MATCH_2})
SET(AAXSUPPORT_MICRO_VERSION ${CMAKE_MATCH_3})

SET(CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1})
SET(CPACK_PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2})
SET(CPACK_PACKAGE_VERSION_PATCH ${CMAKE_MATCH_3})

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Graphical support utilities for AeonWave")

# We have some custom .cmake scripts not in the official distribution.
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

INCLUDE(CPack_Modules)
INCLUDE(CPack_Defaults)
INCLUDE(CheckLibraryExists)


SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)
FIND_PACKAGE(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Network)
IF(WIN32)
  FIND_PACKAGE(Qt5WinMain REQUIRED)
  SET (QT_USE_QTMAIN TRUE)
  SET(CPACK_NSIS_MODIFY_PATH "ON")
  ADD_DEFINITIONS("-D_WIN32")
  CMP_QT_LIBRARIES_INSTALL_RULES("QtCore;QtGui" "bin" "Support")
  CMP_QT_LIBRARIES_INSTALL_RULES("QtCore;QtGui;QtNetwork" "bin" "Media")

ELSE(WIN32)

  # DEBIAN
  SET(CPACK_DEBIAN_PACKAGE_REPLACES "aaxconfig, aaxconfig-bin")
  SET(CPACK_DEBIAN_PACKAGE_SECTION "support utilities")
  SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.1), libaax3-bin (>= 3.0), libqt5core5a (>= 5.1.0), libqt5gui5 (>= 5.1.0), libqt5network5 (>= 5.1.0)")

  # RPM
  SET(CPACK_RPM_PACKAGE_LICENSE "GPL v3")
  SET(CPACK_RPM_PACKAGE_OBSOLETES "aaxconfig-bin")
  SET(CPACK_RPM_PACKAGE_GROUP "System Environment/Base")
  SET(CPACK_RPM_PACKAGE_REQUIRES "libaax3-bin >= 3.0")
ENDIF(WIN32)

# ZIP
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${AAXSUPPORT_VERSION}" CACHE INTERNAL "tarball basename")



ADD_DEFINITIONS(-DHAVE_CONFIG_H=1)
IF(WERROR)
  ADD_DEFINITIONS(-Werror)
ENDIF(WERROR)

IF(MSVC)
#   SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmt")
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

    OPTION(FORCE_STATIC_VCRT "Force /MT for static VC runtimes" ON)
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
    IF(X86_64)
       SET(C_FLAGS "${C_FLAGS} -march=athlon64 -msse2 -mfpmath=sse")
       SET(CXX_FLAGS "${CXX_FLAGS} -march=athlon64 -msse2 -mfpmath=sse")
    ELSEIF(X86)
       SET(C_FLAGS "${C_FLAGS} -march=i686")
       SET(CXX_FLAGS "${CXX_FLAGS} -march=i686")
    ELSEIF(ARM)
       SET(C_FLAGS "${C_FLAGS} -march=armv5 -marm")
       SET(CXX_FLAGS "${CXX_FLAGS} -march=armv5 -marm")
    ENDIF()

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

# Required libraries
FIND_PACKAGE(AAX COMPONENTS aax REQUIRED)
FIND_PACKAGE(XML COMPONENTS xml REQUIRED)

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}"
   "${CMAKE_SOURCE_DIR}/include"
   "${PROJECT_BINARY_DIR}/include"
   "${AAX_INCLUDE_DIR}"
   "${XML_INCLUDE_DIR}"
)

CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cmake_config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/config.h")

SET(BASE_OBJS
     base/api.c
     base/types.c
   )
SET(LIBBASE base)
ADD_LIBRARY(${LIBBASE} STATIC ${BASE_OBJS})
SET(EXTRA_LIBS ${EXTRA_LIBS} ${LIBBASE})

SET(CPACK_PACKAGE_EXECUTABLES "" "")
ADD_SUBDIRECTORY(aaxplayer)
ADD_SUBDIRECTORY(aaxrecorder)
ADD_SUBDIRECTORY(aaxconfig)

#IF(WIN32)
   # Used to create Start Menu shortcuts <EXEC> <LABEL>
SET(CPACK_PACKAGE_EXECUTABLES "aaxconfig;AeonWave Config" "aaxplayer;Audio Player" "aaxrecorder;Audio Recorder")
#  SET(CPACK_PACKAGE_EXECUTABLES "aaxconfig" "AeonWave Config")
#  SET(CPACK_PACKAGE_EXECUTABLE "aaxplayer" "Audio Player")
#  SET(CPACK_PACKAGE_EXECUTABLE "aaxrecorder" "Audio Recorder")
#ENDIF(WIN32)

INCLUDE(CPack)

#-----------------------------------------------------------------------------
### uninstall target
#-----------------------------------------------------------------------------
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
#ADD_CUSTOM_TARGET(uninstall
#  "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")
