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

#include <math.h>
#include <assert.h>

#include <QMessageBox>
#include <QFileDialog>

#include <base/types.h>
#include "aaxplayer_ui.h"
#include "aaxplayer.h"
#include "setup.h"

#undef NDEBUG

#ifndef NDEBUG
# define _ATB(a)                \
do {                            \
   int r=(a);                   \
   if (r!=AAX_TRUE) printf("Error at line %i: %s\n",__LINE__,aaxGetErrorString(aaxGetErrorNo())); \
   assert(r==AAX_TRUE); \
} while(0);
#else
# define _ATB(a)                a
#endif

AeonWavePlayer::AeonWavePlayer(QWidget *parent) :
    QDialog(parent),
    setup(NULL),
    outdev(NULL),
    indev(NULL),
    agc_enabled(true),
    playing(false),
    paused(false),
    file(NULL),
    bitrate(-1),
    recording(false),
    outfiles_path(""),
    infiles_path(""),
    max_samples(0)
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
    openOutputDevice();

    QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));
    QObject::connect(ui->actionHardware, SIGNAL(triggered()), this, SLOT(setupHardware()));

    QObject::connect(ui->startPlay, SIGNAL(released()), this,  SLOT(startInput()));
    QObject::connect(ui->pausePlay, SIGNAL(released()), this,  SLOT(togglePause()));
    QObject::connect(ui->stopPlay, SIGNAL(released()), this,  SLOT(stopInput()));
    QObject::connect(ui->volume, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
//  QObject::connect(ui->startRecord, SIGNAL(released()), this,  SLOT(toggleRecord()));
//  QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveTo()));

    QObject::connect(&timer, SIGNAL(timeout()), SLOT(tick()));
    timer.setSingleShot(false);
    timer.start(100);

    ui->pctPlaying->setValue(0);
}

