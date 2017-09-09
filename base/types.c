/*
 * Copyright (C) 2005-2015 by Erik Hofman.
 * Copyright (C) 2007-2015 by Adalin B.V.
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

#include <stdio.h>
#include <stdarg.h>

#include  "types.h"

#ifdef WIN32
int _aax_snprintf(char *str, size_t size, const char *fmt, ...)
{
   int ret;
   va_list ap;

   va_start(ap,fmt);
   ret = vsnprintf(str,size,fmt,ap);
   // Whatever happen in vsnprintf, what i'll do is just to null terminate it
   str[size-1] = '\0';
   va_end(ap);
   return ret;
}
#endif  /* if defined(WIN32) */
