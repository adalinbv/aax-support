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
#include <errno.h>

#include <xml.h>

#include <iostream>
#include <fstream>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QEventLoop>
#include <QDir>

#include <base/api.h>
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
    int r=(a);                    \
    if (r!=AAX_TRUE) printf("Error at line %i: %s\n",__LINE__,aaxGetErrorString(aaxGetErrorNo())); \
    assert(r==AAX_TRUE); \
} while(0);
#else
# define _TEST(a)                a
#endif

AeonWavePlayer::AeonWavePlayer(QWidget *parent) :
    QDialog(parent),
    setup(NULL),
    bitrate(-1),
    in_freq(44100.0f),
    agc_enabled(false),
    autoplay(true),
    show_vu(true),
    elapsed(0.0f),
    indir_pos(0),
    wildcards("*.wav"),
    tracks(2),
    max_samples(0), 
    play_pressed(false),
    remote_stream(false),
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

    favorites = new QMenu(tr("Show Favorite"), ui->menuView);
    ui->menuView->addAction(favorites->menuAction());
    readFavorite();

    QObject::connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(connectRemote()));
    QObject::connect(ui->actionOpenDir, SIGNAL(triggered()), this, SLOT(loadDirectory()));
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(loadFile()));
    QObject::connect(ui->actionHardware, SIGNAL(triggered()), this, SLOT(setupHardware()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    QObject::connect(ui->actionInfo, SIGNAL(triggered()), this, SLOT(showSongInfo()));
    QObject::connect(ui->actionAddFavorite, SIGNAL(triggered()), this, SLOT(addFavorite()));
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

    QFont font1;
    font1.setPointSize(10);
    font1.setBold(true);
    font1.setWeight(75);
    for (int track=0; track<MAX_TRACKS; track++)
    {
        VU[track] = new QProgressBar(this);
        VU[track]->setGeometry(355+20*track, 30, 14, 85);
        VU[track]->setFont(font1);
        VU[track]->setOrientation(Qt::Vertical);
        VU[track]->setFormat(QString());
        VU[track]->setValue(0);
        if (track > 2) VU[track]->setVisible(false);
    }
    resize();
}

AeonWavePlayer::~AeonWavePlayer()
{
    stopInput();
    stopOutput();

    for (int track=0; track<MAX_TRACKS; track++) {
        delete VU[track];
    }

    ui->menuView->clear();
    delete favorites;

    if (setup) delete setup;

    delete ui;
}

void
AeonWavePlayer::resize()
{
    if (show_vu)
    {
        int width = 350 + show_vu*(int)(20*(tracks+0.5f));
        setFixedWidth(width);
        ui->menubar->setGeometry(0, 0, width, 25);

        for (int track=0; track<tracks; track++) {
            VU[track]->setVisible(true);
        }
        for (int track=tracks; track<MAX_TRACKS; track++) {
            VU[track]->setVisible(false);
        }
    }
    else
    {
        setFixedWidth(350);
        ui->menubar->setGeometry(0, 0, 350, 25);

        for (int track=0; track<MAX_TRACKS; track++) {
            VU[track]->setVisible(false);
        }
    }
}

/* ------------------------------------------------------------------------- */

void
AeonWavePlayer::tick()
{
    elapsed += 0.1f;

    if (playing)
    {
        if (indev.get() == AAX_PROCESSED) {
            stopInput();
        }
    }

    if (!playing && play_pressed && (autoplay && !indir.isEmpty())) {
        startInput();
    }

    if (playing)
    {
        float hour, minutes, seconds, pos;

        pos = (float)indev.offset(AAX_SAMPLES);

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

        if (show_vu)
        {
            static const double MAX = 8388608;
            static const double MAXDIV = 1.0/MAX;
            static const double REFERENCE = 256;
            static const double MIN_DB = 10*log10(1.0/REFERENCE);
            static const double MAX_DB = 0;
            for (int track=0; track<tracks; track++)
            {
                enum aaxSetupType e1 = aaxSetupType(AAX_AVERAGE_VALUE+track);

                int ival = indev.get(e1);
                float dB = (ival > 0) ? 10*log10(ival*MAXDIV) : -1000000.0;

                ival = _MINMAX(100*(dB-MIN_DB)/(MAX_DB-MIN_DB), 0, 99);
                VU[track]->setValue(ival);
            }
        }

        QApplication::processEvents();
    }

    if (elapsed >= 3.0f)
    {
        elapsed -= 3.0f;

        QString title = indev.info(AAX_TRACK_TITLE_UPDATE);
        QString artist = indev.info(AAX_MUSIC_PERFORMER_UPDATE);
        if (!title.isEmpty() || !artist.isEmpty())
        {
            if (title.isEmpty()) {
                title = artist;
            } else if (!title.isEmpty() && !artist.isEmpty()) {
                title = artist + " - " + title;
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
    }
}

void
AeonWavePlayer::exit()
{
    qApp->exit();
}

void
AeonWavePlayer::readFavorite()
{
    std::string path = userConfigFile(CONFIG_FILE);
    if (!path.empty())
    {
        void *xid = xmlOpen(path.c_str());
        if (xid)
        {
            void *xbid = xmlNodeGet(xid, "/favorites");
            if (xbid)
            {
                favorites->clear();

                void *xuid = xmlMarkId(xbid);
                unsigned num = xmlNodeGetNum(xuid, "url");
                for (unsigned u=0; u<num; u++)
                {
                    xmlNodeGetPos(xbid, xuid, "url", u);

                    char *name = xmlAttributeGetString(xuid, "name");
                    char *genre = xmlAttributeGetString(xuid, "genre");
                    char *url = xmlGetString(xuid);

                    QString actname;
                    if (name)
                    {
                        actname = name;
                        if (genre)
                        {
                            actname += " (";
                            actname += genre;
                            actname += ")";
                        }
                    }
                    else {
                       actname = url;
                    }

                    QAction *action = new QAction(actname, favorites);
                    action->setObjectName(url);
                    QObject::connect(action, SIGNAL(triggered()),
                                     this, SLOT(loadFavorite()));
                    favorites->addAction(action);
                }
                xmlFree(xuid);
                xmlFree(xbid);
            }
            xmlClose(xid);
        }
    }
}

void
AeonWavePlayer::loadFavorite()
{
   QObject *obj = sender();

   infile = obj->objectName();
   QUrl url(infile);
   QNetworkReply *reply = manager.get(QNetworkRequest(url));

   QObject::connect(reply, SIGNAL(readyRead()), this, SLOT(loadPlaylist()));
}

void
AeonWavePlayer::writeFavorite()
{
    std::string from_path = userConfigFile(CONFIG_FILE);
    if (!from_path.empty())
    {
        const char *username = getenv("SUDO_USER");
        if (!username) username = getenv("USER");
        if (username)
        {
            std::string to_path = from_path + std::string(".old");
            std::rename(from_path.c_str(), to_path.c_str());
        }

        std::ofstream file;
        file.open(from_path.c_str());

        if (file.fail() || file.bad())
        {
            QString msg = tr("Error writing to file: ");
            msg += from_path.c_str();
            msg += "\n\r";
            msg += tr(strerror(errno));
            alert(msg);
            file.close();
            return;
        }

        file << "<?xml version=\"1.0\"?>" << std::endl << std::endl;
        file << "<favorites>" << std::endl;

        QList<QAction*> actionList = favorites->actions();
        for (int i=0; i<actionList.size(); ++i)
        {
            QAction *action = actionList.at(i);
            QString title = action->text();
            QString url = action->objectName();

            file << " <url";
            if (!title.isEmpty())
            {
                int idx = title.indexOf(" (");
                if (idx >= 0)
                {
                   QString name = title.left(idx);
                   QString genre = title.mid(idx+2);

                   genre.remove(genre.size()-1, 1);
                   file << " name=\"" << name.toUtf8().constData() << "\"";
                   file << " genre=\"" << genre.toUtf8().constData() << "\"";
                }
                else {
                    file << "name=\"" << title.toUtf8().constData() << "\"";
                }                 
            }
            file << ">"<< url.toUtf8().constData();
            file << "</url>" << std::endl;
        }

        file << "</favorites>" << std::endl;
        file.close();

#ifndef _WIN32
        int mode = strtol("0600", 0, 8);
        chmod(from_path.c_str(), mode);
#endif
    }

}

void
AeonWavePlayer::addFavorite()
{
    QString actname, objname;
    if (remote_stream)
    {
        QString name = indev.info(AAX_SONG_COMPOSER_STRING); 
        QString genre = indev.info(AAX_MUSIC_GENRE_STRING);

        if (!name.isEmpty())
        {
            actname = name;
            if (!genre.isEmpty()) {
                actname += " (" + genre + ")";
            }
        }
        else {
           actname = infile;
        }
        objname = infile;
    }
    else if (indir.isEmpty())	// file
    {
        QString artist = indev.info(AAX_MUSIC_PERFORMER_STRING);
        QString title = indev.info(AAX_TRACK_TITLE_STRING);

        if (!artist.isEmpty())
        {
            actname = artist;
            if (!title.isEmpty()) {
                actname += " - " + title;
            }
        }
        else {
           actname = infile;
        }
        objname = infile;
    }
    else	// directory
    {
        actname = infiles_path;
        objname = infiles_path;
    }

    QAction *action = new QAction(actname, favorites);
    action->setObjectName(infile);
    QObject::connect(action, SIGNAL(triggered()), this, SLOT(loadFavorite()));
    favorites->addAction(action);

    writeFavorite();
}

void
AeonWavePlayer::startOutput()
{
    std::string d = std::string(odevname_str.toUtf8().constData());
    const char *dev = d.empty() ? NULL : d.c_str();

    outdev = AAX::AeonWave(dev, AAX_MODE_WRITE_STEREO);
    if (outdev)
    {
        AAX::DSP flt = outdev.get(AAX_VOLUME_FILTER);
        float vol = _MIN(flt.get(AAX_GAIN, AAX_LINEAR), 1.0f);
        ui->volume->setValue(rintf(vol*100));

        int res = outdev.set(AAX_INITIALIZED);
        if (res)
        {
            _TEST(outdev.set(AAX_PLAYING));
            odevname_str = outdev.info(AAX_RENDERER_STRING);
        }
        else
        {
            alert(tr("<br>Unable to initialize the output device:</br>"
                        "<p><i><b>%1</b></i></p>"
                    ).arg(outdev.error()));
            _TEST(outdev.close());
        }
    }
}

void
AeonWavePlayer::stopOutput()
{
    play_pressed = false;
    _TEST(outdev.set(AAX_STOPPED));
    _TEST(outdev.close());

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

        indev = AAX::AeonWave(dev, AAX_MODE_READ);
        if (indev)
        {
            /** set capturing Auto-Gain Control (AGC): 0dB */
            if (agc_enabled)
            {
                AAX::DSP filter = indev.get(AAX_VOLUME_FILTER);
                if (filter)
                {
                    filter.set(AAX_AGC_RESPONSE_RATE, AAX_LINEAR, 1.5f);
                    _TEST(indev.set(filter));
                }
            }

            _TEST(outdev.add(indev));

            /** must be called after aaxMixerRegisterSensor */
            int res = indev.set(AAX_INITIALIZED);
            if (!res)
            {
                alert(tr("<br>File initialization error:</br><br>%1</br>"
                            "<p><i><b>%2</b></i></p>"
                        ).arg(infile).arg(indev.error()));
                infile = QString();
                stopInput();
                return;
            }
            _TEST(indev.set(AAX_CAPTURING));

            in_freq = (float)indev.get(AAX_FREQUENCY);
            max_samples = (float)indev.get(AAX_SAMPLES_MAX);
            tracks = _MINMAX(indev.get(AAX_TRACKS), 2, 8);
            resize();

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

            QString title = indev.info(AAX_TRACK_TITLE_STRING);
            QString artist = indev.info(AAX_MUSIC_PERFORMER_STRING);
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
                title = indev.info(AAX_RENDERER_STRING);
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
        _TEST(indev.set(AAX_CAPTURING));
        paused = false;
    }
    else if (playing && !indir.isEmpty())	// Next song is requested
    {
        stopInput();
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
        _TEST(indev.set(AAX_STOPPED));
        _TEST(outdev.remove(indev));
        _TEST(indev.close());
    }

    setWindowTitle(QApplication::translate("AudioPlayer", "AeonWave Audio Player", 0, QApplication::UnicodeUTF8));
    setToolTip(QApplication::translate("AudioPlayer", "Audio Player", 0, QApplication::UnicodeUTF8));
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
        _TEST(indev.set(AAX_SUSPENDED));
        paused = true;
    }
}

void
AeonWavePlayer::volumeChanged(int val)
{
    AAX::DSP flt = outdev.get(AAX_VOLUME_FILTER);
    _TEST(flt.set(AAX_GAIN, AAX_LINEAR, (float)val/100.0f));
    _TEST(outdev.set(flt));
}

void
AeonWavePlayer::setWildcards()
{
    AAX::AeonWave cfgi("AeonWave on Audio Files", AAX_MODE_WRITE_STEREO);
    if (cfgi)
    {
        const char *d, *f;

        d = cfgi.device(0);
        f = cfgi.interface(d, 0);
        wildcards = f;
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

        QObject::connect(reply, SIGNAL(readyRead()),this, SLOT(loadPlaylist()));
    }
    remote_stream = true;
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
        remote_stream = false;
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
        remote_stream = false;
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
    AAX::AeonWave aax;

    type.backend.clear();

    while (const char *d = aax.drivers(mode))
    {
        while (const char *r = aax.devices())
        {
           struct backend_t backend;

           backend.name = d + QString(" on ") + r;

           backend.interface_name.clear();
           while (const char *i = aax.interfaces()) {
               backend.interface_name.append(i);
           }
           type.backend.append(backend);
        }
    }
}

void
AeonWavePlayer::freeDevices()
{
    togglePause();
    if (playing) {
        _TEST(outdev.remove(indev));
    }

    stopOutput();
    startOutput();

    if (playing)
    {
        _TEST(outdev.add(indev));
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

    s = indev.info(AAX_MUSIC_PERFORMER_STRING);
    msg += tr("<tr><td>Performer:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = indev.info(AAX_TRACK_TITLE_STRING);
    msg += tr("<tr><td>Title:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = indev.info(AAX_ALBUM_NAME_STRING);
    msg += tr("<tr><td>Album:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = indev.info(AAX_SONG_COMPOSER_STRING);
    msg += tr("<tr><td>Composer:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = indev.info(AAX_ORIGINAL_PERFORMER_STRING);
    msg += tr("<tr><td>Original:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = indev.info(AAX_MUSIC_GENRE_STRING);
    msg += tr("<tr><td>Genre:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = indev.info(AAX_RELEASE_DATE_STRING);
    msg += tr("<tr><td>Release date:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = indev.info(AAX_TRACK_NUMBER_STRING);
    msg += tr("<tr><td>Track number:&nbsp;</td><td>%1</td></tr>").arg(s ? s : "-");

    s = indev.info(AAX_SONG_COPYRIGHT_STRING);
    msg += tr("<tr><td>Copyright:</td><td>%1</td></tr>").arg(s ? s : "-");

    s = indev.info(AAX_WEBSITE_STRING);
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
            ).arg(AAXSUPPORT_MAJOR_VERSION).arg(AAXSUPPORT_MINOR_VERSION).arg(outdev.version()));
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
            "<p>This program is distributed in the hope that it will be useful,"
            " but WITHOUT ANY WARRANTY; without even the implied warranty of "
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
            "GNU General Public License for more details.</p>"
            ));
    license.adjustSize();
    license.exec();
}
