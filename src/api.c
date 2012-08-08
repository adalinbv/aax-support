/*
 * Copyright 2011-2012 by Adalin B.V.
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

#define CONFIG_FILE             "config.xml"

#if defined(WIN32)
# define SYSTEM_DIR		getenv("PROGRAMFILES")
# define AAX_DIR		"\\aax\\"

# define LOCALAPP_DIR		getenv("LOCALAPPDATA")
# define USER_AAX_DIR		"\\adalin\\aax\\"

# define USER_DIR		getenv("USERPROFILE")

#else	/* !WIN32 */
# define SYSTEM_DIR		"/etc"
# define AAX_DIR		"/aax/"

# define LOCALAPP_DIR		getenv("HOME")
# define USER_AAX_DIR		"/.aax/"

# define USER_DIR		getenv("HOME")
#endif


#ifndef WIN32
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdio.h>

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
            mkdir(nfile, mode);       /* the new directory does not yet exist */
         }

         snprintf(nfile, 256, "%s%s%s", path, USER_AAX_DIR, CONFIG_FILE);
         if (stat(nfile, &sb)) {
            rename(ofile, nfile);   /* the new config file does not yet exist */
         }
      }
   }
}
#else
# define moveOldCfgFile()
#endif

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
      int len;

      len = strlen(app_path);
      len += strlen(USER_AAX_DIR);
      len += strlen(CONFIG_FILE);
      len++;

      rv = malloc(len);
      if (rv) {
         snprintf(rv, len, "%s%s%s", app_path, USER_AAX_DIR, CONFIG_FILE);
      }
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
      int len;

      len = strlen(global_path);
      len += strlen(AAX_DIR);
      len += strlen(CONFIG_FILE);
      len++;

      rv = malloc(len);
      if (rv) {
         snprintf(rv, len, "%s%s%s", global_path, AAX_DIR, CONFIG_FILE);
      }
   }

   return rv;
}


/* ------------------------------------------------------------------------- */

#include <aax.h>
#include "api.h"

