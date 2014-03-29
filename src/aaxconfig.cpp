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

#include <stdio.h>	/* std::rename */
#include <sys/stat.h>	/* std::chmod  */
#include <errno.h>

#include <iostream>
#include <fstream>

#include <xml.h>

#include <QMessageBox>

#include "aaxconfig_ui.h"
#include "aaxconfig.h"
#include "types.h"
#include "api.h"


AeonWaveConfig::AeonWaveConfig(QWidget *parent) :
    QDialog(parent),
    refresh_rate(46),
    general_sample_freq(48000),
    general_setup(AAX_MODE_WRITE_STEREO),
    file_be_pos(-1),
    current_device(0)
{
    getSystemResources();   
    readConfigFiles();

    ui = new Ui_Configuration;
    ui->setupUi(this);
    displayUiConfig();

    QPixmap pixmap_logo(":/logo.png");
    int width = ui->graphicsView->width()*2/3;
    int height = ui->graphicsView->height()*2/3;
    ui->graphicsView_logo->setAttribute(Qt::WA_TranslucentBackground);
    ui->graphicsView_logo->setPixmap(pixmap_logo.scaled(width, height,
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation));

    QPixmap pixmap(speaker_setup[STEREO].pixmap);
    ui->graphicsView->setPixmap(pixmap);

    setMinimumSize( size() );
    setMaximumSize( size() );

    connect(ui->OK, SIGNAL(accepted()), this, SLOT(writeConfig()));
    connect(ui->OK, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui->SpeakerSetup, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSpeakerSetup(int)));
    connect(ui->InputSampleFreq, SIGNAL(currentIndexChanged(int)), this, SLOT(changeInputSampleFreq(int)));
    connect(ui->InputBitrate, SIGNAL(currentIndexChanged(int)), this, SLOT(changeInputBitrate(int)));
    connect(ui->OutputSampleFreq, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOutputSampleFreq(int)));
    connect(ui->InputConnector, SIGNAL(currentIndexChanged(int)), this, SLOT(changeInputConnector(int)));
    connect(ui->OutputConnector, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOutputConnector(int)));
    connect(ui->Device, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDevice(int)));
    connect(ui->OutputSpeakers, SIGNAL(currentIndexChanged(int)), this, SLOT(changeNoSpeakers(int)));
    connect(ui->OutputPeriods, SIGNAL(currentIndexChanged(int)), this, SLOT(changeNoPeriods(int)));
    connect(ui->Timer, SIGNAL(clicked(bool)), this, SLOT(changeTimerDriven(bool)));
    connect(ui->Shared, SIGNAL(clicked(bool)), this, SLOT(changeShared(bool)));

    connect(ui->ProductKey, SIGNAL(textEdited(QString)), this, SLOT(changeProductKey(QString)));
}

AeonWaveConfig::~AeonWaveConfig()
{
    delete ui;
}

/* ------------------------------------------------------------------------- */

#define MAX_FREQ	12
static unsigned int _freq[MAX_FREQ] = {
  8000,  11025, 16000, 22050,  32000,
  44056,  44100,  48000, 88200, 96000,
 176400, 192000
};

void
AeonWaveConfig::alert(std::string msg)
{
   QMessageBox::warning(0, "AeonWave-Config", QString(msg.c_str()));
}

void
AeonWaveConfig::changeProductKey(QString str)
{
    product_key = str.toStdString();
}

void
AeonWaveConfig::changeRefreshRate(int val)
{
    refresh_rate = val;
}

void
AeonWaveConfig::changeGeneralSampleFreq(int val)
{
    general_sample_freq = _freq[val];
}

void
AeonWaveConfig::changeTimerDriven(bool val)
{
    unsigned dev = devices[current_device]->current_output_connector;
    devices[current_device]->output[dev]->timed = val;
}

void
AeonWaveConfig::changeShared(bool val)
{
    unsigned dev = devices[current_device]->current_output_connector;
    devices[current_device]->output[dev]->shared = val;
}

void
AeonWaveConfig::changeSpeakerSetup(int val)
{
    unsigned dev = devices[current_device]->current_output_connector;
    devices[current_device]->output[dev]->setup = aaxRenderMode(val+1);
    changeNoSpeakers(devices[current_device]->output[dev]->no_speakers/2-1);
}

