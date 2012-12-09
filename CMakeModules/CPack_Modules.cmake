
# Component support
SET(CPACK_COMPONENTS_ALL Applications)

# Display name
SET(CPACK_COMPONENT_APPLICATIONS_HIDDEN ON)
SET(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Applictaions")
SET(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C/C++ Development Files")

# Descriptions
SET(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
   "Support applictaions to test the capabilities of the software")
SET(CPACK_COMPONENT_LIBRARIES_DESCRIPTION
   "Dynamically shared components of the software")
SET(CPACK_COMPONENT_HEADERS_DESCRIPTION
   "Development header files and library components for use with the software")

# Dependecies
#SET(CPACK_COMPONENT_HEADERS_DEPENDS Libraries)
#SET(CPACK_COMPONENT_APPLICATIONS_DEPENDS Libraries)

# Component grouping
IF(WIN32)
  # NSIS
  SET(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
  SET(CPACK_COMPONENT_LIBRARIES_GROUP "Runtime")
  SET(CPACK_COMPONENT_HEADERS_GROUP "Development")

  # Note Windows DLL are specified by RUNTIME
  SET(CPACK_COMPONENT_GROUP_RUNTIME_DESCRIPTION
     "Software required to run the software")
  SET(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION
     "C/C++ Development headers and libraries")

  SET(CPACK_ALL_INSTALL_TYPES Runtime Developer)
  SET(CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES Runtime Developer)
  SET(CPACK_COMPONENT_HEADERS_INSTALL_TYPES Developer)
  SET(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Runtime)

ELSE(WIN32)
  SET(CPACK_COMPONENT_APPLICATIONS_GROUP "bin")
  SET(CPACK_COMPONENT_LIBRARIES_GROUP "bin")
  SET(CPACK_COMPONENT_HEADERS_GROUP "dev")

  SET(CPACK_COMPONENT_GROUP_BIN_DESCRIPTION
     "Software required to run the software")
  SET(CPACK_COMPONENT_GROUP_DEV_DESCRIPTION
     "C/C++ Development headers and libraries")

  SET(CPACK_ALL_INSTALL_TYPES bin dev)
  SET(CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES bin dev)
  SET(CPACK_COMPONENT_HEADERS_INSTALL_TYPES dev)
  SET(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES bin)
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
    IF(${customlib} MATCHES "/lib/")
      STRING(REGEX REPLACE "/lib/" "/bin/" LIBNAME ${LIBNAME})
    ENDIF(${customlib} MATCHES "/lib/")
    IF(${customlib} MATCHES ".lib$")
      STRING(REGEX REPLACE ".lib$" ".dll" LIBNAME ${LIBNAME})
    ENDIF(${customlib} MATCHES ".lib$")

    message(STATUS "Generating Install Rule for DLL Library ${LIBNAME}")
    INSTALL(FILES ${LIBNAME} 
             DESTINATION "${destination}"
              CONFIGURATIONS Release
              COMPONENT Applications)
  endif()
endmacro()
