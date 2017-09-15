
# Descriptions
SET(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
   "Utilities to show the capabilities of AeonWave")

# Component support
SET(CPACK_COMPONENTS_ALL Support Media)

SET(CPACK_COMPONENT_APPLICATIONS_DEPENDS Support)
SET(CPACK_COMPONENT_APPLICATIONS_DEPENDS Media)

# Display name
SET(CPACK_COMPONENT_APPLICATIONS_HIDDEN ON)
SET(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Applications")

# Component grouping
IF(WIN32)
  # NSIS
  SET(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
  SET(CPACK_COMPONENT_SUPPORT_GROUP "Runtime")
  SET(CPACK_COMPONENT_MEDIA_GROUP "Runtime")

  # Note Windows DLL are specified by RUNTIME
  SET(CPACK_COMPONENT_GROUP_RUNTIME_DESCRIPTION
     "All software components")
  SET(CPACK_COMPONENT_SUPPORT_DESCRIPTION
     "Configuration utility for AeonWave")
  SET(CPACK_COMPONENT_MEDIA_DESCRIPTION
     "Audio player and recorder")

  # GUI grouping, NSIS only
  SET(CPACK_ALL_INSTALL_TYPES Runtime)
  SET(CPACK_COMPONENT_SUPPORT_INSTALL_TYPES Runtime)
  SET(CPACK_COMPONENT_MEDIA_INSTALL_TYPES Runtime)
  SET(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Runtime)

ELSE(WIN32)
  SET(CPACK_COMPONENT_APPLICATIONS_GROUP "bin")
  SET(CPACK_COMPONENT_SUPPORT_GROUP "bin")
  SET(CPACK_COMPONENT_MEDIA_GROUP "bin")

  SET(CPACK_COMPONENT_GROUP_BIN_DESCRIPTION
     "All software components")

ENDIF(WIN32)


# --------------------------------------------------------------------
# 
# 
macro (CMP_QT_LIBRARIES_INSTALL_RULES QTLIBLIST destination comp)
  # message(STATUS "CMP_COPY_QT4_RUNTIME_LIBRARIES")
  if (WIN32)
    FOREACH(qtlib ${QTLIBLIST})
      get_target_property(QT_DLL_PATH_tmp ${qtlib} LOCATION)
      message(STATUS "Generating Install Rule for Qt Debug DLL Library ${QT_DLL_PATH_tmp}")  
      INSTALL(FILES ${QT_DLL_PATH_tmp} 
              DESTINATION "${destination}"
              CONFIGURATIONS Debug
              COMPONENT "${comp}"
             )
      message(STATUS "Generating Install Rule for Qt Release DLL Library ${QT_DLL_PATH_tmp}")
      INSTALL(FILES ${QT_DLL_PATH_tmp}
              DESTINATION "${destination}"
              CONFIGURATIONS Release
              COMPONENT "${comp}"
             )
    ENDFOREACH(qtlib)
  endif()
endmacro()

macro (INSTALL_CUSTOM_LIB customlib destination comp)
  if (WIN32)
    SET(LIBNAME "${customlib}")
    IF(${customlib} MATCHES "/lib/lib[A-Za-z0-9]+.lib$")
      STRING(REGEX REPLACE "/lib/lib([A-Za-z0-9]+).lib$" "/bin/\\1.dll" LIBNAME ${LIBNAME})
    ENDIF(${customlib} MATCHES "/lib/lib[A-Za-z0-9]+.lib$")

    message(STATUS "Generating Install Rule for DLL Library ${LIBNAME}")
    INSTALL(FILES ${LIBNAME} 
             DESTINATION "${destination}"
             CONFIGURATIONS Release
             COMPONENT "${comp}"
           )
  endif()
endmacro()

macro (INSTALL_CUSTOM_FILES FILELIST destination comp)
  FILE(GLOB FILES ${FILELIST})
  FOREACH(instfile ${FILES})
    message(STATUS "Adding ${instfile} to ${destination}")
    INSTALL(FILES ${instfile}
            DESTINATION "${destination}"
            CONFIGURATIONS Release Debug
            COMPONENT "${comp}")
  ENDFOREACH(instfile)
endmacro()