void
AeonWaveConfig::changeInputSampleFreq(int val)
{
    unsigned dev = devices[current_device]->current_input_connector;
    devices[current_device]->input[dev]->sample_freq = _freq[val];
}

void
AeonWaveConfig::changeInputBitrate(int val)
{
    unsigned dev = devices[current_device]->current_input_connector;
    devices[current_device]->input[dev]->bitrate = (val+1)*64;
}


void
AeonWaveConfig::changeOutputSampleFreq(int val)
{
    unsigned dev = devices[current_device]->current_output_connector;
    devices[current_device]->output[dev]->sample_freq = _freq[val];
}

void
AeonWaveConfig::changeNoSpeakers(int val)
{
    unsigned be = current_device;
    unsigned dev = devices[be]->current_output_connector;

    devices[be]->output[dev]->no_speakers = (val+1)*2;

    char *path = speaker_setup[STEREO].pixmap;
    for (int sp=0; sp<MAX_SPEAKER_SETUP; sp++)
    {
       if (devices[be]->output[dev]->setup == speaker_setup[sp].setup
            && devices[be]->output[dev]->no_speakers
                 == speaker_setup[sp].no_speakers
          )
       {
           path = speaker_setup[sp].pixmap;
           break;
       }
    }
  
    QPixmap pixmap(path);
    ui->graphicsView->setPixmap(pixmap);
}

void
AeonWaveConfig::changeNoPeriods(int val)
{
    unsigned be = current_device;
    unsigned dev = devices[be]->current_output_connector;

    devices[be]->output[dev]->no_periods = val+2;
}

void
AeonWaveConfig::changeInputConnector(int val)
{
    unsigned be = current_device;
    int max_input = _MAX(devices[be]->input.size()-1, 0);
    int dev = _MINMAX(val, 0, max_input);

    devices[be]->current_input_connector = dev;

    val = FreqToIndex(devices[be]->input[dev]->sample_freq);
    ui->InputSampleFreq->setCurrentIndex(val);

    val = (devices[be]->input[dev]->bitrate/64)-1;
    ui->InputBitrate->setCurrentIndex(val);
}

void
AeonWaveConfig::changeOutputConnector(int val)
{
    unsigned be = current_device;
    int max_output = _MAX(devices[be]->output.size()-1, 0);
    int dev = _MINMAX(val, 0, max_output);

    devices[be]->current_output_connector = dev;

    ui->Timer->setChecked(devices[be]->output[dev]->timed);
    ui->Shared->setChecked(devices[be]->output[dev]->shared);

    val = (devices[be]->output[dev]->no_speakers/2)-1;
    ui->OutputSpeakers->setCurrentIndex(val);

    val = devices[be]->output[dev]->no_periods - 2;
    ui->OutputPeriods->setCurrentIndex(val);

    val = aaxRenderMode(devices[be]->output[dev]->setup)-1;
    ui->SpeakerSetup->setCurrentIndex(val);

    val = FreqToIndex(devices[be]->output[dev]->sample_freq);
    ui->OutputSampleFreq->setCurrentIndex(val);
}

void
AeonWaveConfig::changeDevice(int val)
{
    int max_device = _MAX(devices.size()-1, 0);
    current_device = _MINMAX(val, 0, max_device);
    displayUiDevicesConfig();
}

void
AeonWaveConfig::writeConfig()
{
    writeConfigFile();
    close();
}

unsigned int
AeonWaveConfig::FreqToIndex(unsigned int freq)
{
    unsigned int i = 0;
    while ( (freq > _freq[i]) && (i < MAX_FREQ) ) {
        ++i;
    }
    return i;
}

