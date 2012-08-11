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
#include <errno.h>

#include <iostream>
#include <fstream>

#include <xml.h>

#include <QMessageBox>

#include "aaxconfig.h"
#include "types.h"
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

void
AeonWaveConfig::alert(std::string msg)
{
   QMessageBox::warning(0, "AeonWave-Config", QString(msg.c_str()));
}


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
    unsigned dev = backends[current_backend]->current_output_device;

    backends[current_backend]->output[dev]->setup = aaxRenderMode(val+1);
    changeNoSpeakers(backends[current_backend]->output[dev]->no_speakers/2-1);
}

void
AeonWaveConfig::changeInputSampleFreq(int val)
{
    unsigned dev = backends[current_backend]->current_input_device;

    backends[current_backend]->input[dev]->sample_freq = _freq[val];
}

void
AeonWaveConfig::changeOutputSampleFreq(int val)
{
    unsigned dev = backends[current_backend]->current_output_device;

    backends[current_backend]->output[dev]->sample_freq = _freq[val];
}

void
AeonWaveConfig::changeNoSpeakers(int val)
{
    unsigned be = current_backend;
    unsigned dev = backends[be]->current_output_device;

    backends[be]->output[dev]->no_speakers = (val+1)*2;

    char *path = speaker_setup[STEREO].pixmap;
    for (int sp=0; sp<MAX_SPEAKER_SETUP; sp++)
    {
       if (backends[be]->output[dev]->setup == speaker_setup[sp].setup
            && backends[be]->output[dev]->no_speakers
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
AeonWaveConfig::changeInputDevice(int val)
{
    unsigned be = current_backend;
    int max_input = _MAX(backends[be]->input.size()-1, 0);
    int dev = _MINMAX(val, 0, max_input);

    backends[be]->current_input_device = dev;

    val = FreqToIndex(backends[be]->input[dev]->sample_freq);
    ui->InputSampleFreq->setCurrentIndex(val);
}

void
AeonWaveConfig::changeOutputDevice(int val)
{
    unsigned be = current_backend;
    int max_output = _MAX(backends[be]->output.size()-1, 0);
    int dev = _MINMAX(val, 0, max_output);

    backends[be]->current_output_device = dev;

    val = (backends[be]->output[dev]->no_speakers/2)-1;
    ui->OutputSpeakers->setCurrentIndex(val);

    val = aaxRenderMode(backends[be]->output[dev]->setup)-1;
    ui->SpeakerSetup->setCurrentIndex(val);

    val = FreqToIndex(backends[be]->output[dev]->sample_freq);
    ui->OutputSampleFreq->setCurrentIndex(val);
}

void
AeonWaveConfig::changeBackend(int val)
{
    int max_backend = _MAX(backends.size()-1, 0);
    current_backend = _MINMAX(val, 0, max_backend);
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
            backend_t *backend;
            std::string d;

            d = aaxDriverGetSetup(cfg, AAX_NAME_STRING);
            if (d == "None") //  || d == "AeonWave Loopback")
                        continue;

            backend = new backend_t;
            backend->current_output_device = 0;
            backend->current_input_device = 0;
            backend->name = d;

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
                            backend->output.push_back(device);
                        }
                    }
                    else
                    {
                        device_t *device = new device_t(r);
                        backend->output.push_back(device);
                    }
                }
            }
            else {
                backend->output.push_back(new device_t);
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
                            backend->input.push_back(device);
                        }
                    }
                    else
                    {
                        device_t *device = new device_t(r);
                        backend->input.push_back(device);
                    }
                }
            }
            else {
                backend->input.push_back(new device_t);
            }
            aaxDriverDestroy(cfg);
            backends.push_back(backend);
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

        void* xbid = xmlMarkId(xcid);
        unsigned num = xmlNodeGetNum(xbid, "backend");
        for (unsigned xbe=0; xbe<num; xbe++)
        {
            xmlNodeGetPos(xcid, xbid, "backend", xbe);

            char *name = xmlNodeGetString(xbid, "name");
            if (!name) continue;

            bool found = false;
            unsigned be = 0;
            for (unsigned be=0; be<backends.size(); be++)
            {
                if (!strcasecmp(backends[be]->name.c_str(), name)) 
                {
                    found = true;
                    break;
                }
            }
            xmlFree(name);
            if (!found) continue;

            void* output = xmlNodeGet(xbid, "output");
            if (output)
            {
                char *renderer = xmlNodeGetString(output, "renderer");
                if (renderer)
                {
                    for (unsigned dev=0; dev<backends[be]->output.size(); dev++)
                    {
                        size_t found = backends[be]->output[dev]->name.find(renderer);
                        if (found != std::string::npos)
                        {
                            backends[be]->current_output_device = dev;

                            int val = xmlNodeGetInt(output, "frequency-hz");
                            if (val) {
                                backends[be]->output[dev]->sample_freq = val;
                            }

                            val = xmlNodeGetInt(output, "channels");
                            if (val) {
                                backends[be]->output[dev]->no_speakers = val;
                            }

                            char *setup = xmlNodeGetString(output, "setup");
                            if (setup)
                            {
                                if (!strcasecmp(setup, "stereo"))
                                {
                                    backends[be]->output[dev]->setup
                                        = AAX_MODE_WRITE_STEREO;
                                }
                                else if (!strcasecmp(setup, "spatial"))
                                {
                                    backends[be]->output[dev]->setup
                                        = AAX_MODE_WRITE_SPATIAL;
                                }
                                else if (!strcasecmp(setup, "surround"))
                                {
                                    backends[be]->output[dev]->setup
                                        = AAX_MODE_WRITE_SURROUND;
                                }
                                else if (!strcasecmp(setup, "hrtf"))
                                {
                                    backends[be]->output[dev]->setup
                                        = AAX_MODE_WRITE_HRTF;
                                }
                                xmlFree(setup);
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
                    for (unsigned dev=0; dev<backends[be]->input.size(); dev++)
                    {
                        size_t found;

                        found = backends[be]->input[dev]->name.find(renderer);
                        if (found != std::string::npos)
                        {
                            backends[be]->current_input_device = dev;

                            int val = xmlNodeGetInt(input, "frequency-hz");
                            if (val) {
                                backends[be]->input[dev]->sample_freq = val;
                            }
                            break;
                        }
                    }
                    xmlFree(renderer);
                }
                xmlFree(input);
            }
        }
        xmlFree(xbid);
        xmlFree(xcid);
    }
}

void
AeonWaveConfig::displayUiDevicesConfig()
{
    unsigned be = current_backend;
    int idx;

    /* Output devices */
    idx = backends[be]->current_output_device;
    ui->OutputDevice->clear();
    for (unsigned i=0; i<backends[be]->output.size(); i++)
    {
        QString name = QString::fromStdString(backends[be]->output[i]->name);
        ui->OutputDevice->addItem(name);
    }
    ui->OutputDevice->setCurrentIndex(idx);

    /* Input devices */
    idx = backends[be]->current_input_device;
    ui->InputDevice->clear();
    for (unsigned i=0; i<backends[be]->input.size(); i++)
    {
        QString name = QString::fromStdString(backends[be]->input[i]->name);
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
    for(unsigned be=0; be<backends.size(); be++)
    {
        QString name = QString::fromStdString(backends[be]->name);
        ui->Backend->addItem(name);
    }
    ui->Backend->setCurrentIndex(current_backend);

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
            for (unsigned dev=0; dev<backends[be]->output.size(); dev++)
            {
                file << " <backend>\n";
                file << "  <name>" << backends[be]->name << "</name>\n";
                file << "  <output>\n";

                file << "   <renderer>";
                file << backends[be]->output[dev]->name;
                file << "</renderer>\n";

                file << "   <frequency-hz>";
                file << backends[be]->output[dev]->sample_freq;
                file << "</frequency-hz>\n";

                file << "   <setup>"; 
                switch(backends[be]->output[dev]->setup)
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
                file << backends[be]->output[dev]->no_speakers;
                file <<"</channels>\n";

                for (unsigned sp=0; sp<MAX_SPEAKER_SETUP; sp++)
                {
                   if (backends[be]->output[dev]->setup
                           == speaker_setup[sp].setup 
                       &&  backends[be]->output[dev]->no_speakers
                               == speaker_setup[sp].no_speakers
                      )
                   {
                       for (unsigned n=0; n<speaker_setup[sp].no_speakers; n++)
                       {
                           file << "    <speaker n=\"" << n << "\">\n";
                           file << "     <channel>";
                           file << speaker_setup[sp].speaker[n].channel;
                           file << "</channel>\n";
                           file << "     <pos-x>";
                           file << speaker_setup[sp].speaker[n].pos[0];
                           file << "</pos-x>\n";
                           file << "     <pos-y>";
                           file << speaker_setup[sp].speaker[n].pos[1];
                           file << "</pos-y>\n";
                           file << "     <pos-z>";
                           file << speaker_setup[sp].speaker[n].pos[2];
                           file << "</pos-z>\n";
                           file << "    </speaker>\n";
                       }
                       break;
                   }
                }
                file << "  </output>\n";
                file << " </backend>\n\n";
            }

            for(unsigned dev=0; dev<backends[be]->input.size(); dev++)
            {
                file << " <backend>\n";
                file << "  <name>" << backends[be]->name << "</name>\n";
                file << "  <input>\n";

                file << "   <renderer>";
                file << backends[be]->input[dev]->name;
                file << "</renderer>\n";

                file << "   <frequency-hz>";
                file << backends[be]->input[dev]->sample_freq;
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

