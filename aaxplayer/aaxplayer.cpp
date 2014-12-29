/*
 * Copyright (C) 2014 by Adalin B.V.
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

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <QMessageBox>
#include <QFileDialog>

#include <copyright.h>
#include <types.h>

#include "aaxplayer_ui.h"
#include "aaxplayer.h"
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

AeonWavePlayer::AeonWavePlayer(QWidget *parent) :
    QDialog(parent),
    setup(NULL),
    outdev(NULL),
    indev(NULL),
    file(NULL),
    bitrate(-1),
    in_freq(44100.0f),
    agc_enabled(true),
    autoplay(false),
    indir_pos(0),
    wildcards("*.wav"),
    max_samples(0), 
    playing(false),
    paused(false)
{
    ui = new Ui_AudioPlayer;
    ui->setupUi(this);

    QFile QSSFile(":/aaxplayer.qss");
    QSSFile.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(QSSFile.readAll());
    qApp->setStyleSheet(StyleSheet);

    QPixmap PlayPixMap(":/play.png");
    QIcon PlayIcon(PlayPixMap);
    ui->startPlay->setIcon(PlayIcon);
    ui->startPlay->setIconSize(PlayPixMap.rect().size());

    QPixmap PausePixMap(":/pause.png");
    QIcon PauseIcon(PausePixMap);
    ui->pausePlay->setIcon(PauseIcon);
    ui->pausePlay->setIconSize(PausePixMap.rect().size());

    QPixmap StopPixMap(":/stop.png");
    QIcon StopIcon(StopPixMap);
    ui->stopPlay->setIcon(StopIcon);
    ui->stopPlay->setIconSize(StopPixMap.rect().size());

    getSystemResources(odevices, AAX_MODE_WRITE_STEREO);
    setWildcards();
    startOutput();

    QObject::connect(ui->actionOpenDir, SIGNAL(triggered()), this, SLOT(loadDirectory()));
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
    QObject::connect(ui->actionHardware, SIGNAL(triggered()), this, SLOT(setupHardware()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(viewAbout()));
    QObject::connect(ui->actionLicense, SIGNAL(triggered()), this, SLOT(viewLicense()));

    QObject::connect(ui->startPlay, SIGNAL(released()), this,  SLOT(startInput()));
    QObject::connect(ui->pausePlay, SIGNAL(released()), this,  SLOT(togglePause()));
    QObject::connect(ui->stopPlay, SIGNAL(released()), this,  SLOT(stopInput()));
    QObject::connect(ui->volume, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));

    QObject::connect(&timer, SIGNAL(timeout()), SLOT(tick()));
    timer.setSingleShot(false);
    timer.start(100);

    ui->pctPlaying->setValue(0);
}

AeonWavePlayer::~AeonWavePlayer()
{
    stopInput();
    stopOutput();

    if (setup) delete setup;

    delete ui;
}

/* ------------------------------------------------------------------------- */

void
AeonWavePlayer::tick()
{
   if (playing)
   {
       if (aaxMixerGetState(indev) == AAX_PROCESSED) {
           stopInput();
       }
   }

   if (!playing && (autoplay || new_file)) {
       startInput();
   }

   if (playing)
   {
       float hour, minutes, seconds, pos;

       pos = (float)aaxSensorGetOffset(indev, AAX_SAMPLES);

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

       ui->pctPlaying->setValue(100*pos/max_samples);

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
}

void
AeonWavePlayer::exit()
{
    qApp->exit();
}


void
AeonWavePlayer::startOutput()
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
        if (res)
        {
            _TEST(aaxMixerSetState(outdev, AAX_PLAYING));
            odevname_str = aaxDriverGetSetup(outdev, AAX_RENDERER_STRING);
        }
        else
        {
            alert(tr("<br>Unable to initialize the output device:</br>"
                     "<p><i><b>%1</b></i></p>"
                  ).arg(aaxGetErrorString(aaxGetErrorNo())));
            _TEST(aaxDriverClose(outdev));
            _TEST(aaxDriverDestroy(outdev));
            outdev = NULL;
        }
    }
}

void
AeonWavePlayer::stopOutput()
{
    _TEST(aaxMixerSetState(outdev, AAX_STOPPED));
    _TEST(aaxDriverClose(outdev));
    _TEST(aaxDriverDestroy(outdev));
    outdev = NULL;

}

