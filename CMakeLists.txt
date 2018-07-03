cmake_minimum_required(VERSION 3.9.1)

set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.7.0/5.7/msvc2015_64")
set( cmake_verbose_makefile TRUE)

project(aaxsupport)
set(PACKAGE_NAME "AeonWave-Support")
set(PACKAGE "aaxsupport")

option(WERROR "Treat compile warnings as errors"   OFF)

# read 'version' file into a variable (stripping any newlines or spaces)
file(READ version versionFile)
string(STRIP ${versionFile} AAXSUPPORT_VERSION)
set(VERSION AAXSUPPORT_VERSION)

# split version string into components, note CMAKE_MATCH_0 is the entire regexp match
string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" CPACK_PACKAGE_VERSION ${AAXSUPPORT_VERSION})

set(AAXSUPPORT_MAJOR_VERSION ${CMAKE_MATCH_1})
set(AAXSUPPORT_MINOR_VERSION ${CMAKE_MATCH_2})
set(AAXSUPPORT_MICRO_VERSION ${CMAKE_MATCH_3})

set(CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1})
set(CPACK_PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2})
set(CPACK_PACKAGE_VERSION_PATCH ${CMAKE_MATCH_3})

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Graphical support utilities for AeonWave")

# We have some custom .cmake scripts not in the official distribution.
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

include(CPack_Modules)
include(CPack_Defaults)
include(CheckLibraryExists)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network)
if(WIN32)
  set(CPACK_NSIS_MODifY_PATH "ON")
  add_definitions("-D_WIN32")

  install_qt5_plugin("Qt5::QMinimalIntegrationPlugin" QT_PLUGIN "bin" "Support")
  install_qt5_plugin("Qt5::QWindowsIntegrationPlugin" QT_PLUGIN "bin" "Support")
  install_qt5_plugin("Qt5::QMinimalIntegrationPlugin" QT_PLUGIN "bin" "Media")
  install_qt5_plugin("Qt5::QWindowsIntegrationPlugin" QT_PLUGIN "bin" "Media")

else(WIN32)

  # DEBIAN
  set(CPACK_DEBIAN_PACKAGE_REPLACES "aaxconfig, aaxconfig-bin")
  set(CPACK_DEBIAN_PACKAGE_SECTION "support utilities")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.1), libaax3-bin (>= 3.0), libqt5core5a (>= 5.1.0), libqt5gui5 (>= 5.1.0), libqt5network5 (>= 5.1.0)")

  # RPM
  set(CPACK_RPM_PACKAGE_LICENSE "GPL v3")
  set(CPACK_RPM_PACKAGE_OBSOLETES "aaxconfig-bin")
  set(CPACK_RPM_PACKAGE_GROUP "System Environment/Base")
  set(CPACK_RPM_PACKAGE_REQUIRES "libaax3-bin >= 3.0")
endif(WIN32)

# ZIP
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${AAXSUPPORT_VERSION}" CACHE INTERNAL "tarball basename")

add_definitions(-DHAVE_CONFIG_H=1)
if(WERROR)
  add_definitions(-Werror)
endif(WERROR)

if(WIN32)
  set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ".")
  set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
  include(InstallRequiredSystemLibraries)

  get_target_property(QtCore_location Qt5::Core LOCATION)
  get_target_property(QtGui_location Qt5::Gui LOCATION)
  get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
  get_target_property(QtNetworklocation Qt5::Network LOCATION)
  set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS "${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS};${QtCore_location};${QtGui_location};${QtWidgets_location};${QtNetworklocation}")
  message("Installing: ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}")

  # InstallRequiredSystemLibraries installs the runtime libraries without
  # specifying COMPONENT Runtime. We have to do it manually here:
  install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
          DESTINATION bin
          COMPONENT Support)
  install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
          DESTINATION bin
          COMPONENT Media)
endif(WIN32)

