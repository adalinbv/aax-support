/*
 * Copyright (C) 2014-2015 by Erik Hofman
 * Copyright (C) 2014-2015 by Adalin B.V.
 *
 * This file is part of AeonWave-AudioPlayer
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

#ifndef AAXPLAYER_H
#define AAXPLAYER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtGui/QDialog>
#include <QStringList>
#include <QTextStream>
#include <QTimer>
#include <QMenu>

#include <SimpleGlob.h>
#include <aax/aax.h>

#include "setup.h"
#include "remote.h"

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
    QString infile;		// single file or url
    QString odevname_str;
    struct device_t odevices;

    aaxConfig outdev;
    aaxConfig indev;

    aaxConfig file;
    int bitrate;
    float in_freq;
    bool agc_enabled;
    bool autoplay;

    void stopOutput();
    void startOutput();
    void setWildcards();
    void getSystemResources(device_t&, enum aaxRenderMode);
    void freeDevices();

private:
    float elapsed;
    QTimer timer;
    QNetworkAccessManager manager;

    QStringList indir;		// all files from a subdir
    int indir_pos;

    QString outfiles_path;
    QString infiles_path;
    QString wildcards;

    size_t max_samples;
    bool play_pressed;
    bool playing;
    bool paused;
    bool new_file;

    void alert(QString msg);
    bool setFileOrPlaylist(QStringList& list);
    bool setFileOrPlaylist(QStringList& list, QTextStream& tstream);
    void readM3U(QStringList &list, QTextStream& data, bool utf8 = false);
    void readPLS(QStringList &list, QTextStream& data, bool utf8 = false);

    void readBookmarks();
    void writeBookmarks();
    QMenu *bookmarks;

private slots:
    void setupHardware();
    void viewAbout();
    void viewLicense();
    void connectRemote();
    void loadFile();
    void loadDirectory();
    void loadPlaylist();
    void showSongInfo();
    void exit();

    void stopInput();
    void startInput();
    void togglePause();
    void volumeChanged(int);

    void loadBookmark();

protected slots:
    void tick();
    
};

extern AeonWavePlayer *_mw;

#endif /* !AAXPLAYER_H */