void
AeonWavePlayer::startInput()
{
    if (!playing && (!infile.isNull() || !indir.isEmpty()))
    {
        if (!indir.isEmpty())
        {
            infile.clear();
            infile = indir.at(indir_pos++);
            if (indir_pos >= indir.size()) {
                indir_pos = 0;
            }
        }

        QString idevname_str = "AeonWave on Audio Files: "+infile;
        std::string d = std::string(idevname_str.toUtf8().constData());
        const char *dev = d.empty() ? NULL : d.c_str();

        indev = aaxDriverOpenByName(dev, AAX_MODE_READ);
        if (indev)
        {
            /** set capturing Auto-Gain Control (AGC): 0dB */
            if (agc_enabled)
            {
                aaxFilter filter = aaxMixerGetFilter(indev, AAX_VOLUME_FILTER);
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
                alert(tr("<br>File initialization error:</br><br>%1</br>"
                         "<p><i><b>%2</b></i></p>"
                        ).arg(infile).arg(aaxGetErrorString(aaxGetErrorNo())));
                infile = QString();
                stopInput();
                return;
            }
            _TEST(aaxSensorSetState(indev, AAX_CAPTURING));

            in_freq = (float)aaxMixerGetSetup(indev, AAX_FREQUENCY);
            max_samples = (float)aaxMixerGetSetup(indev, AAX_SAMPLES_MAX);

            float hour, minutes, seconds;
            seconds = max_samples/in_freq;
            hour = floorf(seconds/(60.0f*60.0f));
            seconds -= hour*60.0f*60.0f;
            minutes = floorf(seconds/60.0f);
            seconds -= minutes*60.0f;

            QString total = QString("%1:%2:%3").arg(hour,2,'f',0,'0').arg(minutes,2,'f',0,'0').arg(seconds,2,'f',0,'0');
            ui->timeTotal->setText(total);

            QString filename = aaxDriverGetSetup(indev, AAX_RENDERER_STRING);
            int spos = filename.lastIndexOf('/');
            if (spos >= 0)
            {
                int dpos = filename.lastIndexOf('.');
                if (dpos >= 0) dpos -= (spos+1);
                filename = filename.mid(spos+1, dpos);
            }
            if (!filename.isEmpty() && !filename.isNull())
            {
                std::string f = std::string(filename.toUtf8().constData());
                const char *title = f.c_str();
                setWindowTitle(QApplication::translate("AudioPlayer", title,
                                             0, QApplication::UnicodeUTF8));

                filename = QString("<html><head/><body><p>%1</p></body></html>").arg(filename);
                f = std::string(filename.toUtf8().constData());
                title = f.c_str();
                setToolTip(QApplication::translate("AudioPlayer", title, 0,
                                                QApplication::UnicodeUTF8));
            }

            playing = true;
            if (indir.isEmpty()) {
                new_file = false;
            } else {
               autoplay = true;
            }
        }
    }
    else if (paused)
    {
        _TEST(aaxSensorSetState(indev, AAX_CAPTURING));
        paused = false;
    }
    else if (playing && !indir.isEmpty()) {
        stopInput();
    }
}

void
AeonWavePlayer::stopInput()
{
    if (playing)
    {
        _TEST(aaxMixerSetState(indev, AAX_STOPPED));
        _TEST(aaxMixerDeregisterSensor(outdev, indev));
        _TEST(aaxDriverClose(indev));
        _TEST(aaxDriverDestroy(indev));
    }

    setWindowTitle(QApplication::translate("AudioPlayer", "AeonWave Audio Player", 0, QApplication::UnicodeUTF8));
    setToolTip(QApplication::translate("AudioPlayer", "Audio Player", 0, QApplication::UnicodeUTF8));
    ui->VUleft->setValue(0);
    ui->VUright->setValue(0);
    ui->timeTotal->setText("00:00:00");
    new_file = false;
    max_samples = 0;
    playing = false;
    paused = false;
}

void
AeonWavePlayer::togglePause()
{
    if (playing && !paused)
    {
        _TEST(aaxSensorSetState(indev, AAX_SUSPENDED));
        paused = true;
    }
}

void
AeonWavePlayer::volumeChanged(int val)
{
    aaxFilter flt = aaxMixerGetFilter(outdev, AAX_VOLUME_FILTER);
    _TEST(aaxFilterSetParam(flt, AAX_GAIN, AAX_LINEAR, (float)val/100.0f));
    _TEST(aaxMixerSetFilter(outdev, flt));
    _TEST(aaxFilterDestroy(flt));
}