void
AeonWaveConfig::getSystemResources()
{
    if (aaxGetMajorVersion() < 2 || aaxGetMinorVersion() < 4)
    {
        alert("WARNING:\n"
              "This software only works with AeonWave 2.4.0 or later.\n");
        exit(-1);
    }

    unsigned max_driver = aaxDriverGetCount(general_setup);
    for (unsigned driver=0; driver<max_driver; driver++)
    {
        aaxConfig cfg = aaxDriverGetByPos(driver, general_setup);
        if (cfg)
        {
            enum aaxRenderMode mode;
            unsigned max_dev;
            device_t *device;
            std::string d;

            d = aaxDriverGetSetup(cfg, AAX_NAME_STRING);
            if (d == "None") //  || d == "AeonWave Loopback")
                        continue;

            mode = general_setup;
            max_dev = aaxDriverGetDeviceCount(cfg, mode);
            if (max_dev)
            {
                for (unsigned dev=0; dev<max_dev; dev++)
                {
                    unsigned max_ifs;
                    std::string r;

                    r = aaxDriverGetDeviceNameByPos(cfg, dev, mode);
                    device = new device_t;
                    device->current_output_connector = 0;
                    device->current_input_connector = 0;
                    device->name = d + " on " + r;
                    devices.push_back(device);

                    max_ifs = aaxDriverGetInterfaceCount(cfg, r.c_str(), mode);
                    if (max_ifs)
                    {
                        for (unsigned ifs=0; ifs<max_ifs; ifs++)
                        {
                            std::string i;
                            i = aaxDriverGetInterfaceNameByPos(cfg, r.c_str(),
                                                               ifs, mode);

                            connector_t *connector = new connector_t(i);
                            device->output.push_back(connector);
                        }
                    }
                }
            }

            mode = AAX_MODE_READ;
            max_dev = aaxDriverGetDeviceCount(cfg, mode);
            if (max_dev)
            {
                for (unsigned dev=0; dev<max_dev; dev++)
                {
                    std::string r, devname;
                    unsigned max_ifs;
                    size_t q;

                    r = aaxDriverGetDeviceNameByPos(cfg, dev, mode);

                    devname = d + " on " + r;
                    for (q=0; q<devices.size(); q++)
                    {
                        if (devices[q]->name == devname) {
                            break;
                        }
                    }

                    if (q == devices.size())
                    {
                        device = new device_t;
                        device->current_output_connector = 0;
                        device->current_input_connector = 0;
                        device->name = devname;
                        devices.push_back(device);
                    }
                    else
                    {
                        device = devices[q];

                        if (devname == "AeonWave on Audio Files") {
                            file_be_pos = q;
                        }
                    }

                    max_ifs = aaxDriverGetInterfaceCount(cfg, r.c_str(), mode);
                    if (max_ifs)
                    {
                        for (unsigned ifs=0; ifs<max_ifs; ifs++)
                        {
                            std::string i;
                            i = aaxDriverGetInterfaceNameByPos(cfg, r.c_str(),
                                                               ifs, mode);

                            connector_t *connector = new connector_t(i);
                            device->input.push_back(connector);
                        }
                    }
                }
            }
            aaxDriverDestroy(cfg);
        }
    }
}

void
AeonWaveConfig::readConfigFiles()
{
    /* read the system wide configuration file */
    std::string path = systemConfigFile();
    if (!path.empty())
    {
        void *xid = xmlOpen(path.c_str());
        if (xid)
        {
            readConfigSettings(xid);
            xmlClose(xid);
        }
    }

     /* read the user configurstion file */
    path = userConfigFile();
    if (!path.empty())
    {
        void *xid = xmlOpen(path.c_str());
        if (xid)
        {
            readConfigSettings(xid);
            xmlClose(xid);
        }
    }
}

