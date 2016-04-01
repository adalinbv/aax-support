/*
 * Copyright (C) 2011-2015 by Adalin B.V.
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
#define AAXCONFIG_H 1

#include <vector>
#include <string>

#include <aax/AeonWave>

#include <QtGui/QDialog>
#include <QtGui/QComboBox>
#include <QShortcut>

#define CONFIG_FILE	"config.xml"

class Ui_Configuration;

class AeonWaveConfig : public QDialog
{
    Q_OBJECT

public:
    AeonWaveConfig(QWidget *parent = 0);
    ~AeonWaveConfig();

    inline std::string get_productkey() {
        return product_key;
    }

    Ui_Configuration *ui;

private slots:
    void changeShared(bool);
    void changeSetDefault(bool);
    void changeInputSetDefault(bool);
    void changeTimerDriven(bool);
    void changeRefreshRate(int val);
    void changeInputRefreshRate(int val);
    void changeSpeakerSetup(int val);
    void changeLineInSetup(int val);
    void changeInputSampleFreq(int val);
    void changeOutputSampleFreq(int val);
    void changeOutputBitrate(int val);
    void changeNoSpeakers(int val);
    void changeNoPeriods(int val);
    void changeNoInputPeriods(int val);
    void changeInputConnector(int val);
    void changeOutputConnector(int val);
    void changeMixer(int val);
    void changeDevice(int val);
    void writeConfigFile();
    void writeConfig();

    void changeProductKey(QString str);

private:
    QShortcut *saveAct;

    typedef struct _connector
    {
        std::string name;
        unsigned int bitrate;
        unsigned int refresh_rate;
        unsigned int sample_freq;
        unsigned int no_speakers;
        unsigned int no_periods;
        unsigned int setup;
        bool shared;
        bool timed;

        _connector(bool m = true) :
            name("default"),
            bitrate(320),
            refresh_rate(46),
            sample_freq(44100),
            no_speakers(2),
            no_periods(2),
            setup(m ? int(AAX_MODE_WRITE_STEREO) : AAX_TRACK_ALL),
            shared(false),
            timed(false) {};
         _connector(const char* n, bool m = true) :
            name(n),
            bitrate(320),
            refresh_rate(46),
            sample_freq(44100),
            no_speakers(2),
            no_periods(2),
            setup(m ? int(AAX_MODE_WRITE_STEREO) : AAX_TRACK_ALL),
            shared(false),
            timed(false) {};
        _connector(std::string n, bool m = true) :
            name(n),
            bitrate(320),
            refresh_rate(46),
            sample_freq(44100),
            no_speakers(2),
            no_periods(2),
            setup(m ? int(AAX_MODE_WRITE_STEREO) : AAX_TRACK_ALL),
            shared(false),
            timed(false) {};
        ~_connector() {};
    } connector_t;

    typedef struct _device
    {
        std::string name;

        unsigned int default_output_connector;
        unsigned int current_output_connector;
        std::vector<connector_t*> output;

        unsigned int default_input_connector;
        unsigned int current_input_connector;
        std::vector<connector_t*> input;
           
    } device_t;
 
    std::string product_key;
    unsigned int refresh_rate;
    unsigned int general_sample_freq;
    enum aaxRenderMode general_setup;

    int file_be_pos;
    unsigned int default_device;
    unsigned int current_device;
    unsigned int default_input_device;
    std::vector<device_t*> devices;

    void getSystemResources();
    void readConfigFiles();
    void readOldConfigSettings(void*);
    void readNewConfigSettings(void*);
    void readConnectorOutSettings(void*, unsigned, unsigned);
    void readConnectorInSettings(void*, unsigned, unsigned);
    void readConfigSettings(void*);
    void setDefaultDevice(char*, char *input = NULL);

    void displayUiConfig();
    void displayUiDevicesConfig();
    void itemsGrayOut(QComboBox*, unsigned int, unsigned int);
    void alert(QString);

    unsigned int FreqToIndex(unsigned int freq);

    inline void alert(const char *msg) {
       alert(QString(msg));
    }
    inline void alert(std::string msg) {
       alert(QString(msg.c_str()));
    }
};

extern AeonWaveConfig *_mw;

#endif // AAXCONFIG_H
