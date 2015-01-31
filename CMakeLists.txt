cmake_minimum_required(VERSION 2.8.3)
SET( cmake_verbose_makefile TRUE)

PROJECT(aaxsupport)
SET(PACKAGE_NAME "AeonWave-Support")
SET(PACKAGE "aaxsupport")

OPTION(WERROR "Treat compile warnings as errors"   OFF)

IF(WIN32)
  # By default, do not warn when built on machines using only VS Express:
  # and define the library location ourselves.
  IF(NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
    SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
    SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS
        "c:/Windows/System32/msvcr100.dll" 
        "c:/Windows/System32/msvcp100.dll")
  ENDIF(NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)

  # InstallRequiredSystemLibraries installs the runtime libraries without
  # specifying COMPONENT Runtime. We have to do it manually here:
  SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
  INSTALL(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin
          COMPONENT Applications)
  INCLUDE(InstallRequiredSystemLibraries)
ENDIF(WIN32)

# read 'version' file into a variable (stripping any newlines or spaces)
FILE(READ version versionFile)
STRING(STRIP ${versionFile} AAXCONFIG_VERSION)
SET(VERSION AAXCONFIG_VERSION)

# split version string into components, note CMAKE_MATCH_0 is the entire regexp match
STRING(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" CPACK_PACKAGE_VERSION ${AAXCONFIG_VERSION})

SET(AAXCONFIG_MAJOR_VERSION ${CMAKE_MATCH_1})
SET(AAXCONFIG_MINOR_VERSION ${CMAKE_MATCH_2})
SET(AAXCONFIG_MICRO_VERSION ${CMAKE_MATCH_3})

SET(CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1})
SET(CPACK_PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2})
SET(CPACK_PACKAGE_VERSION_PATCH ${CMAKE_MATCH_3})

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Graphical support utilities for AeonWave")

# We have some custom .cmake scripts not in the official distribution.
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

INCLUDE(CPack_Modules)
INCLUDE(CPack_Defaults)
INCLUDE(CheckLibraryExists)

IF(WIN32)
  SET(CPACK_NSIS_MODIFY_PATH "ON")
  ADD_DEFINITIONS("-D_WIN32")
ELSE(WIN32)

  # DEBIAN
  SET(CPACK_DEBIAN_PACKAGE_REPLACES "aaxconfig, aaxconfig-bin")
  SET(CPACK_DEBIAN_PACKAGE_SECTION "support utilities")
  SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.1), libaax2-bin (>= 2.5), libqt4-core (>= 4.6.2), libqt4-gui (>= 4.7.0)")

  # RPM
  SET(CPACK_RPM_PACKAGE_LICENSE "GPL v3")
  SET(CPACK_RPM_PACKAGE_OBSOLETES "aaxconfig-bin")
  SET(CPACK_RPM_PACKAGE_GROUP "System Environment/Base")
  SET(CPACK_RPM_PACKAGE_REQUIRES "libaax2-bin >= 2.2")
ENDIF(WIN32)

# ZIP
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${AAXCONFIG_VERSION}" CACHE INTERNAL "tarball basename")



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

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}"
   ${CMAKE_SOURCE_DIR}/include 
    "${CMAKE_CURRENT_BINARY_DIR}/include"
)

CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cmake_config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/config.h")

INCLUDE(CPack)

ADD_SUBDIRECTORY(aaxplayer)
ADD_SUBDIRECTORY(aaxrecorder)
ADD_SUBDIRECTORY(aaxconfig)

#-----------------------------------------------------------------------------
### uninstall target
#-----------------------------------------------------------------------------
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
#ADD_CUSTOM_TARGET(uninstall
#  "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")
