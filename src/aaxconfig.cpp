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

#include <string.h>	/* for strcasestr */
#include <xml.h>

#include "aaxconfig.h"
#include "api.h"



AeonWaveConfig::AeonWaveConfig(QWidget *parent) :
    QDialog(parent)
{
    getSystemResources();   
    readConfigFiles();

    ui = new UiConfig;
    ui->setupUi(this);
    displayUiConfig();

    QPixmap pixmap(":/surround7_1.png");
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
static unsigned int _freq[MAX_FREQ] = {  8000,  11025, 16000, 22050,  32000,
                        44056,  44100,  48000, 88200, 96000, 176400, 192000};

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
    backends[current_backend].output[pos].setup = aaxRenderMode(val+1);
    changeNoSpeakers(backends[current_backend].output[pos].no_speakers/2-1);
}

void
AeonWaveConfig::changeInputSampleFreq(int val)
{
    unsigned pos = backends[current_backend].current_input_device;
    backends[current_backend].input[pos].sample_freq = _freq[val];
}

void
AeonWaveConfig::changeOutputSampleFreq(int val)
{
    unsigned pos = backends[current_backend].current_output_device;
    backends[current_backend].output[pos].sample_freq = _freq[val];
}

void
AeonWaveConfig::changeNoSpeakers(int val)
{
    unsigned pos = backends[current_backend].current_output_device;
    backends[current_backend].output[pos].no_speakers = (val+1)*2;

    pos = current_backend;
    int i = backends[pos].current_output_device;

    char *path = ":/stereo.png";
    if (backends[pos].output[i].setup != AAX_MODE_WRITE_HRTF)
    {
        if (backends[pos].output[i].no_speakers == 2) {
            path = ":/stereo.png";
        } else if (backends[pos].output[i].no_speakers == 4) {
            path = ":/quad.png";
        } else if (backends[pos].output[i].no_speakers == 6) {
            path = ":/surround6.png";
        } else {
            path = ":/surround7_1.png";
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

    val = FreqToIndex(backends[pos].input[i].sample_freq);
    ui->InputSampleFreq->setCurrentIndex(val);
}

void
AeonWaveConfig::changeOutputDevice(int val)
{
    if (val < 0) val = 0;
    backends[current_backend].current_output_device = val;

    int i = val;
    int pos = current_backend;

    val = (backends[pos].output[i].no_speakers/2)-1;
    ui->OutputSpeakers->setCurrentIndex(val);

    val = aaxRenderMode(backends[pos].output[i].setup)-1;
    ui->SpeakerSetup->setCurrentIndex(val);

    val = FreqToIndex(backends[pos].output[i].sample_freq);
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
    refresh_rate = 46;
    general_sample_freq = 48000;
    general_setup = AAX_MODE_WRITE_STEREO;
    current_backend = 0;

    unsigned max = aaxDriverGetCount(general_setup);
    for (unsigned x=0; x<max; x++)
    {
        enum aaxRenderMode mode;

        aaxConfig cfg = aaxDriverGetByPos(x, general_setup);
        if (cfg)
        {
            unsigned max_device;
            backend_t backend;
            const char *d;

            d = aaxDriverGetSetup(cfg, AAX_NAME_STRING);
            backend.current_output_device = 0;
            backend.current_input_device = 0;
            backend.name = d;

            mode = general_setup;
            max_device = aaxDriverGetDeviceCount(cfg, mode);
            if (max_device)
            {
                device_t device;
                for (unsigned y=0; y<max_device; y++)
                {
                    unsigned max_interface;
                    const char *r;

                    r = aaxDriverGetDeviceNameByPos(cfg, y, mode);
                    device.name = r;

                    max_interface = aaxDriverGetInterfaceCount(cfg, r, mode);
                    if (max_interface)
                    {
                        for (unsigned z=0; z<max_interface; z++)
                        {
                            const char *ifs;

                            ifs = aaxDriverGetInterfaceNameByPos(cfg,r,z,mode);
                            device.name = r;
                            device.name += ": ";
                            device.name += ifs;

                            device.setup = AAX_MODE_WRITE_STEREO;
                            device.sample_freq = 44100;
                            device.no_speakers = 2;

                            backend.output.push_back(device);
                        }
                    }
                    else {
                        backend.output.push_back(device);
                    }
                }
            }

            mode = AAX_MODE_READ;
            max_device = aaxDriverGetDeviceCount(cfg, mode);
            if (max_device)
            {
                device_t device;
                for (unsigned y=0; y<max_device; y++)
                {
                    unsigned max_interface; 
                    const char *r;

                    r = aaxDriverGetDeviceNameByPos(cfg, y, mode);
                    device.name = r;
 
                    max_interface = aaxDriverGetInterfaceCount(cfg, r, mode);
                    if (max_interface)
                    {
                        for (unsigned z=0; z<max_interface; z++)
                        {
                            const char *ifs;

                            ifs = aaxDriverGetInterfaceNameByPos(cfg,r,z,mode);
                            device.name = r;
                            device.name += ": ";
                            device.name += ifs;

                            device.sample_freq = 22050;

                            backend.input.push_back(device);
                        }
                    }
                    else {
                        backend.input.push_back(device);
                    }
                }
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
    
    char *path;
    void *xid;
 
    path = systemConfigFile();
    if (path)
    {
        xid = xmlOpen(path);
        if (xid)
        {
            readConfigSettings(xid);
            xmlClose(xid);
        }
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
                    for (unsigned i=0; i<backends[q].output.size(); i++) {
                        if (strcasestr(backends[q].output[i].name.c_str(), str))
                        {
                            backends[q].current_output_device = i;

                            int val = xmlNodeGetInt(output, "frequency-hz");
                            if (val) backends[q].output[i].sample_freq = val;

                            val = xmlNodeGetInt(output, "channels");
                            if (val) backends[q].output[i].no_speakers = val;

                            str = xmlNodeGetString(output, "setup");
                            if (str)
                            {
                                if (!strcasecmp(str, "stereo")) {
                                    backends[q].output[i].setup = AAX_MODE_WRITE_STEREO;
                                } else if (!strcasecmp(str, "spatial")) {
                                    backends[q].output[i].setup = AAX_MODE_WRITE_SPATIAL;
                                } else if (!strcasecmp(str, "surround")) {
                                    backends[q].output[i].setup = AAX_MODE_WRITE_SURROUND;
                                } else if (!strcasecmp(str, "hrtf")) {
                                    backends[q].output[i].setup = AAX_MODE_WRITE_HRTF;
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
                        if (strcasestr(backends[q].input[i].name.c_str(), renderer))
                        {
                            backends[q].current_input_device = i;

                            int val = xmlNodeGetInt(input, "frequency-hz");
                            if (val) backends[q].input[i].sample_freq = val;

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
        QString name = QString::fromStdString(backends[pos].output[i].name);
        ui->OutputDevice->addItem(name);
    }
    ui->OutputDevice->setCurrentIndex(idx);

    /* Input devices */
    idx = backends[pos].current_input_device;
    ui->InputDevice->clear();
    for (unsigned i=0; i<backends[pos].input.size(); i++)
    {
        QString name = QString::fromStdString(backends[pos].input[i].name);
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

        const char *username = getenv("SUDO_USER");
        if (!username) username = getenv("USER");
        if (username)
        {
            std::string to_path = from_path;
            to_path += ".old";
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

        for (unsigned i=0; i<backends.size(); i++)
        {
            file << " <backend>\n";
            file << "  <name>" << backends[i].name << "</name>\n";
            file << "  <output>\n";

            unsigned pos = backends[i].current_output_device;
            if (pos < backends[i].output.size())
            {
                file << "   <renderer>";
                file << backends[i].output[pos].name;
                file << "</renderer>\n";

                file << "   <frequency-hz>" << backends[i].output[pos].sample_freq;
                file << "</frequency-hz>\n";

                file << "   <setup>"; 
                if (backends[i].output[pos].setup == AAX_MODE_WRITE_STEREO) 
                    file << "stereo";
                else if (backends[i].output[pos].setup == AAX_MODE_WRITE_SPATIAL) 
                    file << "spatial";
                else if (backends[i].output[pos].setup == AAX_MODE_WRITE_SURROUND) 
                    file << "surround";
                else if (backends[i].output[pos].setup == AAX_MODE_WRITE_HRTF) 
                    file << "hrtf";
                file << "</setup>\n";

                file << "   <channels>"<< backends[i].output[pos].no_speakers;
                file <<"</channels>\n";
            }
            else {
                file << "   <renderer>default</renderer>\n";
            }
            file << "  </output>\n";

            file << "  <input>\n";

            pos = backends[i].current_input_device;
            if (pos < backends[i].input.size())
            {
                file << "   <renderer>";
                file << backends[i].input[pos].name;
                file << "</renderer>\n";

                file << "   <frequency-hz>" << backends[i].input[pos].sample_freq;
                file << "</frequency-hz>\n";

            }
            else {
                file << "   <renderer>default</renderer>\n";
            }

            file << "  </input>\n";
            file << " </backend>\n\n";
        }
        file << "</configuration>\n";
        file.close();

        int mode = strtol("0600", 0, 8);
        chmod(from_path.c_str(), mode);
    }
}