void
AeonWaveConfig::readOldConfigSettings(void* xcid)
{
    if (xcid)
    {
        void* xbid = xmlMarkId(xcid);
        unsigned num = xmlNodeGetNum(xbid, "backend");
        for (unsigned xbe=0; xbe<num; xbe++)
        {
            xmlNodeGetPos(xcid, xbid, "backend", xbe);

            void* output = xmlNodeGet(xbid, "output");
            if (output)
            {
                char *name = xmlNodeGetString(xbid, "name");
                char *renderer = xmlNodeGetString(output, "renderer");
                if (name && renderer)
                {
                    cfgBackendDriverToDeviceConnector(&name, &renderer);

                    bool found = false;
                    unsigned be = 0;
                    for (be=0; be<devices.size(); be++)
                    {
                        if (!strcasecmp(devices[be]->name.c_str(), name)) 
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found) continue;

                    for (unsigned dev=0; dev<devices[be]->output.size(); dev++)
                    {
                        size_t found = devices[be]->output[dev]->name.find(renderer);
                        if (found != std::string::npos)
                        {
                            devices[be]->current_output_connector = 0;
                            readConnectorOutSettings(output, be, dev);
                            break;
                        }
                    }
                }
                xmlFree(output);
                xmlFree(renderer);
                xmlFree(name);
            }

            void* input = xmlNodeGet(xbid, "input");
            if (input)
            {
                char *name = xmlNodeGetString(xbid, "name");
                char *renderer = xmlNodeGetString(input, "renderer");
                if (name && renderer)
                {
                    cfgBackendDriverToDeviceConnector(&name, &renderer);

                    bool found = false;
                    unsigned be = 0;
                    for (unsigned be=0; be<devices.size(); be++)
                    {
                        if (!strcasecmp(devices[be]->name.c_str(), name))
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found) continue;

                    for (unsigned dev=0; dev<devices[be]->input.size(); dev++)
                    {
                        size_t found;

                        found = devices[be]->input[dev]->name.find(renderer);
                        if (found != std::string::npos)
                        {
                            devices[be]->current_input_connector = 0;
                            readConnectorInSettings(input, be, dev);
                            break;
                        }
                    }
                }
                xmlFree(input);
                xmlFree(renderer);
                xmlFree(name);
            }
        }
        xmlFree(xbid);
    }
}

void
AeonWaveConfig::readConfigSettings(void* xid)
{
    void *xcid = xmlNodeGet(xid, "/configuration");
    if (xcid)
    {
        char *str = xmlNodeGetString(xcid, "product-key");
        if (str) product_key = str;

        void *xoid = xmlNodeGet(xcid, "output");
        if (xoid)
        {
            int i = xmlNodeGetInt(xoid, "frequency-hz");
            if (i) general_sample_freq = i;

            i = xmlNodeGetInt(xoid, "interval-hz");
            if (i) refresh_rate = i;

            char *setup = xmlNodeGetString(xoid, "setup");
            if (setup)
            {
                if (!strcasecmp(setup, "stereo")) {
                    general_setup = AAX_MODE_WRITE_STEREO;
                } else if (!strcasecmp(setup, "spatial")) {
                    general_setup = AAX_MODE_WRITE_SPATIAL;
                } else if (!strcasecmp(setup, "surround")) {
                    general_setup = AAX_MODE_WRITE_SURROUND;
                } else if (!strcasecmp(setup, "hrtf")) {
                    general_setup = AAX_MODE_WRITE_HRTF;
                }
                xmlFree(setup);
            }
            xmlFree(xoid);
        }

        float v = (float)xmlNodeGetDouble(xcid, "version");
        if (v < 0.0f || v > 2.9f) {
         alert("WARNING:\n"
               "Incompattible configuration file version, skipping.\n");
        }
        else if (v < 2.0f) {
           readOldConfigSettings(xcid);
        }
        else {
            readNewConfigSettings(xcid);
        }
        xmlFree(xcid);
    }
}

void
AeonWaveConfig::readNewConfigSettings(void* xcid)
{
    if (xcid)
    {
        void *xdid = xmlMarkId(xcid);
        unsigned dev_num = xmlNodeGetNum(xdid, "device");
        for (unsigned int xdev=0; xdev<dev_num; xdev++)
        {
            xmlNodeGetPos(xcid, xdid, "device", xdev);

            void *xiid = xmlMarkId(xdid);
            unsigned int con_num = xmlNodeGetNum(xiid, "connector");
            for (unsigned int con=0; con<con_num; con++)
            {
                xmlNodeGetPos(xdid, xiid, "connector", con);

                char *backend = xmlAttributeGetString(xdid, "backend");
                char *driver = xmlAttributeGetString(xdid, "name");

                std::string device = backend + std::string(" on ") + driver;
                xmlFree(driver);
                xmlFree(backend);

                bool found = false;
                unsigned be = 0;
                for (be=0; be<devices.size(); be++)
                {
                    if (!strcasecmp(devices[be]->name.c_str(), device.c_str()))
                    {
                        found = true;
                        break;
                    }
                }
                if (found)
                {
                    char *ifs = xmlAttributeGetString(xiid, "name");
                    unsigned dev;

                    if (!xmlAttributeCompareString(xiid, "type", "out"))
                    {
                        for (dev=0; dev<devices[be]->output.size(); dev++)
                        {
                            size_t found;
    
                            found = devices[be]->output[dev]->name.find(ifs);
                            if (found != std::string::npos)
                            {
                                devices[be]->current_output_connector = 0;
                                readConnectorOutSettings(xiid, be, dev);
                                break;
                            }
                        }
                    }
                    else if (!xmlAttributeCompareString(xiid, "type", "in"))
                    {
                        for (dev=0; dev<devices[be]->input.size(); dev++)
                        {
                            size_t found;

                            found = devices[be]->input[dev]->name.find(ifs);
                            if (found != std::string::npos)
                            {
                                devices[be]->current_input_connector = 0;
                                readConnectorInSettings(xiid, be, dev);
                                break;
                            }
                        }
                    }
                    xmlFree(ifs);
                }
            }
            xmlFree(xiid);
        }
        xmlFree(xdid);
    }
}


