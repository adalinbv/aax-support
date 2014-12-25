/*
 * Copyright 2011-2014 by Erik Hofman.
 * Copyright 2011-2014 by Adalin B.V.
 * All Rights Reserved.
 *
 * This file is part of AeonWave-AudioPlayer.
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


#ifndef AAXCONFIG_H
#define AAXCONFIG_H

#include <QtGui/QDialog>
#include <QStringList>
#include <QString>

#include <aax/aax.h>

class Ui_Configuration;

struct backend_t
{
   QString name;
   QStringList interface_name;
   int current;

   backend_t() {
      current = -1;
   }
   ~backend_t() {
      interface_name.clear();
   }
};

struct device_t
{
    int init, current;
    QList<struct backend_t> backend;

    device_t() {
        current = -1, init = -1;
    }
    ~device_t () {
        backend.clear();
    }
};

class Setup : public QDialog
{
    Q_OBJECT
public:
    Setup(QWidget *parent = 0);
    ~Setup();

    void show();

private:
    int m_value;
    Ui_Configuration *_cfg;

    void displayUiConfig();
    void displayUiDevicesConfig();

private slots:
    void accept();
    void reject();

    void changeOutputDevice(int val);
};

#endif // AAXCONFIG_H

