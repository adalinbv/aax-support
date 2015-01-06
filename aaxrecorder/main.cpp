/*
 * Copyright (C) 2014-2015 by Erik Hofman
 * Copyright (C) 2014-2015 by Adalin B.V.
 *
 * This file is part of AeonWave-AudioRecorder.
 *
 *  AeonWave-AudioRecorder is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AeonWave-AudioRecorder is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AeonWave-AudioRecorder.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGui/QApplication>
#include "aaxrecorder.h"

#if defined(WIN32)
# ifndef NDEBUG
#  pragma comment(linker, "/SUBSYSTEM:CONSOLE")
# else
#  pragma comment(linker, "/SUBSYSTEM:WINDOWS /NODEFAULTLIB:msvcrt.lib")
#  define WIN32_LEAN_AND_MEAN
# endif
#endif

AeonWaveRecorder *_mw = 0;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    int rv = -1;

    _mw = new AeonWaveRecorder;
    if (_mw)
    {
        _mw->show();
        rv = app.exec();
        delete _mw;
    }
    return rv;
}
