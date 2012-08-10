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

#include <stdio.h>	/* std::rename */
#include <sys/stat.h>	/* std::chmod  */

#include <iostream>
#include <fstream>

#include <xml.h>

#include "aaxconfig.h"
#include "api.h"



AeonWaveConfig::AeonWaveConfig(QWidget *parent) :
    QDialog(parent),
    refresh_rate(46),
    general_sample_freq(48000),
    general_setup(AAX_MODE_WRITE_STEREO),
    current_backend(0)
{
    getSystemResources();   
    readConfigFiles();

    ui = new Ui_Configuration;
    ui->setupUi(this);
    displayUiConfig();

    QPixmap pixmap(speaker_setup[STEREO].pixmap);
    ui->graphicsView->setPixmap(pixmap);

    setMinimumSize( size() );
    setMaximumSize( size() );
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
AeonWaveConfig::changeSpeakerSetup(int val)
{
    unsigned pos = backends[current_backend].current_output_device;
    backends[current_backend].output[pos]->setup = aaxRenderMode(val+1);
    changeNoSpeakers(backends[current_backend].output[pos]->no_speakers/2-1);
}

void
AeonWaveConfig::changeInputSampleFreq(int val)
{
    unsigned pos = backends[current_backend].current_input_device;
    backends[current_backend].input[pos]->sample_freq = _freq[val];
}

void
AeonWaveConfig::changeOutputSampleFreq(int val)
{
    unsigned pos = backends[current_backend].current_output_device;
    backends[current_backend].output[pos]->sample_freq = _freq[val];
}

void
AeonWaveConfig::changeNoSpeakers(int val)
{
    unsigned pos = backends[current_backend].current_output_device;
    backends[current_backend].output[pos]->no_speakers = (val+1)*2;

    pos = current_backend;
    int i = backends[pos].current_output_device;

    char *path = speaker_setup[STEREO].pixmap;
    for (int s=0; s<MAX_SPEAKER_SETUP; s++)
    {
       if (backends[pos].output[i]->setup == speaker_setup[s].setup &&
           backends[pos].output[i]->no_speakers == speaker_setup[s].no_speakers)
       {
           path = speaker_setup[s].pixmap;
           break;
       }
    }
  
    QPixmap pixmap(path);
    ui->graphicsView->setPixmap(pixmap);
}

void
AeonWaveConfig::changeInputDevice(int val)
{
    if (val < 0) val = 0;
    backends[current_backend].current_input_device = val;

    int i = val;
    int pos = current_backend;

    val = FreqToIndex(backends[pos].input[i]->sample_freq);
    ui->InputSampleFreq->setCurrentIndex(val);
}

void
AeonWaveConfig::changeOutputDevice(int val)
{
    if (val < 0) val = 0;
    backends[current_backend].current_output_device = val;

    int i = val;
    int pos = current_backend;

    val = (backends[pos].output[i]->no_speakers/2)-1;
    ui->OutputSpeakers->setCurrentIndex(val);

    val = aaxRenderMode(backends[pos].output[i]->setup)-1;
    ui->SpeakerSetup->setCurrentIndex(val);

    val = FreqToIndex(backends[pos].output[i]->sample_freq);
    ui->OutputSampleFreq->setCurrentIndex(val);
}

void
AeonWaveConfig::changeBackend(int val)
{
    if (val < 0) val = 0;
    current_backend = val;
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
    unsigned max_driver = aaxDriverGetCount(general_setup);
    for (unsigned driver=0; driver<max_driver; driver++)
    {
        aaxConfig cfg = aaxDriverGetByPos(driver, general_setup);
        if (cfg)
        {
            enum aaxRenderMode mode;
            unsigned max_dev;
            backend_t backend;
            std::string d;

            d = aaxDriverGetSetup(cfg, AAX_NAME_STRING);
            if (d == "None") //  || d == "AeonWave Loopback")
                        continue;

            backend.current_output_device = 0;
            backend.current_input_device = 0;
            backend.name = d;

            mode = general_setup;
            max_dev = aaxDriverGetDeviceCount(cfg, mode);
            if (max_dev)
            {
                for (unsigned dev=0; dev<max_dev; dev++)
                {
                    unsigned max_ifs;
                    std::string r;

                    r = aaxDriverGetDeviceNameByPos(cfg, dev, mode);

                    max_ifs = aaxDriverGetInterfaceCount(cfg, r.c_str(), mode);
                    if (max_ifs)
                    {
                        for (unsigned ifs=0; ifs<max_ifs; ifs++)
                        {
                            std::string i;
                            i = aaxDriverGetInterfaceNameByPos(cfg, r.c_str(),
                                                                   ifs, mode);
                            device_t *device;
                            device = new device_t(r + std::string(": ") + i);
                            backend.output.push_back(device);
                        }
                    }
                    else
                    {
                        device_t *device = new device_t(r);
                        backend.output.push_back(device);
                    }
                }
            }
            else {
                backend.output.push_back(new device_t);
            }

            mode = AAX_MODE_READ;
            max_dev = aaxDriverGetDeviceCount(cfg, mode);
            if (max_dev)
            {
                for (unsigned dev=0; dev<max_dev; dev++)
                {
                    unsigned max_ifs; 
                    std::string r;

                    r = aaxDriverGetDeviceNameByPos(cfg, dev, mode);
                    max_ifs = aaxDriverGetInterfaceCount(cfg, r.c_str(), mode);
                    if (max_ifs)
                    {
                        for (unsigned ifs=0; ifs<max_ifs; ifs++)
                        {
                            std::string i;
                            i = aaxDriverGetInterfaceNameByPos(cfg, r.c_str(),
                                                                   ifs, mode);
                            device_t *device;
                            device = new device_t(r + std::string(": ") + i);
                            backend.input.push_back(device);
                        }
                    }
                    else
                    {
                        device_t *device = new device_t(r);
                        backend.input.push_back(device);
                    }
                }
            }
            else {
                backend.input.push_back(new device_t);
            }
            aaxDriverDestroy(cfg);
            backends.push_back(backend);
        }
    }
}

void
AeonWaveConfig::readConfigFiles()
{
    char *path;
    void *xid;
 
    /* read the system wide configuration file */
    path = systemConfigFile();
    if (path)
    {
        xid = xmlOpen(path);
        if (xid)
        {
            readConfigSettings(xid);
            xmlClose(xid);
        }
        free(path);
    }

     /* read the user configurstion file */
    path = userConfigFile();
    if (path)
    {
        xid = xmlOpen(path);
        if (xid)
        {
            readConfigSettings(xid);
            xmlClose(xid);
        }
        free(path);
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

            str = xmlNodeGetString(xoid, "setup");
            if (str)
            {
                if (!strcasecmp(str, "stereo")) {
                    general_setup = AAX_MODE_WRITE_STEREO;
                } else if (!strcasecmp(str, "spatial")) {
                    general_setup = AAX_MODE_WRITE_SPATIAL;
                } else if (!strcasecmp(str, "surround")) {
                    general_setup = AAX_MODE_WRITE_SURROUND;
                } else if (!strcasecmp(str, "hrtf")) {
                    general_setup = AAX_MODE_WRITE_HRTF;
                }
            }
            xmlFree(xoid);
        }

        void* xbid = xmlMarkId(xcid);
        unsigned num = xmlNodeGetNum(xbid, "backend");
        for (unsigned n=0; n<num; n++)
        {
            unsigned int q;
            char *name;

            xmlNodeGetPos(xcid, xbid, "backend", n);

            name = xmlNodeGetString(xbid, "name");
            if (!name) continue;

            for (q=0; q<backends.size(); q++) {
                if (!strcasecmp(backends[q].name.c_str(), name)) break;
            }
            if (q == backends.size()) continue;

            void* output = xmlNodeGet(xbid, "output");
            if (output)
            {
                char *str;

                str = xmlNodeGetString(output, "renderer");
                if (str)
                {
                    for (unsigned i=0; i<backends[q].output.size(); i++)
                    {
                        size_t found = backends[q].output[i]->name.find(str);
                        if (found != std::string::npos)
                        {
                            backends[q].current_output_device = i;

                            int val = xmlNodeGetInt(output, "frequency-hz");
                            if (val) backends[q].output[i]->sample_freq = val;

                            val = xmlNodeGetInt(output, "channels");
                            if (val) backends[q].output[i]->no_speakers = val;

                            str = xmlNodeGetString(output, "setup");
                            if (str)
                            {
                                if (!strcasecmp(str, "stereo")) {
                                    backends[q].output[i]->setup = AAX_MODE_WRITE_STEREO;
                                } else if (!strcasecmp(str, "spatial")) {
                                    backends[q].output[i]->setup = AAX_MODE_WRITE_SPATIAL;
                                } else if (!strcasecmp(str, "surround")) {
                                    backends[q].output[i]->setup = AAX_MODE_WRITE_SURROUND;
                                } else if (!strcasecmp(str, "hrtf")) {
                                    backends[q].output[i]->setup = AAX_MODE_WRITE_HRTF;
                                }
                                free(str);
                            }
                            break;
                        }
                    }
                }
                xmlFree(output);
            }

            void* input = xmlNodeGet(xbid, "input");
            if (input)
            {
                char *renderer = xmlNodeGetString(input, "renderer");
                if (renderer)
                {
                    for (unsigned i=0; i<backends[q].input.size(); i++)
                    {
                        size_t found = backends[q].input[i]->name.find(renderer);
                        if (found != std::string::npos)
                        {
                            backends[q].current_input_device = i;

                            int val = xmlNodeGetInt(input, "frequency-hz");
                            if (val) backends[q].input[i]->sample_freq = val;

                            break;
                        }
                    }
                    free(renderer);
                }
                xmlFree(input);
            }
            xmlFree(name);
        }
        xmlFree(xbid);
        xmlFree(xcid);
    }
}