if(MSVC)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -D_DEBUG")
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -DNDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib /entry:mainCRTStartup")

    option(FORCE_STATIC_VCRT "Force /MT for static VC runtimes" OFF)
    if(FORCE_STATIC_VCRT)
        FOREACH(flag_var
                CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
                CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
            if(${flag_var} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
            endif()
        ENDFOREACH(flag_var)
    endif()

else(MSVC)
    if(X86_64)
       set(C_FLAGS "${C_FLAGS} -march=athlon64 -msse2 -mfpmath=sse")
       set(CXX_FLAGS "${CXX_FLAGS} -march=athlon64 -msse2 -mfpmath=sse")
    ELSEif(X86)
       set(C_FLAGS "${C_FLAGS} -march=i686")
       set(CXX_FLAGS "${CXX_FLAGS} -march=i686")
    ELSEif(ARM)
       set(C_FLAGS "${C_FLAGS} -march=armv5 -marm")
       set(CXX_FLAGS "${CXX_FLAGS} -march=armv5 -marm")
    endif()

    set(C_FLAGS "${C_FLAGS} -Os -Wall -Werror=format-security" CACHE STRING
        "Flags used by al build types."
        FORCE)

    set(CXX_FLAGS "${CXX_FLAGS} -Os -Wall -Werror=format-security" CACHE STRING
        "Flags used by al build types."
        FORCE)

    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${C_FLAGS} -g -O2 -D_DEBUG" CACHE STRING
        "Flags used by the compiler during Release with Debug Info builds."
        FORCE)
    set(CMAKE_C_FLAGS_MINSIZEREL "${C_FLAGS} -Os -DNDEBUG" CACHE STRING
        "Flags used by the compiler during release minsize builds."
        FORCE)
    set(CMAKE_C_FLAGS_RELEASE "${C_FLAGS} -Os -fomit-frame-pointer -DNDEBUG"
        CACHE STRING "Flags used by the compiler during release builds"
        FORCE)
    set(CMAKE_C_FLAGS_DEBUG "${C_FLAGS} -g3 -D_DEBUG" CACHE STRING
        "Flags used by the compiler during debug builds."
        FORCE)

    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${C_FLAGS} -g -O2 -D_DEBUG" CACHE STRING
        "Flags used by the compiler during Release with Debug Info builds."
        FORCE)
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${C_FLAGS} -Os -DNDEBUG" CACHE STRING
        "Flags used by the compiler during release minsize builds."
        FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "${C_FLAGS} -Os -fomit-frame-pointer -DNDEBUG"
        CACHE STRING "Flags used by the compiler during release builds"
        FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "${C_FLAGS} -g3 -D_DEBUG" CACHE STRING
        "Flags used by the compiler during debug builds."
        FORCE)
endif(MSVC)

# Required libraries
find_package(AAX COMPONENTS aax REQUIRED)
find_package(XML COMPONENTS xml REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}"
   "${CMAKE_SOURCE_DIR}/include"
   "${PROJECT_BINARY_DIR}/include"
   "${AAX_INCLUDE_DIR}"
   "${XML_INCLUDE_DIR}"
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cmake_config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/config.h")

set(BASE_OBJS
     base/api.c
     base/types.c
   )
set(LIBBASE base)
add_library(${LIBBASE} STATIC ${BASE_OBJS})
set(EXTRA_LIBS ${EXTRA_LIBS} ${LIBBASE})

set(CPACK_PACKAGE_EXECUTABLES "" "")
add_subdirectory(aaxplayer)
add_subdirectory(aaxrecorder)
add_subdirectory(aaxconfig)

set(CPACK_PACKAGE_EXECUTABLES "aaxconfig;AeonWave Config" "aaxplayer;Audio Player" "aaxrecorder;Audio Recorder")

include(CPack)

#-----------------------------------------------------------------------------
### uninstall target
#-----------------------------------------------------------------------------
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
#add_CUSTOM_TARGET(uninstall
#  "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")
