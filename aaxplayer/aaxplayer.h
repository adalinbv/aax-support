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
#include <QStringList>
#include <QTimer>

#include <SimpleGlob.h>
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
    QTimer timer;

    QStringList indir;		// all files from a subdir
    int indir_pos;

    QString infile;		// or just one fingle file
    QString outfiles_path;
    QString infiles_path;
    QString wildcards;

    size_t max_samples;
    bool playing;
    bool paused;
    bool new_file;

    void alert(QString msg);

private slots:
    void setupHardware();
    void viewAbout();
    void viewLicense();
    void loadFile();
    void loadDirectory();
    void showSongInfo();
//  void saveTo();
    void exit();

    void stopInput();
    void startInput();
    void togglePause();
    void volumeChanged(int);

protected slots:
    void tick();
    
};

extern AeonWavePlayer *_mw;

#endif /* !AAXPLAYER_H */
