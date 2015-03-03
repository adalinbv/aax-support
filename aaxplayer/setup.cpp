/*
 * Copyright 2014-2015 by Erik Hofman.
 * Copyright 2014-2015 by Adalin B.V.
 * All Rights Reserved.
 *
 * This file is part of AeonWave-AudioPlayer.
 *
 *  AeonWave-AudioPlayer is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AeonWave-AudioPlayer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AeonWave-AudioPlayer.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "aaxplayer.h"
#include "setup_ui.h"
#include "setup.h"

class QDialog;

#include <stdio.h>
Setup::Setup(QWidget *parent)
    : QDialog(parent)
{
    _cfg = new Ui_Configuration;
    _cfg->setupUi(this);

    QObject::connect(_cfg->OutputDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOutputDevice(int)));
    QObject::connect(_cfg->enableVUmeters, SIGNAL(clicked(bool)), this, SLOT(changeVUmeters(bool)));
    QObject::connect(_cfg->enableAGC, SIGNAL(clicked(bool)), this, SLOT(changeAGC(bool)));
    QObject::connect(_cfg->enableAutoPlay, SIGNAL(clicked(bool)), this, SLOT(changeAutoPlay(bool)));

    show();
}

Setup::~Setup()
{
    delete _cfg;
}

void
Setup::show()
{
    displayUiConfig();
    QDialog::show();
}

void
Setup::accept()
{
    int outdev_idx = _cfg->OutputDevice->currentIndex();
    _mw->odevices.current = _MINMAX(outdev_idx, 0, _mw->odevices.backend.size()-1);
    _cfg->OutputDevice->setCurrentIndex(_mw->odevices.current);

    if (_mw->odevices.current != -1)
    {
        int devpos = _mw->odevices.current;
        int ifpos = _cfg->OutputInterface->currentIndex();
        if ((ifpos == -1) && _cfg->OutputInterface->count() > 0) {
            ifpos =0;
        }

        _mw->odevices.backend[devpos].current = ifpos;
        QString str = _mw->odevices.backend[devpos].name;
        if (ifpos >= 0) {
            str += ": " +_mw->odevices.backend[devpos].interface_name[ifpos];
        }
        _mw->odevname_str = str;
        _mw->freeDevices();
    }

    displayUiConfig();
    QDialog::accept();
}

void
Setup::reject()
{
    displayUiConfig();
    QDialog::reject();
}

void
Setup::displayUiConfig()
{
    QString device;
    int pos;

    /** Output */
    _mw->getSystemResources(_mw->odevices, AAX_MODE_WRITE_STEREO);

    device = _mw->odevname_str;
    pos = device.indexOf(": ");
    if (pos > 0) {
        device = _mw->odevname_str.mid(0, pos);
    }

    if (_mw->odevices.init)
    {
        _cfg->OutputDevice->blockSignals(true);
        _cfg->OutputDevice->clear();
        for (int i=0; i<_mw->odevices.backend.size(); i++)
        {
            _cfg->OutputDevice->addItem(_mw->odevices.backend[i].name);
            if (_mw->odevices.backend[i].name == device) {
                _mw->odevices.current = i;
            }
        }
        _cfg->OutputDevice->blockSignals(false);
//      _mw->odevices.init = 0;
    }
    _cfg->OutputDevice->setCurrentIndex(_mw->odevices.current);

    displayUiDevicesConfig();

    _cfg->enableAGC->setChecked(_mw->agc_enabled);
    _cfg->enableAutoPlay->setChecked(_mw->autoplay);
    _cfg->enableVUmeters->setChecked(_mw->show_vu);

    _mw->resize();
}

void
Setup::displayUiDevicesConfig()
{
    struct backend_t *backend;
    if (_mw->odevices.current != -1)
    {
        backend = &_mw->odevices.backend[_mw->odevices.current];
        _cfg->OutputInterface->blockSignals(true);
        _cfg->OutputInterface->clear();
        _cfg->OutputInterface->insertItems(0, backend->interface_name);
        _cfg->OutputInterface->blockSignals(false);

        int pos = _mw->odevname_str.indexOf(": ");
        if (pos > 0)
        {
            QString interface_name = _mw->odevname_str.mid(pos+strlen(": "));
            backend->current = backend->interface_name.indexOf(interface_name);
        }
        else if (_cfg->OutputInterface->count() > 0) {
           backend->current = 0;
        }
        _cfg->OutputInterface->setCurrentIndex(backend->current);
    }
}

void
Setup::changeOutputDevice(int value)
{
    _mw->odevices.current = _MINMAX(value, 0, _mw->odevices.backend.size()-1);
    displayUiDevicesConfig();
}

void
Setup::changeAGC(bool val)
{
    _mw->agc_enabled = val;
}


void
Setup::changeAutoPlay(bool val)
{
    _mw->autoplay = val;
}

void
Setup::changeVUmeters(bool val)
{
    _mw->show_vu = val;
}