void
AeonWaveConfig::displayUiDevicesConfig()
{
    unsigned pos = current_backend;
    int idx;

    /* Output devices */
    idx = backends[pos].current_output_device;
    ui->OutputDevice->clear();
    for (unsigned i=0; i<backends[pos].output.size(); i++)
    {
        QString name = QString::fromStdString(backends[pos].output[i]->name);
        ui->OutputDevice->addItem(name);
    }
    ui->OutputDevice->setCurrentIndex(idx);

    /* Input devices */
    idx = backends[pos].current_input_device;
    ui->InputDevice->clear();
    for (unsigned i=0; i<backends[pos].input.size(); i++)
    {
        QString name = QString::fromStdString(backends[pos].input[i]->name);
        ui->InputDevice->addItem(name);
    }
    ui->InputDevice->setCurrentIndex(idx);
}

void
AeonWaveConfig::displayUiConfig()
{
    /* General */
    ui->ProductKey->setText( QString::fromStdString(product_key) );
    ui->GeneralSampleFreq->setCurrentIndex( FreqToIndex(general_sample_freq) );
    ui->RefreshRate->setValue( refresh_rate );

    /* Backends */
    ui->Backend->clear();
    for(unsigned i=0; i<backends.size(); i++)
    {
        QString name = QString::fromStdString(backends[i].name);
        ui->Backend->addItem(name);
    }
    ui->Backend->setCurrentIndex(current_backend);

    displayUiDevicesConfig();
}

