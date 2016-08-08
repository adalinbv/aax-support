/*
 * Copyright (C) 2014-2016 by Adalin B.V.
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

#ifndef AAXCONFIG_WIDGETS_H
#define AAXCONFIG_WIDGETS_H 1

#include <QLineEdit>
#include <QEvent>

#include "aaxconfig.h"

class ConfigLineEdit : public QLineEdit
{

    Q_OBJECT

public:
    ConfigLineEdit(QWidget* parent);
    ~ConfigLineEdit() {};

private:
    bool eventFilter(QObject* object, QEvent* event);
};

#endif
