/*
 * Copyright (C) 2014-2015 by Erik Hofman
 * Copyright (C) 2014-2015 by Adalin B.V.
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

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <stdlib.h>
#include <time.h>	// time()
#include <assert.h>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QEventLoop>
#include <QDir>

#include <copyright.h>
#include <types.h>

#include "aaxplayer_ui.h"
#include "aaxplayer.h"
#include "remote_ui.h"
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
    agc_enabled(false),
    autoplay(true),
    indir_pos(0),
    wildcards("*.wav"),
    max_samples(0), 
    play_pressed(false),
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

    QObject::connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(connectRemote()));
    QObject::connect(ui->actionOpenDir, SIGNAL(triggered()), this, SLOT(loadDirectory()));
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
    QObject::connect(ui->actionHardware, SIGNAL(triggered()), this, SLOT(setupHardware()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    QObject::connect(ui->actionInfo, SIGNAL(triggered()), this, SLOT(showSongInfo()));
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
       if (aaxMixerGetState(indev) == AAX_PROCESSED)
       {
           bool nf = new_file;
           stopInput();
           new_file = nf;
       }
   }

   if (!playing && play_pressed && (autoplay && !indir.isEmpty())) {
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

       if (!max_samples) {
          ui->timeRemaining->setText(current);
       }
       else
       {
           seconds = ceilf((max_samples-pos)/in_freq);
           hour = floorf(seconds/(60.0f*60.0f));
           seconds -= hour*60.0f*60.0f;
           minutes = floorf(seconds/60.0f);
           seconds -= minutes*60.0f;

           QString remain = QString("%1:%2:%3").arg(hour,2,'f',0,'0').arg(minutes,2,'f',0,'0').arg(seconds,2,'f',0,'0');
           ui->timeRemaining->setText(remain);

           ui->pctPlaying->setValue(100*pos/max_samples);
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
    play_pressed = false;
    _TEST(aaxMixerSetState(outdev, AAX_STOPPED));
    _TEST(aaxDriverClose(outdev));
    _TEST(aaxDriverDestroy(outdev));
    outdev = NULL;

}

void
AeonWavePlayer::startInput()
{
    play_pressed = true;
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

            if (!max_samples) {
                ui->timeTotal->setText("00:00:00");
            }
            else
            {
                float hour, minutes, seconds;
                seconds = max_samples/in_freq;
                hour = floorf(seconds/(60.0f*60.0f));
                seconds -= hour*60.0f*60.0f;
                minutes = floorf(seconds/60.0f);
                seconds -= minutes*60.0f;

                QString total = QString("%1:%2:%3").arg(hour,2,'f',0,'0').arg(minutes,2,'f',0,'0').arg(seconds,2,'f',0,'0');
                ui->timeTotal->setText(total);
            }
            ui->pctPlaying->setValue(0);

            QString title = aaxDriverGetSetup(indev, AAX_TRACK_TITLE_STRING);
            QString artist = aaxDriverGetSetup(indev, AAX_MUSIC_PERFORMER_STRING);
            if (!title.isEmpty() || !artist.isEmpty())
            {
                if (title.isEmpty()) {
                   title = artist;
                } else if (!title.isEmpty() && !artist.isEmpty()) {
                   title = artist + " - " + title;
                }
            }
            else
            {
                title = aaxDriverGetSetup(indev, AAX_RENDERER_STRING);
                int spos = title.lastIndexOf('/');
                if (!spos) spos = title.lastIndexOf(QDir::separator());
                if (spos >= 0)
                {
                    int dpos = title.lastIndexOf('.');
                    if (dpos >= 0) dpos -= (spos+1);
                    title = title.mid(spos+1, dpos);
                }
            }

            if (!title.isEmpty() && !title.isNull())
            {
                std::string t = std::string(title.toUtf8().constData());
                setWindowTitle(QApplication::translate("AudioPlayer", t.c_str(),
                                             0, QApplication::UnicodeUTF8));

                title = QString("<html><head/><body><p>%1</p></body></html>").arg(title);
                t = std::string(title.toUtf8().constData());
                setToolTip(QApplication::translate("AudioPlayer", t.c_str(), 0,
                                                QApplication::UnicodeUTF8));
            }

            playing = true;
        }
    }
    else if (paused)
    {
        _TEST(aaxSensorSetState(indev, AAX_CAPTURING));
        paused = false;
    }
    else if (playing && !indir.isEmpty())	// Next song is requested
    {
        bool nf = new_file;
        stopInput();
        new_file = nf;
        if (autoplay) {
            startInput();
        }
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
AeonWavePlayer::connectRemote()
{
    Remote remote(this);
    remote.exec();

    indir.clear();
    indir_pos = 0;
    if (infile.endsWith(".m3u", Qt::CaseInsensitive) ||
        infile.endsWith(".m3u8", Qt::CaseInsensitive) ||
        infile.endsWith(".pls", Qt::CaseInsensitive))
    {
        QUrl url = QUrl::fromUserInput(infile);
        QNetworkReply *reply = manager.get(QNetworkRequest(url));

        QObject::connect(reply, SIGNAL(readyRead()),this,SLOT(loadPlaylist()));
    }
}

void
AeonWavePlayer::loadFile()
{
    QString filter = wildcards;
    filter.append(";;*.m3u *.m3u8 *.pls");
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open Audio Input File"),
                                    infiles_path, filter);
    if (!fileName.isNull())
    {
        indir.clear();
        indir_pos = 0;
        new_file = true;
        infile = fileName;
        if (setFileOrPlaylist(indir) == false)
        {
            size_t fpos = infile.lastIndexOf('/');
            infiles_path = infile.mid(0, fpos);
        }
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
            infile = glob.File(n);
            if (setFileOrPlaylist(list) == false) {
                list.append(infile);
            }
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

void
AeonWavePlayer::loadPlaylist()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    QTextStream tstream(reply);
    setFileOrPlaylist(indir, tstream);
}

bool
AeonWavePlayer::setFileOrPlaylist(QStringList& list, QTextStream& tstream)
{
    bool rv = true;

    if (infile.endsWith(".m3u", Qt::CaseInsensitive))
    {
        readM3U(list, tstream);
        infile = QString();
        new_file = true;
    }
    else if (infile.endsWith(".m3u8", Qt::CaseInsensitive))
    {
        readM3U(list, tstream, true);
        infile = QString();
        new_file = true;
    }
    else if (infile.endsWith(".pls", Qt::CaseInsensitive))
    {
        readPLS(list, tstream);
        infile = QString();
        new_file = true;
    }
    else {
        rv = false;
    }

    return rv;
}

bool
AeonWavePlayer::setFileOrPlaylist(QStringList& list)
{
    bool rv = false;

    if (infile.endsWith(".m3u", Qt::CaseInsensitive) ||
        infile.endsWith(".m3u8", Qt::CaseInsensitive) ||
        infile.endsWith(".pls", Qt::CaseInsensitive))
    {
        QFile file(infile);

        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextStream tstream(&file);
        rv = setFileOrPlaylist(list, tstream);
        file.close();
    }

    return rv;
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
AeonWavePlayer::readM3U(QStringList& list, QTextStream &tstream, bool utf8)
{
    if (utf8) {
        tstream.setCodec("UTF-8");
    }
    while(!tstream.atEnd())
    {
        QString line = tstream.readLine();
        if (line.at(0) != '#')
        {
            infile = line;
            if (setFileOrPlaylist(list) == false) {
                list.append(line);
            }
        }
    }
}

void
AeonWavePlayer::readPLS(QStringList& list, QTextStream &tstream, bool utf8)
{
    if (utf8) {
        tstream.setCodec("UTF-8");
    }
    while(!tstream.atEnd())
    {
        QString line = tstream.readLine();
        if (line.startsWith("File", Qt::CaseInsensitive))
        {
            QStringList flist;

            flist = line.split(QRegExp("\\s?=\\s?"));
            if (flist.size() == 2)
            {
                infile = flist[1];
                if (setFileOrPlaylist(list) == false) {
                    list.append(flist[1]);
                }
            }
        }
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
AeonWavePlayer::showSongInfo()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Song Information"));
//  msgBox.setIcon(QMessageBox::Warning);
    msgBox.setTextFormat(Qt::RichText);

    const char *s;

    QString msg = "<table width=\"300\"><tr rowspan=\"9\">";
    msg += "<td></td></tr>";

    s = aaxDriverGetSetup(indev, AAX_MUSIC_PERFORMER_STRING);
    msg += tr("<tr><td>Performer:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = aaxDriverGetSetup(indev, AAX_TRACK_TITLE_STRING);
    msg += tr("<tr><td>Title:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = aaxDriverGetSetup(indev, AAX_ALBUM_NAME_STRING);
    msg += tr("<tr><td>Album:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = aaxDriverGetSetup(indev, AAX_SONG_COMPOSER_STRING);
    msg += tr("<tr><td>Composer:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = aaxDriverGetSetup(indev, AAX_ORIGINAL_PERFORMER_STRING);
    msg += tr("<tr><td>Original:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = aaxDriverGetSetup(indev, AAX_MUSIC_GENRE_STRING);
    msg += tr("<tr><td>Genre:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = aaxDriverGetSetup(indev, AAX_RELEASE_DATE_STRING);
    msg += tr("<tr><td>Release date:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = aaxDriverGetSetup(indev, AAX_TRACK_NUMBER_STRING);
    msg += tr("<tr><td>Track number:&nbsp;</td><td>%1</td></tr>").arg(s ? s : "-");

    s = aaxDriverGetSetup(indev, AAX_SONG_COPYRIGHT_STRING);
    msg += tr("<tr><td>Copyright:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = aaxDriverGetSetup(indev, AAX_WEBSITE_STRING);
    msg += tr("<tr><td>Website:</td><td>%1</td></tr>").arg(s ? s : "-");
    
    msgBox.setText(msg);
    msgBox.exec();
}

void
AeonWavePlayer::viewAbout()
{
    alert(tr("<h2>AeonWave Audio Player version %1.%2</h2>"
             "<h5>using %3</h5>"
             "<h4>(C) Copyright 2014-2015 by Adalin B.V.</h4><br>"
             "<b><i>AeonWave Audio Player is a streaming audio player.</i></b>"
             "<p><br>This software lets you play audio files using the "
             "automatic audio streaming capabilities of AeonWave. It is "
             "possible to select the next audio track from the file menu "
             "while the current track is still playing. Audio tracks are "
             "all played at the same volume level when Auto Gain Control "
             "(AGC) is selected.<p>"
             "<sub>This softare makes use of: AeonWave 2.5+, Qt 4.7+</sub>"
            ).arg(AAXSUPPORT_MAJOR_VERSION).arg(AAXSUPPORT_MINOR_VERSION).arg(aaxGetVersionString(outdev)));
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