speaker_t speaker_setup[MAX_SPEAKER_SETUP] =
{
  {
    2, "stereo", ":/stereo.png", AAX_MODE_WRITE_SURROUND,
    {
      { 0, {-1.0f, 0.0f, 0.0f} },    { 1, { 1.0f, 0.0f, 0.0f} },
      { 2, {-1.0f, 0.0f, 0.0f} },    { 3, { 1.0f, 0.0f, 0.0f} },
      { 4, {-1.0f, 0.0f, 0.0f} },    { 5, { 1.0f, 0.0f, 0.0f} },
      { 6, {-1.0f, 0.0f, 0.0f} },    { 7, { 1.0f, 0.0f, 0.0f} }
    }
  },
  {
    4, "quad", ":/quad.png", AAX_MODE_WRITE_SURROUND,
    {
      { 0, {-1.0f, 0.0f, 1.0f} },    { 1, { 1.0f, 0.0f, 1.0f} },
      { 2, {-1.0f, 0.0f,-1.0f} },    { 3, { 1.0f, 0.0f,-1.0f} },
      { 4, { 0.0f, 0.0f, 0.0f} },    { 5, { 0.0f, 0.0f, 0.0f} },
      { 6, { 0.0f, 0.0f, 0.0f} },    { 7, { 0.0f, 0.0f, 0.0f} }
    }
  },
  {
    6, "surround 5.1", ":/surround5_1.png", AAX_MODE_WRITE_SURROUND,
    {
      { 0, {-1.0f, 0.0f, 1.0f} },    { 1, { 1.0f, 0.0f, 1.0f} },
      { 2, {-1.0f, 0.0f,-1.0f} },    { 3, { 1.0f, 0.0f,-1.0f} },
      { 4, { 0.0f, 0.0f, 1.0f} },    { 5, { 0.0f, 0.0f, 1.0f} },
      { 6, { 0.0f, 0.0f, 0.0f} },    { 7, { 0.0f, 0.0f, 0.0f} }
    }
  },
  {
    8, "surround 7.1", ":/surround7_1.png", AAX_MODE_WRITE_SURROUND,
    {
      { 0, {-1.0f, 0.0f, 1.0f} },    { 1, { 1.0f, 0.0f, 1.0f} },
      { 2, {-1.0f, 0.0f,-1.0f} },    { 3, { 1.0f, 0.0f,-1.0f} },
      { 4, { 0.0f, 0.0f, 1.0f} },    { 5, { 0.0f, 0.0f, 1.0f} },
      { 6, {-1.0f, 0.0f, 0.0f} },    { 7, { 1.0f, 0.0f, 0.0f} }
    }
  },
  {
    2, "stereo", ":/stereo.png", AAX_MODE_WRITE_SPATIAL,
    {
      { 0, {-1.0f, 0.0f, 0.0f} },    { 1, { 1.0f, 0.0f, 0.0f} },
      { 2, {-1.0f, 0.0f, 0.0f} },    { 3, { 1.0f, 0.0f, 0.0f} },
      { 4, {-1.0f, 0.0f, 0.0f} },    { 5, { 1.0f, 0.0f, 0.0f} },
      { 6, {-1.0f, 0.0f, 0.0f} },    { 7, { 1.0f, 0.0f, 0.0f} }
    }
  },
  {
    4, "quad", ":/quad.png", AAX_MODE_WRITE_SPATIAL,
    {
      { 0, {-1.0f, 0.0f, 1.0f} },    { 1, { 1.0f, 0.0f, 1.0f} },
      { 2, {-1.0f, 0.0f,-1.0f} },    { 3, { 1.0f, 0.0f,-1.0f} },
      { 4, { 0.0f, 0.0f, 0.0f} },    { 5, { 0.0f, 0.0f, 0.0f} },
      { 6, { 0.0f, 0.0f, 0.0f} },    { 7, { 0.0f, 0.0f, 0.0f} }
    }
  },
  {
    6, "spatial 2.4", ":/surround2_4.png", AAX_MODE_WRITE_SPATIAL,
    {
      { 0, {-1.0f, 0.0f, 1.0f} },    { 1, { 1.0f, 0.0f, 1.0f} },
      { 2, {-1.0f, 1.0f, 1.0f} },    { 3, { 1.0f, 1.0f, 1.0f} },
      { 4, {-1.0f, 1.0f,-1.0f} },    { 5, { 1.0f, 1.0f,-1.0f} },
      { 6, { 0.0f, 0.0f, 0.0f} },    { 7, { 0.0f, 0.0f, 0.0f} }
    }
  },
#if 0
  {
    6, "spatial 6", ":/surround6.png", AAX_MODE_WRITE_SPATIAL,
    {
      { 0, {-1.0f, 0.0f, 1.0f} },    { 1, { 1.0f, 0.0f, 1.0f} },
      { 2, {-1.0f, 0.0f,-1.0f} },    { 3, { 1.0f, 0.0f,-1.0f} },
      { 4, {-1.0f, 0.0f, 0.0f} },    { 5, { 1.0f, 0.0f, 0.0f} },
      { 6, { 0.0f, 0.0f, 0.0f} },    { 7, { 0.0f, 0.0f, 0.0f} }
   }
  },
#endif
  {
    8, "spatial 8", ":/surround8.png", AAX_MODE_WRITE_SPATIAL,
    {
      { 0, {-1.0f, 0.0f, 1.0f} },    { 1, { 1.0f, 0.0f, 1.0f} },
      { 2, {-1.0f, 0.0f,-1.0f} },    { 3, { 1.0f, 0.0f,-1.0f} },
      { 4, {-1.0f, 1.0f, 1.0f} },    { 5, { 1.0f, 1.0f, 1.0f} },
      { 6, {-1.0f, 1.0f,-1.0f} },    { 7, { 1.0f, 1.0f,-1.0f} }
    }
  },
  {
    8, "user defined", ":/surround8.png", AAX_MODE_WRITE_SPATIAL,
    {
      { 0, {-1.0f, 0.0f, 1.0f} },    { 1, { 1.0f, 0.0f, 1.0f} },
      { 2, {-1.0f, 0.0f,-1.0f} },    { 3, { 1.0f, 0.0f,-1.0f} },
      { 4, { 0.0f, 0.0f, 1.0f} },    { 5, { 0.0f, 0.0f, 1.0f} },
      { 6, {-1.0f, 0.0f, 0.0f} },    { 7, { 1.0f, 0.0f, 0.0f} }
    }
  }
};


