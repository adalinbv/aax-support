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
#include <assert.h>
#include <stdio.h>

#include "types.h"

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
