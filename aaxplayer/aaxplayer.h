/*
 * Copyright (C) 2014 by Adalin B.V.
 *
 * This file is part of AeonWave-AudioPlayer
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

#ifndef AAXPLAYER_H
#define AAXPLAYER_H

#include <QtGui/QDialog>
#include <QTimer>

#include <aax/aax.h>

#include "setup.h"

#define _MINMAX(a,b,c)  (((a)>(c)) ? (c) : (((a)<(b)) ? (b) : (a)))

class Ui_AudioPlayer;

class AeonWavePlayer : public QDialog
{
    Q_OBJECT

public:
    AeonWavePlayer(QWidget *parent = 0);
    ~AeonWavePlayer();

    Ui_AudioPlayer *ui;
    Setup *setup;

    /* device list */
    QString odevname_str;
    struct device_t odevices;

    aaxConfig outdev;
    aaxConfig indev;
    bool agc_enabled;
    bool playing;
    bool paused;

    aaxConfig file;
    int bitrate;
    bool recording;
    float in_freq;

    void openOutputDevice();
    void getSystemResources(device_t&, enum aaxRenderMode);
    void freeDevices();

private:
    QTimer timer;
    QString infile;
    QString outfiles_path;
    QString infiles_path;
    size_t max_samples;

    void alert(QString msg);
    void stopRecord();

private slots:
    void setupHardware();
    void stopInput();
    void startInput();
    void togglePause();
    void toggleRecord();
    void volumeChanged(int);
    void loadFile();
    void saveTo();
    void exit();

protected slots:
    void tick();
    
};

extern AeonWavePlayer *_mw;

#endif /* !AAXPLAYER_H */