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
#include <stdlib.h>             /* for getenv */

#define CONFIG_FILE	"bookmarks.xml"

#if defined(WIN32)
# define SYSTEM_DIR             getenv("PROGRAMFILES")
# define AAX_DIR                "\\AAX\\"

# define LOCALAPP_DIR           getenv("LOCALAPPDATA")
# define VENDOR_DIR             "\\Adalin\\"
# define USER_AAX_DIR           "\\Adalin\\AAX\\"

# define USER_DIR               getenv("USERPROFILE")
# define TEMP_DIR               getenv("TEMP")

#else   /* !WIN32 */
# include <sys/stat.h>
# include <stdio.h>

# define SYSTEM_DIR             "/etc"
# define AAX_DIR                "/aax/"

# define LOCALAPP_DIR           getenv("HOME")
# define USER_AAX_DIR           "/.aax/"

# define USER_DIR               getenv("HOME")
# define TEMP_DIR               "/tmp"
#endif

char*
userConfigFile()
{
    const char *app_path = LOCALAPP_DIR;
    char *rv = NULL;

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

