/*
 * Copyright 2011-2015 by Erik Hofman
 * Copyright 2011-2015 by Adalin B.V.
 *
 * This file is part of AeonWave-Player.
 *
 *  AeonWave-Player is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AeonWave-Player is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AeonWave-Player.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __BASE_API_H
#define __BASE_API_H 1

#if defined(__cplusplus)
extern "C" {
#endif


const char* tmpDir();
const char* userHomeDir();
char* userConfigFile(const char*);
char* systemConfigFile(const char*);

#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif

