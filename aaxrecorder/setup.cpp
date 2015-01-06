/*
 * Copyright 2014-2015 by Erik Hofman.
 * Copyright 2014-2015 by Adalin B.V.
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
#include "setup_ui.h"
#include "setup.h"

class QDialog;

#include <stdio.h>
Setup::Setup(QWidget *parent)
    : QDialog(parent)
{
    _cfg = new Ui_Configuration;
    _cfg->setupUi(this);

    QObject::connect(_cfg->InputDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(changeInputDevice(int)));
    QObject::connect(_cfg->OutputDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOutputDevice(int)));

    QObject::connect(_cfg->enableAGC, SIGNAL(clicked(bool)), this, SLOT(changeAGC(bool)));

    show();
}

Setup::~Setup()
{
    delete _cfg;
}

void
Setup::show()
{
    _mw->freeDevices();
    displayUiConfig();
    QDialog::show();
}

void
Setup::accept()
{
    int outdev_idx = _cfg->OutputDevice->currentIndex();
    _mw->odevices.current = _MINMAX(outdev_idx, 0, _mw->odevices.backend.size()-1);
    _cfg->OutputDevice->setCurrentIndex(_mw->odevices.current);

    int indev_idx = _cfg->InputDevice->currentIndex();
    _mw->idevices.current = _MINMAX(indev_idx, 0, _mw->idevices.backend.size()-1);
    _cfg->InputDevice->setCurrentIndex(_mw->idevices.current);

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
    }

    if (_mw->idevices.current != -1)
    {
        int devpos = _mw->idevices.current;
        int ifpos = _cfg->InputInterface->currentIndex();
        if ((ifpos == -1) && _cfg->InputInterface->count() > 0) {
            ifpos =0;
        }

        _mw->idevices.backend[devpos].current = ifpos;
        QString str = _mw->idevices.backend[devpos].name;
        if (ifpos >= 0) {
            str += ": " + _mw->idevices.backend[devpos].interface_name[ifpos];
        }
        _mw->idevname_str = str;
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

    /** Input */
    _mw->getSystemResources(_mw->idevices, AAX_MODE_READ);

    device = _mw->idevname_str;
    pos = device.indexOf(": ");
    if (pos > 0) {
        device = _mw->idevname_str.mid(0, pos);
    }

    if (_mw->idevices.init)
    {
        _cfg->InputDevice->blockSignals(true);
        _cfg->InputDevice->clear();
        for (int i=0; i<_mw->idevices.backend.size(); i++)
        {
            _cfg->InputDevice->addItem(_mw->idevices.backend[i].name);
            if (_mw->idevices.backend[i].name == device) {
                _mw->idevices.current = i;
            }
        }
        _cfg->InputDevice->blockSignals(false);
//      _mw->odevices.init = 0;
    }
    _cfg->InputDevice->setCurrentIndex(_mw->idevices.current);

    displayUiDevicesConfig();
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

    if (_mw->idevices.current != -1)
    {
        backend = &_mw->idevices.backend[_mw->idevices.current];
        _cfg->InputInterface->blockSignals(true);
        _cfg->InputInterface->clear();
        _cfg->InputInterface->insertItems(0, backend->interface_name);
        _cfg->InputInterface->blockSignals(false);

        int pos = _mw->idevname_str.indexOf(": ");
        if (pos > 0)
        {
            QString interface_name = _mw->idevname_str.mid(pos+strlen(": "));
            backend->current = backend->interface_name.indexOf(interface_name);
        }
        else if (_cfg->InputInterface->count() > 0) {
           backend->current = 0;
        }
        _cfg->InputInterface->setCurrentIndex(backend->current);
    }
}

void
Setup::changeInputDevice(int value)
{
    _mw->idevices.current = _MINMAX(value, 0, _mw->idevices.backend.size()-1);
    displayUiDevicesConfig();
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
    _mw->agc_enabled = _cfg->enableAGC->isChecked();
}