void
AeonWaveConfig::readConnectorOutSettings(void *xiid, unsigned be, unsigned dev)
{
    devices[be]->output[dev]->timed = xmlNodeGetBool(xiid, "timed");
    devices[be]->output[dev]->shared = xmlNodeGetBool(xiid, "shared");

    int val = xmlNodeGetInt(xiid, "frequency-hz");
    if (val) {
        devices[be]->output[dev]->sample_freq = val;
    }
    val = xmlNodeGetInt(xiid, "channels");
    if (val) {
        devices[be]->output[dev]->no_speakers = val;
    }

    val = xmlNodeGetInt(xiid, "periods");
    if (val) {
        devices[be]->output[dev]->no_periods = val;
    }

    char *setup = xmlNodeGetString(xiid, "setup");
    if (setup)
    {
        if (!strcasecmp(setup, "stereo")) {
            devices[be]->output[dev]->setup = AAX_MODE_WRITE_STEREO;
        }
        else if (!strcasecmp(setup, "spatial")) {
            devices[be]->output[dev]->setup = AAX_MODE_WRITE_SPATIAL;
        }
        else if (!strcasecmp(setup, "surround")) {
            devices[be]->output[dev]->setup = AAX_MODE_WRITE_SURROUND;
        }
        else if (!strcasecmp(setup, "hrtf")) {
            devices[be]->output[dev]->setup = AAX_MODE_WRITE_HRTF;
        }
        xmlFree(setup);
    }
}

void
AeonWaveConfig::readConnectorInSettings(void *xiid, unsigned be, unsigned dev)
{
    int val = xmlNodeGetInt(xiid, "frequency-hz");
    if (val) {
        devices[be]->input[dev]->sample_freq = val;
    }

    val = xmlNodeGetInt(xiid, "bitrate");
    if (val) {
        devices[be]->input[dev]->bitrate = val;
    }
}

void
AeonWaveConfig::displayUiDevicesConfig()
{
    unsigned be = current_device;
    int idx;

    /* Output connectors */
    idx = devices[be]->current_output_connector;
    ui->OutputConnector->clear();
    for (unsigned i=0; i<devices[be]->output.size(); i++)
    {
        QString name = QString::fromStdString(devices[be]->output[i]->name);
        ui->OutputConnector->addItem(name);
    }
    ui->OutputConnector->setCurrentIndex(idx);
    changeOutputConnector(idx);

    /* Input connectors */
    idx = devices[be]->current_input_connector;
    ui->InputConnector->clear();
    for (unsigned i=0; i<devices[be]->input.size(); i++)
    {
        QString name = QString::fromStdString(devices[be]->input[i]->name);
        ui->InputConnector->addItem(name);
    }
    ui->InputConnector->setCurrentIndex(idx);
    changeInputConnector(idx);

    if (current_device == (unsigned)file_be_pos) {
        ui->InputBitrate->setEnabled(true);
    } else {
        ui->InputBitrate->setEnabled(false);
    }
}

