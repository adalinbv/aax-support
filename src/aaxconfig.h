/*
 * Copyright (C) 2011 by Adalin B.V.
 *
 * This file is part of AeonWave-Config.
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

#ifndef AAXCONFIG_H
#define AAXCONFIG_H

#include <vector>
#include <string>

#include <aax.h>

#include <QtGui/QDialog>

#include "aaxconfig_ui.h"

class AeonWaveConfig : public QDialog
{
    Q_OBJECT

public:
    AeonWaveConfig(QWidget *parent = 0);
    ~AeonWaveConfig();

private slots:
    void changeProductKey(QString str);
    void changeRefreshRate(int val);
    void changeSpeakerSetup(int val);
    void changeGeneralSampleFreq(int val);
    void changeInputSampleFreq(int val);
    void changeOutputSampleFreq(int val);
    void changeNoSpeakers(int val);
    void changeInputDevice(int val);
    void changeOutputDevice(int val);
    void changeBackend(int val);
    void writeConfig();

private:
    Ui_Configuration *ui;

    typedef struct _device
    {
        std::string name;
        unsigned int sample_freq;
        unsigned int no_speakers;
        enum aaxRenderMode setup;

        _device() :
            name("default"),
            sample_freq(44100),
            no_speakers(2),
            setup(AAX_MODE_WRITE_STEREO) {};
        _device(std::string n) :
            name(n),
            sample_freq(44100),
            no_speakers(2),
            setup(AAX_MODE_WRITE_STEREO) {};
        ~_device() {};
    } device_t;

    typedef struct _backend
    {
        std::string name;

        unsigned int current_output_device;
        std::vector<device_t*> output;

        unsigned int current_input_device;
        std::vector<device_t*> input;
           
    } backend_t;
 
    std::string product_key;
    unsigned int refresh_rate;
    unsigned int general_sample_freq;
    enum aaxRenderMode general_setup;

    unsigned int current_backend;
    std::vector<backend_t*> backends;

    void getSystemResources();
    void readConfigFiles();
    void readConfigSettings(void*);
    void writeConfigFile();
    void displayUiConfig();
    void displayUiDevicesConfig();
    unsigned int FreqToIndex(unsigned int freq);
};

#endif // AAXCONFIG_H
