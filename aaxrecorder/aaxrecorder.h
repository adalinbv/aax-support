/*
 * Copyright (C) 2014-2015 by Erik Hofman
 * Copyright (C) 2014-2015 by Adalin B.V.
 *
 * This file is part of AeonWave-AudioRecorder
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

#ifndef AAXRECORDER_H
#define AAXRECORDER_H

#include <QtWidgets/QDialog>
#include <QStringList>
#include <QTimer>
#include <QTime>

#include <SimpleGlob.h>
#include <aax/aeonwave.hpp>

#include "setup.h"
#include "record.h"

#define _MINMAX(a,b,c)  (((a)>(c)) ? (c) : (((a)<(b)) ? (b) : (a)))

class Ui_AudioRecorder;

class AeonWaveRecorder : public QDialog
{
    Q_OBJECT

public:
    AeonWaveRecorder(QWidget *parent = 0);
    ~AeonWaveRecorder();

    Ui_AudioRecorder *ui;

    QTime time;
    Record *record;
    Setup *setup;

    /* device list */
    QString outfile;
    QString idevname_str;
    QString odevname_str;

    struct device_t idevices;
    struct device_t odevices;

    aax::AeonWave outdev;
    aax::AeonWave indev;
    aax::AeonWave file;
    int bitrate;
    float in_freq;
    bool agc_enabled;

    void stopOutput();
    void startOutput();
    void setWildcards();
    void getSystemResources(device_t&, enum aaxRenderMode);
    void freeDevices();

private:
    QTimer timer;

    QString outfiles_path;
    QString wildcards;

    size_t max_samples;
    bool recording;
    bool paused;

    void alert(QString msg);

private slots:
    void setupHardware();
    void viewAbout();
    void viewLicense();
    void exit();

    void stopRecording();
    void startRecording();
    void togglePause();
    void volumeChanged(int);

protected slots:
    void tick();
    
};

extern AeonWaveRecorder *_mw;

#endif /* !AAXRECORDER_H */
