/*
 * Copyright (C) 2011-2014 by Adalin B.V.
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

class Ui_Configuration;

class AeonWaveConfig : public QDialog
{
    Q_OBJECT

public:
    AeonWaveConfig(QWidget *parent = 0);
    ~AeonWaveConfig();

    std::string get_productkey() {
        return product_key;
    }

    Ui_Configuration *ui;

private slots:
    void changeTimerDriven(bool);
    void changeShared(bool);
    void changeRefreshRate(int val);
    void changeSpeakerSetup(int val);
    void changeOutputBitrate(int val);
    void changeInputSampleFreq(int val);
    void changeOutputSampleFreq(int val);
    void changeNoSpeakers(int val);
     void changeNoPeriods(int val);
    void changeInputConnector(int val);
    void changeOutputConnector(int val);
    void changeDevice(int val);
    void writeConfig();

    void changeProductKey(QString str);

private:
    typedef struct _connector
    {
        std::string name;
        unsigned int bitrate;
        unsigned int sample_freq;
        unsigned int no_speakers;
        unsigned int no_periods;
        enum aaxRenderMode setup;
        bool shared;
        bool timed;

        _connector() :
            name("default"),
            bitrate(320),
            sample_freq(44100),
            no_speakers(2),
            no_periods(2),
            setup(AAX_MODE_WRITE_STEREO),
            shared(false),
            timed(false) {};
        _connector(std::string n) :
            name(n),
            bitrate(320),
            sample_freq(44100),
            no_speakers(2),
            no_periods(2),
            setup(AAX_MODE_WRITE_STEREO),
            shared(false),
            timed(false) {};
        ~_connector() {};
    } connector_t;

    typedef struct _device
    {
        std::string name;

        unsigned int current_output_connector;
        std::vector<connector_t*> output;

        unsigned int current_input_connector;
        std::vector<connector_t*> input;
           
    } device_t;
 
    std::string product_key;
    unsigned int refresh_rate;
    unsigned int general_sample_freq;
    enum aaxRenderMode general_setup;

    int file_be_pos;
    unsigned int current_device;
    std::vector<device_t*> devices;

    void getSystemResources();
    void readConfigFiles();
    void readOldConfigSettings(void*);
    void readNewConfigSettings(void*);
    void readConnectorOutSettings(void*, unsigned, unsigned);
    void readConnectorInSettings(void*, unsigned, unsigned);
    void readConfigSettings(void*);
    void writeConfigFile();

    void displayUiConfig();
    void displayUiDevicesConfig();
    void alert(std::string);

    unsigned int FreqToIndex(unsigned int freq);
};

extern AeonWaveConfig *_mw;

#endif // AAXCONFIG_H
