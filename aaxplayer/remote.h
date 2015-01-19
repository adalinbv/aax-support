/*
 * Copyright 2011-2015 by Erik Hofman.
 * Copyright 2011-2015 by Adalin B.V.
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


#ifndef AAXRECORD_H
#define AAXRECORD_H

#include <QtGui/QDialog>

class Ui_Remote;

class Remote : public QDialog
{
    Q_OBJECT
public:
    Remote(QWidget *parent = 0);
    ~Remote();

    int exec();

private:
    Ui_Remote *_url;

private slots:
    void accept();
    void reject();
};

#endif // AAXRECORD_H

