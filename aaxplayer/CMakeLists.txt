
enable_language(CXX)

check_library_exists(m floorf "" HAVE_LIBM)

find_package(AAX COMPONENTS aax REQUIRED)
find_package(XML COMPONENTS xml REQUIRED)

set(SOURCES aaxplayer.cpp  main.cpp  remote.cpp  setup.cpp)
qt_wrap_ui(GUI aaxplayer.ui remote.ui setup.ui)
qt_add_resources(RC aaxplayer.qrc aaxplayer.rc)

add_executable(aaxplayer ${SOURCES} ${RC} ${GUI} )

target_link_libraries( aaxplayer
    ${AAX_LIBRARY}
    ${XML_LIBRARY}
    ${EXTRA_LIBS}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
)

install(TARGETS aaxplayer
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Media
)

target_include_directories(aaxplayer PRIVATE 
      ${PROJECT_BINARY_DIR}/aaxplayer
      ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  set(CPACK_PACKAGE_EXECUTABLES "aaxplayer;AeonWave Audio Player")
  set(CPACK_PACKAGE_ICON "admin\\\\aaxplayer.xpm")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\aaxplayer.exe")

else(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  set(CPACK_PACKAGE_EXECUTABLES "aaxplayer;AeonWave Audio Player")
  set(CPACK_STRIP_FILES "bin/aaxplayer")
endif(WIN32)

if(APPLE)
  set(PLATFORM_SPECIFIC "aaxplayer.icns")
  set_soruce_files_properties(
      "aaxplayer.icns"
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
  )
  set(GUI_TYPE MACOSX_BUNDLE)
  set(MACOSX_BUNDLE_ICON_FILE MacIcon.icns)
endif()

if(UNIX AND NOT APPLE)
  install(FILES admin/aaxplayer.desktop
          DESTINATION share/applications
          COMPONENT Media)
  install(FILES admin/aaxplayer.xpm
          DESTINATION share/pixmaps
          COMPONENT Media)
endif(UNIX AND NOT APPLE)

