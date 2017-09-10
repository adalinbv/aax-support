
ENABLE_LANGUAGE(CXX)

CHECK_LIBRARY_EXISTS(m floorf "" HAVE_LIBM)

find_package(AAX COMPONENTS aax REQUIRED)
find_package(XML COMPONENTS xml REQUIRED)

file( GLOB GUI *.ui )
file( GLOB SOURCES *.cpp *.c )
file( GLOB RC *.rc *.qrc )

add_executable(aaxconfig ${SOURCES} ${RC} ${GUI} )

target_link_libraries(aaxconfig
    ${AAX_LIBRARY}
    ${XML_LIBRARY}
    ${EXTRA_LIBS}
    Qt5::WinMain
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)

install(TARGETS aaxconfig
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Support
)

if(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  set(CPACK_PACKAGE_EXECUTABLES "aaxconfig;AeonWave Config")
  set(CPACK_PACKAGE_ICON "admin\\\\aaxconfig.xpm")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\aaxconfig.exe")

else(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  set(CPACK_PACKAGE_EXECUTABLES "aaxconfig;AeonWave Config")
  set(CPACK_STRIP_FILES "bin/aaxconfig")
endif(WIN32)

if(APPLE)
  set(PLATFORM_SPECIFIC "aaxconfig.icns")
  set_source_files_properties(
      "aaxconfig.icns"
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
  )
  set(GUI_TYPE MACOSX_BUNDLE)
  set(MACOSX_BUNDLE_ICON_FILE MacIcon.icns)
endif()

if(UNIX AND NOT APPLE)
  install(FILES admin/aaxconfig.desktop
          DESTINATION share/applications
          COMPONENT Support)
  install(FILES admin/aaxconfig.xpm
          DESTINATION share/pixmaps
          COMPONENT Support)
endif(UNIX AND NOT APPLE)

