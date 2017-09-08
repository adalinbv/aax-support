/*
 * Copyright 2011-2015 by Erik Hofman
 * Copyright 2011-2015 by Adalin B.V.
 *
 * This file is part of AeonWave-Support.
 *
 *  AeonWave-Support is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AeonWave-Support is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AeonWave-Support.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdlib.h>		/* for getenv */

#include <base/types.h>

#if defined(WIN32)
# define _WIN32_WINNT 0x0500
# include <Windows.h>
# include <sddl.h>
# include <stdio.h>

#ifdef __MINGW32_VERSION
/* mingw-w32api v3.1 does not yet include sddl.h, so define needed parts here
 */
BOOL WINAPI ConvertStringSecurityDescriptorToSecurityDescriptorA(LPCSTR, DWORD, PSECURITY_DESCRIPTOR *, PULONG);
# define ConvertStringSecurityDescriptorToSecurityDescriptor \
         ConvertStringSecurityDescriptorToSecurityDescriptorA
# define SDDL_REVISION_1	1
#endif

# define SYSTEM_DIR		getenv("PROGRAMFILES")
# define AAX_DIR		"\\AAX\\"

# define LOCALAPP_DIR		getenv("LOCALAPPDATA")
# define VENDOR_DIR		"\\Adalin\\"
# define USER_AAX_DIR		"\\Adalin\\AAX\\"

# define USER_DIR		getenv("USERPROFILE")
# define TEMP_DIR		getenv("TEMP")

static int createDACL(SECURITY_ATTRIBUTES*);

#else	/* !WIN32 */
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdio.h>

# define SYSTEM_DIR		"/etc"
# define AAX_DIR		"/aax/"

# define LOCALAPP_DIR		getenv("HOME")
# define USER_AAX_DIR		"/.aax/"

# define USER_DIR		getenv("HOME")
# define TEMP_DIR		"/tmp"
#endif


const char*
tmpDir()
{
   return TEMP_DIR;
}

const char*
userHomeDir()
{
    return USER_DIR;
}

char*
userConfigFile(const char *config_file)
{
    const char *app_path = LOCALAPP_DIR;
    char *rv = NULL;

    if (app_path)
    {
        char directory[1024];
        size_t len;

        len = strlen(app_path);
        len += strlen(USER_AAX_DIR);
        len += strlen(config_file);
        len++;

        rv = malloc(len);
        if (rv) {
            snprintf(rv, len, "%s%s%s", app_path, USER_AAX_DIR, config_file);
        }

        /* make the directories if they do no exist */
#ifndef WIN32
        do
        {
            int mode = strtol("0700", 0, 8);
            snprintf(directory, 1024, "%s%s", LOCALAPP_DIR, USER_AAX_DIR);
            mkdir(directory, mode);
        }
        while(0);
#else
        /*
         * Note:
         * There is a default string size limit for paths of 248 characters.
         * This limit is related to how the CreateDirectory function parses
         * paths.
         */
        do
        {
            SECURITY_ATTRIBUTES sa;

            sa.nLength = sizeof(SECURITY_ATTRIBUTES);
            sa.bInheritHandle = FALSE; 
            if(createDACL(&sa))
            {
                snprintf(directory, 248, "%s%s", LOCALAPP_DIR, VENDOR_DIR);
                CreateDirectory(directory, &sa);

                snprintf(directory, 248, "%s%s", LOCALAPP_DIR, USER_AAX_DIR);
                CreateDirectory(directory, &sa);

                LocalFree(sa.lpSecurityDescriptor);
            }
        }
        while(0);
#endif
    }

    return rv;
}

char*
systemConfigFile(const char *config_file)
{
    char *global_path = SYSTEM_DIR;
    char *rv = NULL;

    if (global_path)
    {
        char directory[1024];
        size_t len;

        len = strlen(global_path);
        len += strlen(AAX_DIR);
        len += strlen(config_file);
        len++;

        rv = malloc(len);
        if (rv) {
            snprintf(rv, len, "%s%s%s", global_path, AAX_DIR, config_file);
        }

        /* make the directories if they do no exist */
#ifndef WIN32
        do
        {
            int mode = strtol("0700", 0, 8);
            snprintf(directory, 1024, "%s%s", global_path, AAX_DIR);
            mkdir(directory, mode);
        }
        while(0);
#else
        do
        {
            SECURITY_ATTRIBUTES sa;

            sa.nLength = sizeof(SECURITY_ATTRIBUTES);
            sa.bInheritHandle = FALSE;
            if(createDACL(&sa))
            {
                snprintf(directory, 248, "%s%s", global_path, AAX_DIR);
                CreateDirectory(directory, &sa);
                LocalFree(sa.lpSecurityDescriptor);
            }
        }
        while(0);
#endif
    }

    return rv;
}

#if defined(WIN32)
int
createDACL(SECURITY_ATTRIBUTES * pSA)
{
    PULONG nSize = 0;
    TCHAR *szSD = TEXT("D:")
       TEXT("(D;OICI;GA;;;BG)") /* Allow all to built-in Administrators group */
       TEXT("(D;OICI;GRGX;;;AN)") /* Allow all to Authenticated users         */
       TEXT("(A;OICI;GA;;;AU)")   /* Allow read/execute to anonymous logon    */
       TEXT("(A;OICI;GA;;;BA)");  /* Deny all for built-in guests             */

    if (pSA == NULL) {
        return -1;
    }
 
    return ConvertStringSecurityDescriptorToSecurityDescriptor(
                            szSD, SDDL_REVISION_1,
                            (PSECURITY_DESCRIPTOR*)&(pSA->lpSecurityDescriptor),
                            nSize);
}
#endif
