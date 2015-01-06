/*
 * Copyright (C) 2014-2015 by Erik Hofman
 * Copyright (C) 2014-2015 by Adalin B.V.
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

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

#include <copyright.h>
#include <types.h>

#include "aaxrecorder_ui.h"
#include "aaxrecorder.h"
#include "setup.h"

#undef NDEBUG

#ifndef NDEBUG
# define _TEST(a)                \
do {                            \
   int r=(a);                   \
   if (r!=AAX_TRUE) printf("Error at line %i: %s\n",__LINE__,aaxGetErrorString(aaxGetErrorNo())); \
   assert(r==AAX_TRUE); \
} while(0);
#else
# define _TEST(a)                a
#endif

AeonWaveRecorder::AeonWaveRecorder(QWidget *parent) :
    QDialog(parent),
    setup(NULL),
    outdev(NULL),
    indev(NULL),
    file(NULL),
    bitrate(-1),
    in_freq(44100.0f),
    agc_enabled(true),
    wildcards("*.wav"),
    max_samples(0), 
    recording(false),
    paused(false)
{
    ui = new Ui_AudioRecorder;
    ui->setupUi(this);

    QFile QSSFile(":/aaxplayer.qss");
    QSSFile.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(QSSFile.readAll());
    qApp->setStyleSheet(StyleSheet);

    QPixmap RecordPixMap(":/record.png");
    QIcon RecordIcon(RecordPixMap);
    ui->startRecord->setIcon(RecordIcon);
    ui->startRecord->setIconSize(RecordPixMap.rect().size());

    QPixmap PausePixMap(":/pause.png");
    QIcon PauseIcon(PausePixMap);
    ui->pauseRecord->setIcon(PauseIcon);
    ui->pauseRecord->setIconSize(PausePixMap.rect().size());

    QPixmap StopPixMap(":/stop.png");
    QIcon StopIcon(StopPixMap);
    ui->stopRecord->setIcon(StopIcon);
    ui->stopRecord->setIconSize(StopPixMap.rect().size());

    getSystemResources(idevices, AAX_MODE_READ);
    getSystemResources(odevices, AAX_MODE_WRITE_STEREO);
    setWildcards();
    startOutput();

    QObject::connect(ui->actionHardware, SIGNAL(triggered()), this, SLOT(setupHardware()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(viewAbout()));
    QObject::connect(ui->actionLicense, SIGNAL(triggered()), this, SLOT(viewLicense()));

    QObject::connect(ui->startRecord, SIGNAL(released()), this,  SLOT(startRecording()));
    QObject::connect(ui->pauseRecord, SIGNAL(released()), this,  SLOT(togglePause()));
    QObject::connect(ui->stopRecord, SIGNAL(released()), this,  SLOT(stopRecording()));
    QObject::connect(ui->volume, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));

    QObject::connect(&timer, SIGNAL(timeout()), SLOT(tick()));
    timer.setSingleShot(false);
    timer.start(100);

    ui->pctRecording->setValue(0);
}

AeonWaveRecorder::~AeonWaveRecorder()
{
    stopRecording();
    stopOutput();

    if (setup) delete setup;

    delete ui;
}

/* ------------------------------------------------------------------------- */

void
AeonWaveRecorder::tick()
{
    if (recording)
    {
        float hour, minutes, seconds, pos;

        pos = (float)aaxSensorGetOffset(file, AAX_SAMPLES);

        seconds = floorf(pos/in_freq);
        hour = floorf(seconds/(60.0f*60.0f));
        seconds -= hour*60.0f*60.0f;
        minutes = floorf(seconds/60.0f);
        seconds -= minutes*60.0f;

        QString current = QString("%1:%2:%3").arg(hour,2,'f',0,'0').arg(minutes,2,'f',0,'0').arg(seconds,2,'f',0,'0');
        ui->timeCurrent->setText(current);

        seconds = ceilf((max_samples-pos)/in_freq);
        hour = floorf(seconds/(60.0f*60.0f));
        seconds -= hour*60.0f*60.0f;
        minutes = floorf(seconds/60.0f);
        seconds -= minutes*60.0f;

        QString remain = QString("%1:%2:%3").arg(hour,2,'f',0,'0').arg(minutes,2,'f',0,'0').arg(seconds,2,'f',0,'0');
        ui->timeRemaining->setText(remain);

        ui->pctRecording->setValue(100*pos/max_samples);
    }

    static const double MAX = 8388608;
    static const double MAXDIV = 1.0/MAX;
    static const double REFERENCE = 256;
    static const double MIN_DB = 10*log10(1.0/REFERENCE);
    static const double MAX_DB = 0;
    float dB, vu[2];
    for (int track=0; track<2; track++)
    {
        enum aaxSetupType e1 = aaxSetupType(AAX_AVERAGE_VALUE+track);
        int ival;

        ival = aaxMixerGetSetup(outdev, e1);
        dB = (ival > 0) ? 10*log10(ival*MAXDIV) : -1000000.0;
        vu[track] = _MINMAX(100*(dB-MIN_DB)/(MAX_DB-MIN_DB), 0, 99);
    }
    ui->VUleft->setValue(vu[0]);
    ui->VUright->setValue(vu[1]);

    QApplication::processEvents();
}

