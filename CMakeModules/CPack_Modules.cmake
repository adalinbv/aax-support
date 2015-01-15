
# Descriptions
SET(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
   "Support applictaions to test the capabilities of the software")

# Component grouping
IF(WIN32)
  # NSIS

  # Component support
  SET(CPACK_COMPONENTS_ALL Applications)

  # Display name
  SET(CPACK_COMPONENT_APPLICATIONS_HIDDEN ON)
  SET(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Applictaions") 

  SET(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
  SET(CPACK_COMPONENT_AAXCONFIG_GROUP "Support")
  SET(CPACK_COMPONENT_AAXMEDIA_GROUP "Media")

  # Note Windows DLL are specified by RUNTIME
  SET(CPACK_COMPONENT_GROUP_RUNTIME_DESCRIPTION
     "Software required to run the software")

  SET(CPACK_ALL_INSTALL_TYPES Runtime)
  SET(CPACK_COMPONENT_AAXCONFIG_INSTALL_TYPES Runtime)
  SET(CPACK_COMPONENT_AAXMEDIA_INSTALL_TYPES Runtime)
  SET(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Runtime)

ELSE(WIN32)
# SET(CPACK_COMPONENTS_ALL Support Media)
# SET(CPACK_COMPONENT_AAXCONFIG_DISPLAY_NAME "aaxconfig-bin")
# SET(CPACK_COMPONENT_AAXMEDIA_DISPLAY_NAME "aaxmedia-bin")
# SET(CPACK_COMPONENT_AAXCONFIG_GROUP "bin")
# SET(CPACK_COMPONENT_AAXMEDIA_GROUP "bin")
# SET(CPACK_AAXCONFIG_USE_DISPLAY_NAME_IN_FILENAME ON)
#SET(CPACK_ALL_INSTALL_TYPES aaxconfig-bin aaxmedia-bin)

  # Component support
  SET(CPACK_COMPONENTS_ALL Applications Support Media)

  SET(CPACK_COMPONENT_APPLICATIONS_DEPENDS Support)
  SET(CPACK_COMPONENT_APPLICATIONS_DEPENDS Media)

  # Display name
  SET(CPACK_COMPONENT_APPLICATIONS_HIDDEN ON)
  SET(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Applictaions")

  SET(CPACK_COMPONENT_APPLICATIONS_GROUP "bin")
  SET(CPACK_COMPONENT_SUPPORT_GROUP "bin")
  SET(CPACK_COMPONENT_MEDIA_GROUP "bin")

  SET(CPACK_ALL_INSTALL_TYPES bin dev)
  SET(CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES bin dev)
  SET(CPACK_COMPONENT_HEADERS_INSTALL_TYPES dev)
  SET(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES bin)

  SET(CPACK_COMPONENT_GROUP_BIN_DESCRIPTION
     "Software required to run the software")

ENDIF(WIN32)


# --------------------------------------------------------------------
# 
# 
macro (CMP_QT_LIBRARIES_INSTALL_RULES QTLIBLIST destination)
  # message(STATUS "CMP_COPY_QT4_RUNTIME_LIBRARIES")
  if (MSVC)
    if (DEFINED QT_QMAKE_EXECUTABLE)
      set(TYPE "d")
      FOREACH(qtlib ${QTLIBLIST})
 
        GET_FILENAME_COMPONENT(QT_DLL_PATH_tmp ${QT_QMAKE_EXECUTABLE} PATH)
        message(STATUS "Generating Install Rule for Qt Debug DLL Library ${QT_DLL_PATH_tmp}/${qtlib}d4.dll")  
        INSTALL(FILES ${QT_DLL_PATH_tmp}/${qtlib}${type}d4.dll 
                DESTINATION "${destination}"
                CONFIGURATIONS Debug
                COMPONENT Applications)
        message(STATUS "Generating Install Rule for Qt Release DLL Library ${QT_DLL_PATH_tmp}/${qtlib}4.dll")
        INSTALL(FILES ${QT_DLL_PATH_tmp}/${qtlib}4.dll 
                DESTINATION "${destination}"
                CONFIGURATIONS Release
                COMPONENT Applications)  
      ENDFOREACH(qtlib)
    endif(DEFINED QT_QMAKE_EXECUTABLE)
  endif()
endmacro()

macro (INSTALL_CUSTOM_LIB customlib destination)
  if (MSVC)
    SET(LIBNAME "${customlib}")
    IF(${customlib} MATCHES "/lib/lib[A-Za-z0-9]+.lib$")
      STRING(REGEX REPLACE "/lib/lib([A-Za-z0-9]+).lib$" "/bin/\\1.dll" LIBNAME ${LIBNAME})
    ENDIF(${customlib} MATCHES "/lib/lib[A-Za-z0-9]+.lib$")

    message(STATUS "Generating Install Rule for DLL Library ${LIBNAME}")
    INSTALL(FILES ${LIBNAME} 
             DESTINATION "${destination}"
              CONFIGURATIONS Release
              COMPONENT Applications)
  endif()
endmacro()

macro (INSTALL_CUSTOM_FILES FILELIST destination)
  FILE(GLOB FILES ${FILELIST})
  FOREACH(instfile ${FILES})
    message(STATUS "Adding ${instfile} to ${destination}")
    INSTALL(FILES ${instfile}
            DESTINATION "${destination}"
            CONFIGURATIONS Release Debug
            COMPONENT Applications)
  ENDFOREACH(instfile)
endmacro()
