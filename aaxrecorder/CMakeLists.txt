
enable_language(CXX)

check_library_exists(m floorf "" HAVE_LIBM)
if(HAVE_LIBM)
  set(EXTRA_LIBS m ${EXTRA_LIBS})
endif(HAVE_LIBM)

find_package(AAX COMPONENTS aax REQUIRED)

set(SOURCES aaxrecorder.cpp  api.c  main.cpp  record.cpp  setup.cpp)
qt_wrap_ui(GUI aaxrecorder.ui record.ui setup.ui)
qt_add_resources(RC aaxrecorder.qrc aaxrecorder.rc)

add_executable(aaxrecorder ${SOURCES} ${RC} ${GUI} )

target_link_libraries( aaxrecorder
    ${AAX_LIBRARY}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

install(TARGETS aaxrecorder
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Media
)

target_include_directories(aaxrecorder PRIVATE 
      ${PROJECT_BINARY_DIR}/aaxrecorder
      ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  set(CPACK_PACKAGE_EXECUTABLES "aaxrecorder;AeonWave Audio Recorder")
  set(CPACK_PACKAGE_ICON "admin\\\\aaxrecorder.xpm")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\aaxrecorder.exe")

else(WIN32)
  # Used to create Start Menu shortcuts <EXEC> <LABEL>
  set(CPACK_PACKAGE_EXECUTABLES "aaxrecorder;AeonWave Audio Recorder")
  set(CPACK_STRIP_FILES "bin/aaxrecorder")
endif(WIN32)

if(APPLE)
  set(PLATFORM_SPECIFIC "aaxrecorder.icns")
  SET_SOURCE_FILES_PROPERTIES(
      "aaxrecorder.icns"
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
  )
  set(GUI_TYPE MACOSX_BUNDLE)
  set(MACOSX_BUNDLE_ICON_FILE MacIcon.icns)
endif()

if(UNIX AND NOT APPLE)
  install(FILES admin/aaxrecorder.desktop
          DESTINATION share/applications
          COMPONENT Media)
  install(FILES admin/aaxrecorder.xpm
          DESTINATION share/pixmaps
          COMPONENT Media)
endif(UNIX AND NOT APPLE)