void
AeonWaveRecorder::exit()
{
    qApp->exit();
}


void
AeonWaveRecorder::startOutput()
{
    std::string d = std::string(odevname_str.toUtf8().constData());
    const char *dev = d.empty() ? NULL : d.c_str();

    outdev = aaxDriverOpenByName(dev, AAX_MODE_WRITE_STEREO);
    if (outdev)
    {
        aaxFilter flt = aaxMixerGetFilter(outdev, AAX_VOLUME_FILTER);
        float vol = _MIN(aaxFilterGetParam(flt, AAX_GAIN, AAX_LINEAR), 1.0f);
        ui->volume->setValue(rintf(vol*100));
        _TEST(aaxFilterDestroy(flt));

        int res = aaxMixerSetState(outdev, AAX_INITIALIZED);
        if (!res)
        {
            alert(tr("<br>Unable to initialize the output device:</br>"
                     "<p><i><b>%1</b></i></p>"
                  ).arg(aaxGetErrorString(aaxGetErrorNo())));
            stopOutput();
        }
        else
        {
            _TEST(aaxMixerSetState(outdev, AAX_PLAYING));
            odevname_str = aaxDriverGetSetup(outdev, AAX_RENDERER_STRING);

            d = std::string(idevname_str.toUtf8().constData());
            dev = d.empty() ? NULL : d.c_str();
            indev = aaxDriverOpenByName(dev, AAX_MODE_READ);
            if (indev)
            {
                if (agc_enabled)
                {
                    aaxFilter filter;

                    filter = aaxMixerGetFilter(indev, AAX_VOLUME_FILTER);
                    if (filter)
                    {
                        aaxFilterSetParam(filter, AAX_AGC_RESPONSE_RATE,
                                                  AAX_LINEAR, 1.5f);
                        _TEST(aaxMixerSetFilter(indev, filter));
                        _TEST(aaxFilterDestroy(filter));
                    }
                }

                _TEST(aaxMixerRegisterSensor(outdev, indev));

                /** must be called after aaxMixerRegisterSensor */
                int res = aaxMixerSetState(indev, AAX_INITIALIZED);
                if (!res)
                {
                    alert(tr("<br>Unable to initialize the input device:</br>"
                             "<p><i><b>%1</b></i></p>"
                            ).arg(aaxGetErrorString(aaxGetErrorNo())));
                    stopOutput();
                }
                else {
                    _TEST(aaxSensorSetState(indev, AAX_CAPTURING));
                }
            }
            else {
               stopOutput();
            }
        }
    }
}

void
AeonWaveRecorder::stopOutput()
{
    if (indev)
    {
        _TEST(aaxMixerSetState(indev, AAX_STOPPED));
        _TEST(aaxMixerDeregisterSensor(outdev, indev));
        _TEST(aaxDriverClose(indev));
        _TEST(aaxDriverDestroy(indev));
        indev = NULL;
    }
    if (outdev)
    {
        _TEST(aaxMixerSetState(outdev, AAX_STOPPED));
        _TEST(aaxDriverClose(outdev));
        _TEST(aaxDriverDestroy(outdev));
        outdev = NULL;
    }
}

void
AeonWaveRecorder::startRecording()
{
    if (!recording)
    {
        recording = false;
        QString fileName = QFileDialog::getSaveFileName(this,
                                   tr("Open Recording Output File"),
                                   outfiles_path, wildcards);
        if (!fileName.isNull())
        {
            QFileInfo f(fileName);

            if (f.suffix().isEmpty()) {
               fileName += ".wav";
            }

            QString filedev = "AeonWave on Audio Files: "+fileName;
            file = aaxDriverOpenByName(filedev.toUtf8().constData(),
                                       AAX_MODE_WRITE_STEREO);
            if (file)
            {
                _TEST(aaxMixerRegisterSensor(outdev, file));
                if (bitrate > 0) {
                    _TEST(aaxMixerSetSetup(file, AAX_BITRATE, bitrate));
                }
                int res = aaxMixerSetState(file, AAX_INITIALIZED);
                if (!res)
                {
                   alert(tr("<br>Unable to initialize the recording device:</br>"
                            "<p><i><b>%1</b></i></p>"
                         ).arg(aaxGetErrorString(aaxGetErrorNo())));
                   return;
                }
                _TEST(aaxMixerSetState(file, AAX_PLAYING));
                recording = true;
            }
        }
        recording = true;
    }
}

void
AeonWaveRecorder::stopRecording()
{
    if (recording)
    {
        _TEST(aaxMixerSetState(file, AAX_STOPPED));
        _TEST(aaxMixerDeregisterSensor(outdev, file));
        aaxDriverClose(file);
        aaxDriverDestroy(file);
        recording = false;
        file = NULL;
    }
}

void
AeonWaveRecorder::togglePause()
{
    if (recording && !paused)
    {
        _TEST(aaxSensorSetState(file, AAX_SUSPENDED));
        paused = true;
    }
}

void
AeonWaveRecorder::volumeChanged(int val)
{
    aaxFilter flt = aaxMixerGetFilter(indev, AAX_VOLUME_FILTER);
    _TEST(aaxFilterSetParam(flt, AAX_GAIN, AAX_LINEAR, (float)val/100.0f));
    _TEST(aaxMixerSetFilter(indev, flt));
    _TEST(aaxFilterDestroy(flt));
}

void
AeonWaveRecorder::setWildcards()
{
    aaxConfig cfgi;

    cfgi = aaxDriverGetByName("AeonWave on Audio Files", AAX_MODE_READ);
    if (cfgi)
    {
        const char *d, *f;

        d = aaxDriverGetDeviceNameByPos(cfgi, 0, AAX_MODE_WRITE_STEREO);
        f = aaxDriverGetInterfaceNameByPos(cfgi, d, 0, AAX_MODE_WRITE_STEREO);
        wildcards = f;
        _TEST(aaxDriverDestroy(cfgi));
    }
}

void
AeonWaveRecorder::alert(QString msg)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("AeonWave Audio Recorder"));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setTextFormat(Qt::RichText);
    if (!msg.isEmpty())
    {
        msgBox.setText(msg);
        msgBox.exec();
   }
}

void
AeonWaveRecorder::getSystemResources(device_t &type, enum aaxRenderMode mode)
{
    type.backend.clear();

    unsigned max = aaxDriverGetCount(mode);
    for (unsigned b=0; b<max; b++)
    {
        aaxConfig cfg = aaxDriverGetByPos(b, mode);
        if (cfg)
        {
            unsigned max_device;
            QString be_name;

            be_name = aaxDriverGetSetup(cfg, AAX_NAME_STRING);
            max_device = aaxDriverGetDeviceCount(cfg, mode);
            for (unsigned d=0; d<max_device; d++)
            {
                QString device = aaxDriverGetDeviceNameByPos(cfg, d, mode);
                struct backend_t backend;
                unsigned max_interface;

                backend.name = be_name + " on " + device;
                backend.interface_name.clear();

                max_interface = aaxDriverGetInterfaceCount(cfg,
                                                    device.toUtf8().constData(),
                                                    mode);
                for (unsigned i=0; i<max_interface; i++)
                {
                    QString if_name = aaxDriverGetInterfaceNameByPos(cfg,
                                                    device.toUtf8().constData(),
                                                    i, mode);
                    backend.interface_name.append(if_name);
                }
                type.backend.append(backend);
            }
            _TEST(aaxDriverDestroy(cfg));
        }
    }
}

void
AeonWaveRecorder::freeDevices()
{
    togglePause();
    if (recording) {
        _TEST(aaxMixerDeregisterSensor(outdev, indev));
    }

    stopOutput();
    startOutput();

    if (recording)
    {
        _TEST(aaxMixerRegisterSensor(outdev, indev));
        startRecording();
    }
}

void
AeonWaveRecorder::setupHardware()
{
    if (!setup) setup = new Setup;
    setup->show();
}

void
AeonWaveRecorder::viewAbout()
{
    alert(tr("<h2>AeonWave Audio Recorder version %1.%2</h2>"
             "<h5>using %3</h5>"
             "<h4>(C) Copyright 2014-2015 by Adalin B.V.</h4><br>"
             "<b><i>AeonWave Audio Recorder is audio recording software.</i></b>"
             "<p><br>This software lets you record audio files using the "
             "automatic audio streaming capabilities of AeonWave. Input is "
             "recorded at the same volume level when Auto Gain Control "
             "(AGC) is selected.<p>"
             "<sub>This softare makes use of: AeonWave 2.5+, Qt 4.7+</sub>"
            ).arg(AAXCONFIG_MAJOR_VERSION).arg(AAXCONFIG_MINOR_VERSION).arg(aaxGetVersionString(outdev)));
}

void
AeonWaveRecorder::viewLicense()
{
    QString text(QObject::trUtf8((const char*)&___COPYING));
    QMessageBox license;

    license.setWindowTitle(tr("License and Copyright"));
//  license.setTextFormat(Qt::RichText);
    license.setDetailedText(text);
    license.setIcon(QMessageBox::Information);
    license.setText(tr("<h5>(C) Copyright 2014-2015 by Adalin B.V.</h5>"
           "This program is free software: you can redistribute it and/or "
           "modify it under the terms of the GNU General Public License as "
           "published by the Free Software Foundation, either version 3 of "
           "the License, or (at your option) any later version.</p>"
           "<p>This program is distributed in the hope that it will be useful, "
           "but WITHOUT ANY WARRANTY; without even the implied warranty of "
           "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
           "GNU General Public License for more details.</p>"
           ));
    license.adjustSize();
    license.exec();
}