void
AeonWaveConfig::writeConfigFile()
{

    char *path = userConfigFile();
    if (path)
    {
        std::string from_path = path;
        free(path);

        const char *username = getenv("SUDO_USER");
        if (!username) username = getenv("USER");
        if (username)
        {
            std::string to_path = from_path + std::string(".old");
            std::rename(from_path.c_str(), to_path.c_str());
        }

        std::ofstream file;
        file.open(from_path.c_str());
        file << "<?xml version=\"1.0\"?>\n\n";
        file << "<configuration>\n";
//      file << " <version>1.1</version>\n";
        file << " <product-key>" << product_key << "</product-key>\n";
        file << " <output>\n";
        file << "  <frequency-hz>"<< general_sample_freq << "</frequency-hz>\n";
        file << "  <interval-hz>" << refresh_rate << "</interval-hz>\n";
        file << "  <setup>";
        if (general_setup == AAX_MODE_WRITE_STEREO) file << "stereo";
        else if (general_setup == AAX_MODE_WRITE_SPATIAL) file << "spatial";
        else if (general_setup == AAX_MODE_WRITE_SURROUND) file << "surround";
        else if (general_setup == AAX_MODE_WRITE_HRTF) file << "hrtf";
        file << "</setup>\n";
        file << " </output>\n\n";

        for (unsigned be=0; be<backends.size(); be++)
        {
            for (unsigned dev=0; dev<backends[be].output.size(); dev++)
            {
                file << " <backend>\n";
                file << "  <name>" << backends[be].name << "</name>\n";
                file << "  <output>\n";

                file << "   <renderer>";
                file << backends[be].output[dev]->name;
                file << "</renderer>\n";

                file << "   <frequency-hz>";
                file << backends[be].output[dev]->sample_freq;
                file << "</frequency-hz>\n";

                file << "   <setup>"; 
                switch(backends[be].output[dev]->setup)
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
                file << "</setup>\n";

                file << "   <channels>";
                file << backends[be].output[dev]->no_speakers;
                file <<"</channels>\n";

                for (unsigned s=0; s<MAX_SPEAKER_SETUP; s++)
                {
                   if (backends[be].output[dev]->setup
                           == speaker_setup[s].setup 
                       &&  backends[be].output[dev]->no_speakers
                               == speaker_setup[s].no_speakers
                      )
                   {
                       for (unsigned q=0; q<speaker_setup[s].no_speakers; q++)
                       {
                           file << "    <speaker n=\"" << q << "\">\n";
                           file << "     <channel>";
                           file << speaker_setup[s].speaker[q].channel;
                           file << "</channel>\n";
                           file << "     <pos-x>";
                           file << speaker_setup[s].speaker[q].pos[0];
                           file << "</pos-x>\n";
                           file << "     <pos-y>";
                           file << speaker_setup[s].speaker[q].pos[1];
                           file << "</pos-y>\n";
                           file << "     <pos-z>";
                           file << speaker_setup[s].speaker[q].pos[2];
                           file << "</pos-z>\n";
                           file << "    </speaker>\n";
                       }
                       break;
                   }
                }
                file << "  </output>\n";
                file << " </backend>\n\n";
            }

            for(unsigned dev=0; dev<backends[be].input.size(); dev++)
            {
                file << " <backend>\n";
                file << "  <name>" << backends[be].name << "</name>\n";
                file << "  <input>\n";

                file << "   <renderer>";
                file << backends[be].input[dev]->name;
                file << "</renderer>\n";

                file << "   <frequency-hz>";
                file << backends[be].input[dev]->sample_freq;
                file << "</frequency-hz>\n";

                file << "  </input>\n";
                file << " </backend>\n\n";
            }
        }
        file << "</configuration>\n";
        file.close();

#ifndef _WIN32
        int mode = strtol("0600", 0, 8);
        chmod(from_path.c_str(), mode);
#endif
    }
}

