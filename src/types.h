/*
 * Copyright (C) 2005-2014 by Erik Hofman.
 * Copyright (C) 2007-2014 by Adalin B.V.
 *
 * This file is part of OpenAL-AeonWave.
 *
 *  OpenAL-AeonWave is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  OpenAL-AeonWave is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with OpenAL-AeonWave.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __OAL_TYPES_H
#define __OAL_TYPES_H 1

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdlib.h>

#define _MAX(a,b)	(((a)>(b)) ? (a) : (b))
#define _MIN(a,b)	(((a)<(b)) ? (a) : (b))
#define _MINMAX(a,b,c)	(((a)>(c)) ? (c) : (((a)<(b)) ? (b) : (a)))

#if _MSC_VER
# define _WIN32_WINNT 0x0500
# include <Windows.h>
# include <stdio.h>
// # define strtoll _strtoi64
# define snprintf _snprintf
# define strcasecmp _stricmp
// # define strncasecmp _strnicmp
// # define rintf(v) (int)(v+0.5f)
#endif

#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif /* !__OAL_TYPES_H */