void
AeonWavePlayer::setWildcards()
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
AeonWavePlayer::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open Audio Input File"),
                                    infiles_path, wildcards);
    if (!fileName.isNull())
    {
        infile = fileName;
        size_t fpos = infile.lastIndexOf('/');
        infiles_path = infile.mid(0, fpos);
        indir.clear();
        indir_pos = 0;
        new_file = true;
    }
}

void
AeonWavePlayer::loadDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                               tr("Open Audio Directory"),
                               infiles_path);
    if (!dir.isNull())
    {
        QStringList filters = wildcards.split(' ');
        CSimpleGlob glob(SG_GLOB_ONLYFILE);

        for (int n=0; n<filters.size(); n++)
        {
            std::string d = std::string(dir.toUtf8().constData());
            std::string p = std::string(filters[n].toUtf8().constData());

            std::string pattern = d+"/"+p;
            glob.Add(pattern.c_str());

            pattern = d+"/*/"+p;
            glob.Add(pattern.c_str());

            pattern = d+"/*/*/"+p;
            glob.Add(pattern.c_str());

            // Three levels deep shoud be enough
        }

        indir_pos = 0;
        indir.clear();
        QStringList list;
        for (int n=0; n<glob.FileCount(); n++)
        {
            list.append(glob.File(n));
        }

        // randomize the list
        srand(time(NULL));
        int size = list.size();
        while (size)
        {
            int pos = rand() % size;
            indir.append(list.at(pos));
            list.removeAt(pos);
            size = list.size();
        }
        
        infiles_path = dir;
        new_file = true;
    }
}

#if 0
void
AeonWavePlayer::saveTo()
{
    aaxConfig cfgo;

    QString fileName = QFileDialog::getSaveFileName(this,
                                          tr("Open Audio Output File"),
                                          outfiles_path, filter);
    if (!fileName.isNull())
    {
        QFileInfo f(fileName);

        if (f.suffix().isEmpty()) {
           fileName += ".wav";
        }
        outfiles_path = f.absolutePath();

        QString odevname_str = "AeonWave on Audio Files: "+fileName;
        std::string d = std::string(odevname_str.toUtf8().constData());
        const char *dev = d.empty() ? NULL : d.c_str();

        file = aaxDriverOpenByName(dev, AAX_MODE_READ);
        if (file)
        {
            _TEST(aaxMixerSetState(file, AAX_INITIALIZED));
        }
    }
}
#endif

void
AeonWavePlayer::alert(QString msg)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("AeonWave Audio Player"));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setTextFormat(Qt::RichText);
    if (!msg.isEmpty())
    {
        msgBox.setText(msg);
        msgBox.exec();
   }
}

void
AeonWavePlayer::getSystemResources(device_t &type, enum aaxRenderMode mode)
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
AeonWavePlayer::freeDevices()
{
    togglePause();
    if (playing) {
        _TEST(aaxMixerDeregisterSensor(outdev, indev));
    }

    stopOutput();
    startOutput();

    if (playing)
    {
        _TEST(aaxMixerRegisterSensor(outdev, indev));
        startInput();
    }
}

void
AeonWavePlayer::setupHardware()
{
    if (!setup) setup = new Setup;
    setup->show();
}

void
AeonWavePlayer::viewAbout()
{
    alert(tr("<h2>AeonWave Audio Player version %1.%2</h2>"
             "<h5>using %3</h5>"
             "<h4>(C) Copyright 2014,2015 by Adalin B.V.</h4><br>"
             "<b><i>AeonWave Audio Player is a streaming audio player.</i></b>"
             "<p><br>This software lets you play audio files using the "
             "automatic audio streaming capabilities of AeonWave. It is "
             "possible to select the next audio track from the file menu "
             "while the current track is still playing. Audio tracks are "
             "all played at the same volume level when Auto Gain Control "
             "(AGC) is selected.<p>"
             "<sub>This softare makes use of: AeonWave 2.5+, Qt 4.7+</sub>"
            ).arg(AAXCONFIG_MAJOR_VERSION).arg(AAXCONFIG_MINOR_VERSION).arg(aaxGetVersionString(outdev)));
}

void
AeonWavePlayer::viewLicense()
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