AeonWavePlayer::~AeonWavePlayer()
{
    stopRecord();
    stopInput();

    aaxMixerSetState(outdev, AAX_STOPPED);
    aaxDriverClose(outdev);
    aaxDriverDestroy(outdev);
    outdev = NULL;

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
AeonWavePlayer::openOutputDevice()
{
    std::string d = std::string(odevname_str.toUtf8().constData());
    const char *dev = d.empty() ? NULL : d.c_str();

    outdev = aaxDriverOpenByName(dev, AAX_MODE_WRITE_STEREO);
    if (outdev)
    {
        aaxFilter flt = aaxMixerGetFilter(outdev, AAX_VOLUME_FILTER);
        float vol = _MIN(aaxFilterGetParam(flt, AAX_GAIN, AAX_LINEAR), 1.0f);
        ui->volume->setValue(rintf(vol*100));
        aaxFilterDestroy(flt);

        int res = aaxMixerSetState(outdev, AAX_INITIALIZED);
        if (res) {
            aaxMixerSetState(outdev, AAX_PLAYING);
        }
        else
        {
            alert(tr("<br>Unable to initialize the output device:</br>"
                     "<p><i><b>%1</b></i></p>"
                  ).arg(aaxGetErrorString(aaxGetErrorNo())));
            _ATB(aaxDriverClose(outdev));
            _ATB(aaxDriverDestroy(outdev));
            outdev = NULL;
        }
    }
}

void
AeonWavePlayer::startInput()
{
    if (!playing)
    {
        QString idevname_str = "AeonWave on Audio Files: "+infile;
        std::string d = std::string(idevname_str.toUtf8().constData());
        const char *dev = d.empty() ? NULL : d.c_str();

        indev = aaxDriverOpenByName(dev, AAX_MODE_READ);
        if (indev)
        {
            /** set capturing Auto-Gain Control (AGC): 0dB */
            aaxFilter filter = aaxMixerGetFilter(indev, AAX_VOLUME_FILTER);
            if (filter)
            {
                aaxFilterSetParam(filter,AAX_AGC_RESPONSE_RATE,AAX_LINEAR,1.5f);
                aaxMixerSetFilter(indev, filter);
                aaxFilterDestroy(filter);
            }

            _ATB(aaxMixerRegisterSensor(outdev, indev));

            /** must be called after aaxMixerRegisterSensor */
            _ATB(aaxMixerSetState(indev, AAX_INITIALIZED));
            _ATB(aaxSensorSetState(indev, AAX_CAPTURING));

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

            QString fname = aaxDriverGetSetup(indev, AAX_RENDERER_STRING);
            size_t spos = fname.lastIndexOf('/');
            size_t dpos = fname.lastIndexOf('.');
            if (dpos) dpos -= (spos+1);
            QString filename = infile.mid(spos+1, dpos);
            std::string f = std::string(filename.toUtf8().constData());
            if (!f.empty())
            {
                const char *title =f.c_str();
                setWindowTitle(QApplication::translate("AudioPlayer", title, 0, QApplication::UnicodeUTF8));
            }

            playing = true;
        }
    }
    else if (paused)
    {
        _ATB(aaxSensorSetState(indev, AAX_CAPTURING));
        paused = false;
    }
}

void
AeonWavePlayer::stopInput()
{
    if (playing)
    {
        _ATB(aaxMixerSetState(indev, AAX_STOPPED));
        _ATB(aaxMixerDeregisterSensor(outdev, indev));
        aaxDriverClose(indev);
        aaxDriverDestroy(indev);
    }

    setWindowTitle(QApplication::translate("AudioPlayer", "AeonWave Audio Player", 0, QApplication::UnicodeUTF8));
    ui->VUleft->setValue(0);
    ui->VUright->setValue(0);
    ui->timeTotal->setText("00:00:00");
    max_samples = 0;
    playing = false;
    paused = false;
}

void
AeonWavePlayer::stopRecord()
{
    if (recording)
    {
        recording = false;
        aaxMixerSetState(file, AAX_STOPPED);
        aaxMixerDeregisterSensor(file, outdev);
        aaxDriverClose(file);
        aaxDriverDestroy(file);
    }
}


void
AeonWavePlayer::togglePause()
{
    if (!paused)
    {
        _ATB(aaxSensorSetState(indev, AAX_SUSPENDED));
        paused = true;
    }
}

void
AeonWavePlayer::toggleRecord()
{
    if (recording) {
        stopRecord();
    }
    else
    {
        if (!file) {
            saveTo();
        }
        if (file)
        {
            aaxMixerSetState(file, AAX_CAPTURING);
            recording = true;
        }
    }
}

void
AeonWavePlayer::volumeChanged(int val)
{
    aaxFilter flt = aaxMixerGetFilter(outdev, AAX_VOLUME_FILTER);
    aaxFilterSetParam(flt, AAX_GAIN, AAX_LINEAR, (float)val/100.0f);
    aaxMixerSetFilter(outdev, flt);
    aaxFilterDestroy(flt);
}

void
AeonWavePlayer::loadFile()
{
    QString filter = "*.wav";
    aaxConfig cfgi;
 
    cfgi = aaxDriverGetByName("AeonWave on Audio Files", AAX_MODE_READ);
    if (cfgi)
    {
        const char *d, *f;

        d = aaxDriverGetDeviceNameByPos(cfgi, 0, AAX_MODE_WRITE_STEREO);
        f = aaxDriverGetInterfaceNameByPos(cfgi, d, 0, AAX_MODE_WRITE_STEREO);
        filter = f;
        aaxDriverDestroy(cfgi);
    }

    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open Audio Input File"),
                                    infiles_path, filter);
    if (!fileName.isNull())
    {
        infile = fileName;
        size_t fpos = infile.lastIndexOf('/');
        infiles_path = infile.mid(0, fpos);
    }
}

void
AeonWavePlayer::saveTo()
{
    QString filter = "*.wav";
    aaxConfig cfgo;

    cfgo = aaxDriverGetByName("AeonWave on Audio Files", AAX_MODE_READ);
    if (cfgo)
    {
        const char *d, *f;

        d = aaxDriverGetDeviceNameByPos(cfgo, 0, AAX_MODE_WRITE_STEREO);
        f = aaxDriverGetInterfaceNameByPos(cfgo, d, 0, AAX_MODE_WRITE_STEREO);
        filter = f;
        aaxDriverDestroy(cfgo);
    }

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
            aaxMixerSetState(file, AAX_INITIALIZED);
        }
    }
}

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
            aaxDriverDestroy(cfg);;
        }
    }
}

void
AeonWavePlayer::freeDevices()
{
    stopRecord();
    stopInput();

    aaxMixerSetState(outdev, AAX_STOPPED);
    aaxDriverClose(outdev);
    aaxDriverDestroy(outdev);
    outdev = NULL;

    openOutputDevice();
}

void
AeonWavePlayer::setupHardware()
{
    if (!setup) setup = new Setup;
    setup->show();
}
