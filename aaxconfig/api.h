/*
 * Copyright 2011-2016 by Erik Hofman
 * Copyright 2011-2016 by Adalin B.V.
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

#ifndef __API_H
#define __API_H 1

#if defined(__cplusplus)
extern "C" {
#endif

enum
{
    STEREO = 0,
    QUAD,
    SURROUND51,
    SURROUND71,
    SPATIAL2,
    SPATIAL4,
    SPATIAL24,
//  SPATIAL6,
    SPATIAL8,
    USER_DEFINED,
    MAX_SPEAKER_SETUP
};

typedef struct
{
    unsigned no_speakers;
    char *name;
    char *pixmap;
    unsigned int setup;
    struct
    {
        int channel;
        float pos[3];
    } speaker[8];
} speaker_t;

extern speaker_t speaker_setup[MAX_SPEAKER_SETUP];

void cfgBackendDriverToDeviceConnector(char **, char **);
void cfgDeviceConnectorToBackendDriver(char **, char **);

#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif

