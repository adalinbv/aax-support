/*
 * Copyright (C) 2014-2016 by Erik Hofman
 * Copyright (C) 2014-2016 by Adalin B.V.
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

#include <cmath>
#include <cstdlib>
#include <cassert>

#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

#include <copyright.h>
#include <types.h>

#include "ui_aaxrecorder.h"
#include "aaxrecorder.h"
#include "ui_record.h"
#include "setup.h"

#undef NDEBUG

#ifndef NDEBUG
# define _TEST(a)                \
do {                            \
   bool r=(a);                   \
   if (r!=AAX_TRUE) printf("Error at line %i: %s\n",__LINE__,aaxGetErrorString(aaxGetErrorNo())); \
   assert(r==AAX_TRUE); \
} while(0);
#else
# define _TEST(a)                a
#endif

AeonWaveRecorder::AeonWaveRecorder(QWidget *parent) :
    QDialog(parent),
    time(24,59,59,59),
    record(NULL),
    setup(NULL),
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

    QFile QSSFile(":/aaxrecorder.qss");
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

    if (record) delete record;
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

        pos = (float)file.offset(AAX_SAMPLES);

        seconds = floorf(pos/in_freq);
        hour = floorf(seconds/(60.0f*60.0f));
        seconds -= hour*60.0f*60.0f;
        minutes = floorf(seconds/60.0f);
        seconds -= minutes*60.0f;

        QString current = QString("%1:%2:%3").arg(hour,2,'f',0,'0').arg(minutes,2,'f',0,'0').arg(seconds,2,'f',0,'0');
        ui->timeCurrent->setText(current);

        if (max_samples > 0)
        {
            seconds = ceilf((max_samples-pos)/in_freq);
            hour = floorf(seconds/(60.0f*60.0f));
            seconds -= hour*60.0f*60.0f;
            minutes = floorf(seconds/60.0f);
            seconds -= minutes*60.0f;

            QString remain = QString("%1:%2:%3").arg(hour,2,'f',0,'0').arg(minutes,2,'f',0,'0').arg(seconds,2,'f',0,'0');
            ui->timeRemaining->setText(remain);

            ui->pctRecording->setValue(100*pos/max_samples);
        }
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

        ival = outdev.get(e1);
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

    outdev = aax::AeonWave(dev, AAX_MODE_WRITE_STEREO);
    if (outdev)
    {
#if 0
        aax:dsp dsp = outdev.get(AAX_VOLUME_FILTER);
        float vol = _MIN(dsp.get(AAX_GAIN), 1.0f);
        ui->volume->setValue(rintf(vol*100));
#endif

        int res = outdev.set(AAX_INITIALIZED);
        if (!res)
        {
            alert(tr("<br>Unable to initialize the output device:</br>"
                     "<p><i><b>%1</b></i></p>"
                  ).arg(outdev.strerror()));
            stopOutput();
        }
        else
        {
            _TEST(outdev.set(AAX_PLAYING));
            odevname_str = outdev.info(AAX_RENDERER_STRING);

            d = std::string(idevname_str.toUtf8().constData());
            dev = d.empty() ? NULL : d.c_str();
            indev = aax::AeonWave(dev, AAX_MODE_READ);
            if (indev)
            {
                idevname_str = indev.info(AAX_RENDERER_STRING);

                _TEST(outdev.add(indev));

                /** must be called after aaxMixerRegisterSensor */
                int res = indev.set(AAX_INITIALIZED);
                if (!res)
                {
                    alert(tr("<br>Unable to initialize the input device:</br>"
                             "<p><i><b>%1</b></i></p>"
                            ).arg(indev.strerror()));
                    stopOutput();
                }
                else
                {
                    aax::dsp dsp = indev.get(AAX_VOLUME_FILTER);
                    float val = ui->volume->value()/100.0f;
                    _TEST(dsp.set(AAX_GAIN, val));
                    if (agc_enabled) {
                       _TEST(dsp.set(AAX_AGC_RESPONSE_RATE, 1.5f));
                    }
                    _TEST(indev.set(dsp));

                    _TEST(indev.sensor(AAX_CAPTURING));
                    in_freq = outdev.get(AAX_FREQUENCY);
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
        _TEST(indev.sensor(AAX_STOPPED));
        _TEST(outdev.remove(indev));
        _TEST(indev.close());
    }
    if (outdev)
    {
        _TEST(outdev.set(AAX_STOPPED));
        _TEST(outdev.close());
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
            file = aax::AeonWave(filedev.toUtf8().constData(),
                                       AAX_MODE_WRITE_STEREO);
            if (file)
            {
                _TEST(outdev.add(file));
                if (bitrate > 0) {
                    _TEST(file.set(AAX_BIT_RATE, bitrate));
                }
                int res = file.set(AAX_INITIALIZED);
                if (!res)
                {
                   alert(tr("<br>Unable to initialize the recording device:</br>"
                            "<p><i><b>%1</b></i></p>"
                         ).arg(file.strerror()));
                   return;
                }

                 Record record(this);
                 if (record.exec() == QDialog::Accepted)
                 {
                    int hour = time.hour();
                    int minutes = time.minute();
                    int seconds = time.second();

                    QString current = QString("%1:%2:%3").arg(hour,2,'f',0,'0').arg(minutes,2,'f',0,'0').arg(seconds,2,'f',0,'0');
                    ui->timeTotal->setText(current);

                    max_samples = in_freq*(seconds + 60*minutes + 60*60*hour);
                 }

                _TEST(file.set(AAX_PLAYING));
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
        _TEST(file.set(AAX_STOPPED));
        _TEST(outdev.remove(file));
        file.close();
        recording = false;
    }
}

void
AeonWaveRecorder::togglePause()
{
    if (recording && !paused)
    {
        _TEST(file.set(AAX_SUSPENDED));
        paused = true;
    }
}

void
AeonWaveRecorder::volumeChanged(int val)
{
    if (indev)
    {
        aax::dsp dsp = indev.get(AAX_VOLUME_FILTER);
        _TEST(dsp.set(AAX_GAIN, (float)val/100.0f));
        _TEST(indev.set(dsp));
    }
}

void
AeonWaveRecorder::setWildcards()
{
    aax::AeonWave cfgo("AeonWave on Audio Files", AAX_MODE_WRITE_STEREO);
    if (cfgo)
    {
        const char *d, *f;

        d = cfgo.device(0);
        f = cfgo.interface_name(d, 0);
        wildcards = f;
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
    aax::AeonWave aax;

    type.backend.clear();

    while (const char *d = aax.drivers(mode))
    {
        int no_devices = 0;
        while (const char *r = aax.devices())
        {
           struct backend_t backend;

           backend.name = d + QString(" on ") + r;

           backend.interface_name.clear();
           while (const char *i = aax.interfaces()) {
               backend.interface_name.append(i);
           }
           type.backend.append(backend);
           no_devices++;
        }
#if 0
        if (!no_devices)
        {
            struct backend_t backend;
            backend.name = d;
            type.backend.append(backend);
        }
#endif
    }
}

void
AeonWaveRecorder::freeDevices()
{
    togglePause();
    if (recording) {
        _TEST(outdev.remove(indev));
    }

    stopOutput();
    startOutput();

    if (recording)
    {
        _TEST(outdev.remove(indev));
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
             "<h4>(C) Copyright 2014-2016 by Adalin B.V.</h4><br>"
             "<b><i>AeonWave Audio Recorder is audio recording software.</i></b>"
             "<p><br>This software lets you record audio files using the "
             "automatic audio streaming capabilities of AeonWave. Input is "
             "recorded at the same volume level when Auto Gain Control "
             "(AGC) is selected.<p>"
             "<sub>This softare makes use of: AeonWave 2.5+, Qt 4.7+</sub>"
            ).arg(AAXSUPPORT_MAJOR_VERSION).arg(AAXSUPPORT_MINOR_VERSION).arg(outdev.version()));
}

void
AeonWaveRecorder::viewLicense()
{
    QString text((const char*)&___COPYING);
    QMessageBox license;

    license.setWindowTitle(tr("License and Copyright"));
//  license.setTextFormat(Qt::RichText);
    license.setDetailedText(text);
    license.setIcon(QMessageBox::Information);
    license.setText(tr("<h5>(C) Copyright 2014-2016 by Adalin B.V.</h5>"
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