void
AeonWaveConfig::displayUiConfig()
{
    /* General */
//  ui->ProductKey->setText( QString::fromStdString(product_key) );
    std::string hidden;
    hidden.assign(product_key.size(), '*');
    ui->ProductKey->setText( QString::fromStdString(hidden) );

    ui->GeneralSampleFreq->setCurrentIndex( FreqToIndex(general_sample_freq) );
    ui->RefreshRate->setValue( refresh_rate );

    /* Devices */
    ui->Device->clear();
    for(unsigned be=0; be<devices.size(); be++)
    {
        QString name = QString::fromStdString(devices[be]->name);
        ui->Device->addItem(name);
    }
    ui->Device->setCurrentIndex(current_device);

    displayUiDevicesConfig();
}

void
AeonWaveConfig::writeConfigFile()
{
    std::string from_path = userConfigFile();
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
            alert("Error writing to file: "+from_path+"\n\r"+strerror(errno));
            file.close();
            return;
        }

        file << "<?xml version=\"1.0\"?>" << std::endl << std::endl;
        file << "<configuration>" << std::endl;
        file << " <version>2.0</version>" << std::endl;

        file << " <product-key>" << product_key;
        file << "</product-key>" << std::endl;

        file << " <output>" << std::endl;
        file << "  <frequency-hz>"<< general_sample_freq;
        file << "</frequency-hz>" << std::endl;

        file << "  <interval-hz>" << refresh_rate;
        file << "</interval-hz>" << std::endl;

        file << "  <setup>";
        if (general_setup == AAX_MODE_WRITE_STEREO) file << "stereo";
        else if (general_setup == AAX_MODE_WRITE_SPATIAL) file << "spatial";
        else if (general_setup == AAX_MODE_WRITE_SURROUND) file << "surround";
        else if (general_setup == AAX_MODE_WRITE_HRTF) file << "hrtf";
        file << "</setup>" << std::endl;
        file << "  <head>" << std::endl;
        file << "   <side-delay-sec>64e-5</side-delay-sec>" << std::endl;
        file << "   <forward-delay-sec>9e-5</forward-delay-sec>" << std::endl;
        file << "   <up-delay-sec>25e-5</up-delay-sec>" << std::endl;
        file << "   <up-offset-sec>1e-4</up-offset-sec>" << std::endl;
        file << "  </head>" << std::endl;
        file << " </output>" << std::endl << std::endl;

        for (unsigned be=0; be<devices.size(); be++)
        {
            char *device = strdup(devices[be]->name.c_str());
            char *ptr;

            ptr = strstr(device, " on ");
            if (ptr)
            {
                *ptr = 0;
                ptr += strlen(" on ");
            }

            file << " <device backend=\"" << device << "\"";
            file << " name=\"" << ptr << "\">" << std::endl;;
            free(device);

            for (unsigned dev=0; dev<devices[be]->output.size(); dev++)
            {
                file << "  <connector name=\"" << devices[be]->output[dev]->name;
                file << "\" type=\"out\">" << std::endl;

                file << "   <shared>" << (devices[be]->output[dev]->shared ? "true" : "false");
                file << "</shared>" << std::endl;

                file << "   <timed>" << (devices[be]->output[dev]->timed ? "true" : "false");
                file << "</timed>" << std::endl;

                file << "   <setup>"; 
                switch(devices[be]->output[dev]->setup)
                {
                case AAX_MODE_WRITE_SPATIAL:
                    file << "spatial";
                    break;
                case AAX_MODE_WRITE_SURROUND:
                    file << "surround";
                    break;
                case AAX_MODE_WRITE_HRTF:
                    file << "hrtf";
                    break;
                case AAX_MODE_WRITE_STEREO:
                default:
                    file << "stereo";
                    break;
                }
                file << "</setup>" << std::endl;

                file << "   <channels>";
                file << devices[be]->output[dev]->no_speakers;
                file <<"</channels>" << std::endl;

                file << "   <periods>";
                file << devices[be]->output[dev]->no_periods;
                file <<"</periods>" << std::endl;

                file << "   <frequency-hz>";
                file << devices[be]->output[dev]->sample_freq;
                file << "</frequency-hz>" << std::endl;

                for (unsigned sp=0; sp<MAX_SPEAKER_SETUP; sp++)
                {
                   if (devices[be]->output[dev]->setup
                           == speaker_setup[sp].setup 
                       &&  devices[be]->output[dev]->no_speakers
                               == speaker_setup[sp].no_speakers
                      )
                   {
                       for (unsigned n=0; n<speaker_setup[sp].no_speakers; n++)
                       {
                           file << "    <speaker n=\"" << n << "\">" << std::endl;
                           file << "     <channel>";
                           file << speaker_setup[sp].speaker[n].channel;
                           file << "</channel>" << std::endl;
                           file << "     <pos-x>";
                           file << speaker_setup[sp].speaker[n].pos[0];
                           file << "</pos-x>" << std::endl;
                           file << "     <pos-y>";
                           file << speaker_setup[sp].speaker[n].pos[1];
                           file << "</pos-y>" << std::endl;
                           file << "     <pos-z>";
                           file << speaker_setup[sp].speaker[n].pos[2];
                           file << "</pos-z>" << std::endl;
                           file << "    </speaker>" << std::endl;
                       }
                       break;
                    }
                }
                file << "  </connector>" << std::endl;
            }

            for(unsigned dev=0; dev<devices[be]->input.size(); dev++)
            {
                file << "  <connector name=\"" << devices[be]->input[dev]->name;
                file << "\" type=\"in\">" << std::endl;

                file << "   <shared>false</shared>" << std::endl;
                file << "   <timed>false</timed>" << std::endl;

                file << "   <frequency-hz>";
                file << devices[be]->input[dev]->sample_freq;
                file << "</frequency-hz>" << std::endl;

                if (be == (unsigned)file_be_pos)
                {
                    file << "   <bitrate>";
                    file << devices[be]->input[dev]->bitrate;
                    file << "</bitrate>" << std::endl;
                }

                file << " </connector>" << std::endl;
            }
            file << " </device>" << std::endl << std::endl;
        }
        file << "</configuration>" << std::endl;
        file.close();

