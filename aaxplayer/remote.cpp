/*
 * Copyright 2014-2015 by Erik Hofman.
 * Copyright 2014-2015 by Adalin B.V.
 * All Rights Reserved.
 *
 * This file is part of AeonWave-AudioRemoteer.
 *
 *  AeonWave-AudioRemoteer is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AeonWave-AudioRemoteer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AeonWave-AudioRemoteer.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "aaxplayer.h"
#include "remote_ui.h"
#include "remote.h"

class QDialog;

Remote::Remote(QWidget *parent)
    : QDialog(parent)
{
    _url = new Ui_Remote;
    _url->setupUi(this);

    connect(_url->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(_url->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

Remote::~Remote()
{
    delete _url;
}

int
Remote::exec()
{
    return QDialog::exec();
}

void
Remote::accept()
{
    _mw->infile = _url->remoteURL->text();
    QDialog::accept();
}

void
Remote::reject()
{
    QDialog::reject();
}
