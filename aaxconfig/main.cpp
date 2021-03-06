/*
 * Copyright (C) 2011-2016 by Erik Hofman
 * Copyright (C) 2011-2016 by Adalin B.V.
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

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <QtWidgets/QApplication>
#include "aaxconfig.h"

AeonWaveConfig *_mw = 0;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    int rv = -1;

    _mw = new AeonWaveConfig;
    if (_mw)
    {
        _mw->show();
        rv = app.exec();
        delete _mw;
    }
    return rv;
}
