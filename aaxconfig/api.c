/*
 * Copyright 2011-2015 by Erik Hofman
 * Copyright 2011-2015 by Adalin B.V.
 *
 * This file is part of AeonWave-Config.
 *
 *  AeonWave-Config is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AeonWave-Config is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AeonWave-Config.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdlib.h>		/* for getenv */
#include <assert.h>

#include "types.h"

#define CONFIG_FILE                "config.xml"

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


#ifndef WIN32
static void
moveOldCfgFile()
{
    char *path = USER_DIR;
    if (path)
    {
        char ofile[256];
        struct stat sb;

        snprintf(ofile, 256, "%s/.aaxconfig.xml", path);
        if (!stat(ofile, &sb) && (sb.st_size > 0))
        {
            char nfile[256];

            snprintf(nfile, 256, "%s%s", path, USER_AAX_DIR);
            if (stat(nfile, &sb))
            {
                int mode = strtol("0700", 0, 8);
                mkdir(nfile, mode);        /* the new directory does not yet exist */
            }

            snprintf(nfile, 256, "%s%s%s", path, USER_AAX_DIR, CONFIG_FILE);
            if (stat(nfile, &sb)) {
                rename(ofile, nfile);    /* the new config file does not yet exist */
            }
        }
    }
}
#else
# define moveOldCfgFile()
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
userConfigFile()
{
    const char *app_path = LOCALAPP_DIR;
    char *rv = NULL;

    moveOldCfgFile();

    if (app_path)
    {
        char directory[1024];
        int len;

        len = strlen(app_path);
        len += strlen(USER_AAX_DIR);
        len += strlen(CONFIG_FILE);
        len++;

        rv = malloc(len);
        if (rv) {
            snprintf(rv, len, "%s%s%s", app_path, USER_AAX_DIR, CONFIG_FILE);
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
systemConfigFile()
{
    char *global_path = SYSTEM_DIR;
    char *rv = NULL;

    if (global_path)
    {
        char directory[1024];
        int len;

        len = strlen(global_path);
        len += strlen(AAX_DIR);
        len += strlen(CONFIG_FILE);
        len++;

        rv = malloc(len);
        if (rv) {
            snprintf(rv, len, "%s%s%s", global_path, AAX_DIR, CONFIG_FILE);
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

/*
 * Input: Backend (ALSA/WASAP/OSS) and driver (<card>: <interface>)
 * Output: Device (<backend> on <card>) and interface
 */
void
cfgBackendDriverToDeviceConnector(char **backend, char **driver)
{
   char _str[2048], *ptr;
   size_t len;

   assert(backend && *backend);
   assert(driver && *driver);

   snprintf(_str, 2048, "%s on %s", *backend, *driver);
   ptr = strstr(_str, ": ");
   if (!ptr) return;

   len = ptr - _str;
   if (len > strlen(*backend))
   {
      char *tmp = realloc(*backend, len+1);
      if (!tmp) return;

      *backend = tmp;
   }

   *ptr++ = 0;
   memcpy(*backend, _str, ptr - _str);
   memcpy(*driver, (ptr+1), strlen(ptr));
}
/*
 * Input: Device (<backend> on <card>) and interface
 * Output: Backend (ALSA/WASAP/OSS) and driver (<card>: <interface>)
 */
void
cfgDeviceConnectorToBackendDriver(char **device, char **iface)
{
   char _str[2048], *ptr;
   size_t len;

   assert(device && *device);
   assert(iface && *iface);

   snprintf(_str, 2048, "%s: %s", *device, *iface);
   ptr = strstr(_str, " on ");

   len = strlen(ptr)-strlen(" on ");
   if (len > strlen(*iface))
   {
      char *tmp = realloc(*iface, len+1);
      if (!tmp) return;

      *iface = tmp;
   }

   *ptr = 0;
   memcpy(*device, _str, ptr - _str);

   ptr += strlen(" on ");
   memcpy(*iface, ptr, strlen(ptr));
}


/* ------------------------------------------------------------------------- */

#include <aax/aax.h>
#include "api.h"

speaker_t speaker_setup[MAX_SPEAKER_SETUP] =
{
    {
      2, "stereo", ":/stereo.png", AAX_MODE_WRITE_SURROUND,
      {
        { 0, { 1.0f, 0.0f, 0.0f} },     { 1, {-1.0f, 0.0f, 0.0f} },
        { 2, { 1.0f, 0.0f, 0.0f} },     { 3, {-1.0f, 0.0f, 0.0f} },
        { 4, { 1.0f, 0.0f, 0.0f} },     { 5, {-1.0f, 0.0f, 0.0f} },
        { 6, { 1.0f, 0.0f, 0.0f} },     { 7, {-1.0f, 0.0f, 0.0f} }
      }
    },
    {
      4, "quad", ":/quad.png", AAX_MODE_WRITE_SURROUND,
      {
        { 0, { 1.0f, 0.0f, 1.0f} },     { 1, {-1.0f, 0.0f, 1.0f} },
        { 2, { 1.0f, 0.0f,-1.0f} },     { 3, {-1.0f, 0.0f,-1.0f} },
        { 4, { 0.0f, 0.0f, 0.0f} },     { 5, { 0.0f, 0.0f, 0.0f} },
        { 6, { 0.0f, 0.0f, 0.0f} },     { 7, { 0.0f, 0.0f, 0.0f} }
      }
    },
    {
      6, "surround 5.1", ":/surround5_1.png", AAX_MODE_WRITE_SURROUND,
      {
        { 0, { 1.0f, 0.0f, 1.0f} },     { 1, {-1.0f, 0.0f, 1.0f} },
        { 2, { 1.0f, 0.0f,-1.0f} },     { 3, {-1.0f, 0.0f,-1.0f} },
        { 4, { 0.0f, 0.0f, 0.7f} },     { 5, { 0.0f, 0.0f, 0.7f} },
        { 6, { 0.0f, 0.0f, 0.0f} },     { 7, { 0.0f, 0.0f, 0.0f} }
      }
    },
    {
      8, "surround 7.1", ":/surround7_1.png", AAX_MODE_WRITE_SURROUND,
      {
        { 0, { 1.0f, 0.0f, 1.0f} },     { 1, {-1.0f, 0.0f, 1.0f} },
        { 2, { 1.0f, 0.0f,-1.0f} },     { 3, {-1.0f, 0.0f,-1.0f} },
        { 4, { 0.0f, 0.0f, 0.7f} },     { 5, { 0.0f, 0.0f, 0.7f} },
        { 6, { 0.7f, 0.0f, 0.0f} },     { 7, {-0.7f, 0.0f, 0.0f} }
      }
    },
    {
      2, "stereo", ":/stereo.png", AAX_MODE_WRITE_SPATIAL,
      {
        { 0, { 1.0f, 0.0f, 0.0f} },     { 1, {-1.0f, 0.0f, 0.0f} },
        { 2, { 1.0f, 0.0f, 0.0f} },     { 3, {-1.0f, 0.0f, 0.0f} },
        { 4, { 1.0f, 0.0f, 0.0f} },     { 5, {-1.0f, 0.0f, 0.0f} },
        { 6, { 1.0f, 0.0f, 0.0f} },     { 7, {-1.0f, 0.0f, 0.0f} }
      }
    },
    {
      4, "quad", ":/quad.png", AAX_MODE_WRITE_SPATIAL,
      {
        { 0, { 1.0f, 0.0f, 1.0f} },     { 1, {-1.0f, 0.0f, 1.0f} },
        { 2, { 1.0f, 0.0f,-1.0f} },     { 3, {-1.0f, 0.0f,-1.0f} },
        { 4, { 0.0f, 0.0f, 0.0f} },     { 5, { 0.0f, 0.0f, 0.0f} },
        { 6, { 0.0f, 0.0f, 0.0f} },     { 7, { 0.0f, 0.0f, 0.0f} }
      }
    },
    {
      6, "spatial 2.4", ":/surround2_4.png", AAX_MODE_WRITE_SPATIAL,
      {
        { 0, { 1.0f, 0.0f, 1.0f} },     { 1, {-1.0f, 0.0f, 1.0f} },
        { 2, { 1.0f, 1.0f, 1.0f} },     { 3, {-1.0f, 1.0f, 1.0f} },
        { 4, { 1.0f, 1.0f,-1.0f} },     { 5, {-1.0f, 1.0f,-1.0f} },
        { 6, { 0.0f, 0.0f, 0.0f} },     { 7, { 0.0f, 0.0f, 0.0f} }
      }
    },
#if 0
    {
      6, "spatial 6", ":/surround6.png", AAX_MODE_WRITE_SPATIAL,
      {
        { 0, { 1.0f, 0.0f, 1.0f} },     { 1, {-1.0f, 0.0f, 1.0f} },
        { 2, { 1.0f, 0.0f,-1.0f} },     { 3, {-1.0f, 0.0f,-1.0f} },
        { 4, { 0.7f, 0.0f, 0.0f} },     { 5, {-0.7f, 0.0f, 0.0f} },
        { 6, { 0.0f, 0.0f, 0.0f} },     { 7, { 0.0f, 0.0f, 0.0f} }
      }
    },
#endif
    {
      8, "spatial 8", ":/surround8.png", AAX_MODE_WRITE_SPATIAL,
      {
        { 0, { 1.0f, 0.0f, 1.0f} },     { 1, {-1.0f, 0.0f, 1.0f} },
        { 2, { 1.0f, 0.0f,-1.0f} },     { 3, {-1.0f, 0.0f,-1.0f} },
        { 4, { 1.0f, 1.0f, 1.0f} },     { 5, {-1.0f, 1.0f, 1.0f} },
        { 6, { 1.0f, 1.0f,-1.0f} },     { 7, {-1.0f, 1.0f,-1.0f} }
      }
    },
    {
      8, "user defined", ":/surround8.png", AAX_MODE_WRITE_SPATIAL,
      {
        { 0, { 1.0f, 0.0f, 1.0f} },     { 1, {-1.0f, 0.0f, 1.0f} },
        { 2, { 1.0f, 0.0f,-1.0f} },     { 3, {-1.0f, 0.0f,-1.0f} },
        { 4, { 0.0f, 0.0f, 1.0f} },     { 5, { 0.0f, 0.0f, 1.0f} },
        { 6, { 1.0f, 0.0f, 0.0f} },     { 7, {-1.0f, 0.0f, 0.0f} }
      }
    }
};

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