#ifndef _WIN32
        int mode = strtol("0600", 0, 8);
        chmod(from_path.c_str(), mode);
#endif
    }
}

#if 0
void
AeonWaveConfig::writeOldConfigFile()
{
    std::string from_path = userConfigFile();
    if (!from_path.empty())
    {
        const char *username = getenv("SUDO_USER");
        if (!username) username = getenv("USER");
        if (username)
        {
            std::string to_path = from_path + std::string(".old");
            std::rename(from_path.c_str(), to_path.c_str());
        igned dev=0; dev<devices[be]->input.size(); dev++)
            {
                file << " <device>" << std::endl;

                file << "  <name>" << devices[be]->name;
                file << "</name>" << std::endl;

                file << "  <input>" << std::endl;
                file << "   <renderer>";
                file << devices[be]->input[dev]->name;
                file << "</renderer>" << std::endl;

                file << "   <frequency-hz>";
                file << devices[be]->input[dev]->sample_freq;
                file << "</frequency-hz>" << std::endl;

                file << "  </input>" << std::endl;
                file << " </device>" << std::endl << std::endl;
            }
        }
        file << "</configuration>" << std::endl;
        file.close();


        std::ofstream file;
        file.open(from_path.c_str());

        if (file.fail() || file.bad())
        {
            alert("Error writing to file: "+from_path+"\n\r"+strerror(errno));
            file.close();
            return;
        }

        file << "<?xml version=\"1.0\"?>" << std::endl << std::endl;
        file << "<configuration>" << std::endl;
//      file << " <version>1.1</version>" << std::endl;

        file << " <product-key>" << product_key;
        file << "</product-key>" << std::endl;

        file << " <output>" << std::endl;
        file << "  <frequency-hz>"<< general_sample_freq;
        file << "</frequency-hz>" << std::endl;

        file << "  <interval-hz>" << refresh_rate;
        file << "</interval-hz>" << std::endl;

        file << "  <setup>";
        if (general_setup == AAX_MODE_WRITE_STEREO) file << "stereo";
        else if (general_setup == AAX_MODE_WRITE_SPATIAL) file << "spatial";
        else if (general_setup == AAX_MODE_WRITE_SURROUND) file << "surround";
        else if (general_setup == AAX_MODE_WRITE_HRTF) file << "hrtf";
        file << "</setup>" << std::endl;
        file << "  <head>" << std::endl;
        file << "   <side-delay-sec>64e-5</side-delay-sec>" << std::endl;
        file << "   <forward-delay-sec>9e-5</forward-delay-sec>" << std::endl;
        file << "   <up-delay-sec>25e-5</up-delay-sec>" << std::endl;
        file << "   <up-offset-sec>1e-4</up-offset-sec>" << std::endl;
        file << "  </head>" << std::endl;
        file << " </output>" << std::endl << std::endl;

        for (unsigned be=0; be<devices.size(); be++)
        {
            for (unsigned dev=0; dev<devices[be]->output.size(); dev++)
            {
                file << " <device>" << std::endl;
                file << "  <name>" << devices[be]->name;
                file << "</name>" << std::endl;

                file << "  <output>" << std::endl;
                file << "   <renderer>";
                file << devices[be]->output[dev]->name;
                file << "</renderer>" << std::endl;

                file << "   <frequency-hz>";
                file << devices[be]->output[dev]->sample_freq;
                file << "</frequency-hz>" << std::endl;

                file << "   <setup>"; 
                switch(devices[be]->output[dev]->setup)
                {
                case AAX_MODE_WRITE_SPATIAL:
                    file << "spatial";
                    break;
                case AAX_MODE_WRITE_SURROUND:
                    file << "surround";
                    break;
                case AAX_MODE_WRITE_HRTF:
                    file << "hrtf";
                    break;
                case AAX_MODE_WRITE_STEREO:
                default:
                    file << "stereo";
                    break;
                }
                file << "</setup>" << std::endl;

                file << "   <channels>";
                file << devices[be]->output[dev]->no_speakers;
                file <<"</channels>" << std::endl;
                for (unsigned sp=0; sp<MAX_SPEAKER_SETUP; sp++)
                {
                   if (devices[be]->output[dev]->setup
                           == speaker_setup[sp].setup 
                       &&  devices[be]->output[dev]->no_speakers
                               == speaker_setup[sp].no_speakers
                      )
                   {
                       for (unsigned n=0; n<speaker_setup[sp].no_speakers; n++)
                       {
                           file << "    <speaker n=\"" << n << "\">" << std::endl;
                           file << "     <channel>";
                           file << speaker_setup[sp].speaker[n].channel;
                           file << "</channel>" << std::endl;
                           file << "     <pos-x>";
                           file << speaker_setup[sp].speaker[n].pos[0];
                           file << "</pos-x>" << std::endl;
                           file << "     <pos-y>";
                           file << speaker_setup[sp].speaker[n].pos[1];
                           file << "</pos-y>" << std::endl;
                           file << "     <pos-z>";
                           file << speaker_setup[sp].speaker[n].pos[2];
                           file << "</pos-z>" << std::endl;
                           file << "    </speaker>" << std::endl;
                       }
                       break;
                }
                file << "  </output>" << std::endl;
                file << " </device>"<< std::endl << std::endl;
            }

            for(unsigned dev=0; dev<devices[be]->input.size(); dev++)
            {
                file << " <device>" << std::endl;

                file << "  <name>" << devices[be]->name;
                file << "</name>" << std::endl;

                file << "  <input>" << std::endl;
                file << "   <renderer>";
                file << devices[be]->input[dev]->name;
                file << "</renderer>" << std::endl;

                file << "   <frequency-hz>";
                file << devices[be]->input[dev]->sample_freq;
                file << "</frequency-hz>" << std::endl;

                file << "  </input>" << std::endl;
                file << " </device>" << std::endl << std::endl;
            }
        }
        file << "</configuration>" << std::endl;
        file.close();

#ifndef _WIN32
        int mode = strtol("0600", 0, 8);
        chmod(from_path.c_str(), mode);
#endif
    }
}
#endif

bool
aaxConfigLineEdit::eventFilter(QObject* object, QEvent* event)
{
    if (_mw && _mw->ui && object == _mw->ui->ProductKey)
    {
        if (event->type() == QEvent::FocusIn) {
            setText( QString::fromStdString(_mw->get_productkey()) );
        }
        else if (event->type() == QEvent::FocusOut)
        {
            std::string hidden;
            hidden.assign(_mw->get_productkey().size(), '*');
            setText( QString::fromStdString(hidden) );
        }
    }
    return false;
}

