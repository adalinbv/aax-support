/*
 * Copyright 2014-2016 by Erik Hofman.
 * Copyright 2014-2016 by Adalin B.V.
 * All Rights Reserved.
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

#include "aaxrecorder.h"
#include "ui_record.h"
#include "record.h"

class QDialog;

Record::Record(QWidget *parent)
    : QDialog(parent)
{
    _rec = new Ui_Recording;
    _rec->setupUi(this);

    connect(_rec->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(_rec->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

Record::~Record()
{
    delete _rec;
}

int
Record::exec()
{
    return QDialog::exec();
}

void
Record::accept()
{
    _mw->time = _rec->timeEdit->time();
    QDialog::accept();
}

void
Record::reject()
{
    QDialog::reject();
}
