/*
 * Copyright (C) 2011-2024 by Erik Hofman
 * Copyright (C) 2011-2024 by Adalin B.V.
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

#include <cstdio>	/* std::rename */
#include <sys/stat.h>	/* std::chmod  */
#include <cerrno>

#include <iostream>
#include <fstream>

#include <xml.h>

#include <QMessageBox>
#include <QStandardItemModel>

#include <base/api.h>
#include "ui_aaxconfig.h"
#include "aaxconfig.h"
#include "types.h"
#include "api.h"


AeonWaveConfig::AeonWaveConfig(QWidget *parent) :
    QDialog(parent),
    refresh_rate(90),
    general_sample_freq(44100),
    general_setup(AAX_MODE_WRITE_STEREO),
    file_be_pos(-1),
    default_device(UINT_MAX),
    current_device(0),
    default_input_device(UINT_MAX)
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

    QPixmap pixmap_input(":/microphone.png");
    ui->graphicsViewInput->setPixmap(pixmap_input);

    setMinimumSize( size() );
    setMaximumSize( size() );

    connect(ui->OK, SIGNAL(accepted()), this, SLOT(writeConfig()));
    connect(ui->OK, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui->RefreshRate, SIGNAL(valueChanged(int)), this, SLOT(changeRefreshRate(int)));
    connect(ui->InputRefreshRate, SIGNAL(valueChanged(int)), this, SLOT(changeInputRefreshRate(int)));
    QObject::connect(ui->applySettings, SIGNAL(released()), this,  SLOT(writeConfigFile()));
    QObject::connect(ui->testPlay, SIGNAL(released()), this,  SLOT(testPlay()));
    connect(ui->SpeakerSetup, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSpeakerSetup(int)));
    connect(ui->LineInSetup, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLineInSetup(int)));
    connect(ui->OutputSampleFreq, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOutputSampleFreq(int)));
    connect(ui->InputSampleFreq, SIGNAL(currentIndexChanged(int)), this, SLOT(changeInputSampleFreq(int)));
    connect(ui->OutputBitrate, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOutputBitrate(int)));
    connect(ui->InputConnector, SIGNAL(currentIndexChanged(int)), this, SLOT(changeInputConnector(int)));
    connect(ui->OutputConnector, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOutputConnector(int)));
    connect(ui->Mixer, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMixer(int)));
    connect(ui->Device, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDevice(int)));
    connect(ui->OutputSpeakers, SIGNAL(currentIndexChanged(int)), this, SLOT(changeNoSpeakers(int)));
    connect(ui->OutputPeriods, SIGNAL(currentIndexChanged(int)), this, SLOT(changeNoPeriods(int)));
    connect(ui->InputPeriods, SIGNAL(currentIndexChanged(int)), this, SLOT(changeNoInputPeriods(int)));
    connect(ui->Timer, SIGNAL(clicked(bool)), this, SLOT(changeTimerDriven(bool)));
    connect(ui->Shared, SIGNAL(clicked(bool)), this, SLOT(changeShared(bool)));
    connect(ui->SetDefault, SIGNAL(clicked(bool)), this, SLOT(changeSetDefault(bool)));
    connect(ui->InputSetDefault, SIGNAL(clicked(bool)), this, SLOT(changeInputSetDefault(bool)));

    saveAct = new QShortcut(this);
    saveAct->setKey(tr("Ctrl+S"));
    saveAct->setContext(Qt::ApplicationShortcut);
    connect(saveAct, SIGNAL(activated()), this, SLOT(writeConfigFile()));

    QFont font;
    font.setFamily(QString::fromUtf8("Open Sans"));
    font.setPointSize(9);
    font.setBold(false);
    font.setWeight(QFont::Normal);
    font.setLetterSpacing(QFont::PercentageSpacing, 95.0f);

    ui->MixerInfo->setFont(font);
    ui->MixerInfo->setLineWidth(1);
    ui->MixerInfo->setTextFormat(Qt::RichText);
    ui->MixerInfo->setAlignment(Qt::AlignLeft|Qt::AlignTop);

    ui->FiltersEffects->setFont(font);
    ui->FiltersEffects->setLineWidth(1);
    ui->FiltersEffects->setTextFormat(Qt::RichText);
    ui->FiltersEffects->setAlignment(Qt::AlignLeft|Qt::AlignTop);

//  ui->InfoScrollArea->setWidget(ui->MixerInfo);
//  ui->InfoScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->tabWidget->setCurrentIndex(2);
    changeMixer(0);
}

AeonWaveConfig::~AeonWaveConfig()
{
    delete ui;
}

/* ------------------------------------------------------------------------- */

#define MAX_FREQ	13
static unsigned int _freq[MAX_FREQ] = {
  8000,  11025, 16000, 22050,  32000, 44056,  44100,  48000, 88200, 96000,
  176400, 192000
};

void
AeonWaveConfig::alert(QString msg)
{
   QMessageBox::warning(0, "AeonWave-Config", msg);
}

void
AeonWaveConfig::changeRefreshRate(int val)
{
    unsigned dev = devices[current_device]->current_output_connector;
    devices[current_device]->output[dev]->refresh_rate = val;
}

void
AeonWaveConfig::changeInputRefreshRate(int val)
{
    unsigned dev = devices[current_device]->current_input_connector;
    devices[current_device]->input[dev]->refresh_rate = val;
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
AeonWaveConfig::changeSetDefault(bool val)
{
    if (val)
    {
       unsigned dev = devices[current_device]->current_output_connector;
       devices[current_device]->default_output_connector = dev;
       default_device = current_device;

       general_sample_freq = devices[current_device]->output[dev]->sample_freq;
       general_setup = aaxRenderMode(devices[current_device]->output[dev]->setup);
    }
    else
    {
       devices[current_device]->default_output_connector = 0;
       default_device = UINT_MAX;

       general_sample_freq = 44100;
       general_setup = AAX_MODE_WRITE_STEREO;
    }
}

void
AeonWaveConfig::changeInputSetDefault(bool val)
{
    if (val)
    {
       unsigned dev = devices[current_device]->current_input_connector;
       devices[current_device]->default_input_connector = dev;
       default_input_device = current_device;

       general_sample_freq = devices[current_device]->input[dev]->sample_freq;
       general_setup = aaxRenderMode(devices[current_device]->input[dev]->setup);
    }
    else
    {
       devices[current_device]->default_input_connector = 0;
       default_input_device = UINT_MAX;

       general_sample_freq = 44100;
       general_setup = AAX_MODE_WRITE_STEREO;
    }
}

void
AeonWaveConfig::testPlay()
{
    unsigned dev = devices[current_device]->current_output_connector;
    std::string name = devices[current_device]->name;
    if (devices[current_device]->output.size() > dev)
    {
        name.append(": ");
        name.append(devices[current_device]->output[dev]->name);
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    int res = aaxPlaySoundLogo(name.c_str());
    QApplication::restoreOverrideCursor();

    if (!res) {
        alert(aaxGetErrorString(aaxGetErrorNo()));
    }
}

void
AeonWaveConfig::changeSpeakerSetup(int val)
{
    unsigned dev = devices[current_device]->current_output_connector;
    if (devices[current_device]->output.size() > dev)
    {
        devices[current_device]->output[dev]->setup = val+1;
        changeNoSpeakers(devices[current_device]->output[dev]->no_speakers/2-1);
    }
}

void
AeonWaveConfig::changeLineInSetup(int val)
{
    static int track_dest[4] = {
        AAX_TRACK_ALL, AAX_TRACK_LEFT, AAX_TRACK_RIGHT, AAX_TRACK_MIX
    };
    unsigned dev = devices[current_device]->current_input_connector;
    devices[current_device]->input[dev]->setup = track_dest[val];
}

void
AeonWaveConfig::changeInputSampleFreq(int val)
{
    unsigned dev = devices[current_device]->current_input_connector;
    devices[current_device]->input[dev]->sample_freq = _freq[val];
}

void
AeonWaveConfig::changeOutputBitrate(int val)
{
    unsigned dev = devices[current_device]->current_input_connector;
    devices[current_device]->output[dev]->bitrate = val*64;
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
    unsigned int setup = devices[be]->output[dev]->setup;
    unsigned speakers = devices[be]->output[dev]->no_speakers;
    for (int sp=0; sp<MAX_SPEAKER_SETUP; sp++)
    {
       if (setup == speaker_setup[sp].setup &&
           speakers == speaker_setup[sp].no_speakers)
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

    if (devices[be]->output.size()) {
        devices[be]->output[dev]->no_periods = val+1;
    }
}

void
AeonWaveConfig::changeNoInputPeriods(int val)
{
    unsigned be = current_device;
    unsigned dev = devices[be]->current_input_connector;

    if (devices[be]->input.size()) {
        devices[be]->input[dev]->no_periods = val+1;
    }
}

void
AeonWaveConfig::changeInputConnector(int val)
{
    unsigned be = current_device;
    int max_input = (int)devices[be]->input.size() - 1;
    if (max_input >= 0)
    {
        int dev = _MINMAX(val, 0, max_input);

        devices[be]->current_input_connector = dev;

        if (current_device == default_input_device &&
            devices[be]->current_input_connector ==
            devices[be]->default_input_connector)
        {
           ui->InputSetDefault->setChecked(true);
        } else {
           ui->InputSetDefault->setChecked(false);
        }

        val = FreqToIndex(devices[be]->input[dev]->sample_freq);
        ui->InputSampleFreq->setCurrentIndex(val);

        val = devices[be]->input[dev]->refresh_rate;
        ui->InputRefreshRate->setValue(val);

        val = devices[be]->input[dev]->no_periods - 1;
        ui->InputPeriods->setCurrentIndex(val);

        switch(devices[be]->input[dev]->setup)
        {
        case AAX_TRACK_ALL:
            val = 0;
            break;
        case AAX_TRACK_LEFT:
            val = 1;
            break;
        case AAX_TRACK_RIGHT:
            val = 2;
            break;
        default:
            val = 3;
            break;
        }
        ui->LineInSetup->setCurrentIndex(val);
    }
}

void
AeonWaveConfig::changeOutputConnector(int val)
{
    unsigned be = current_device;
    int max_output = (int)devices[be]->output.size() - 1;
    if (max_output >= 0)
    {
        int dev = _MINMAX(val, 0, max_output);

        devices[be]->current_output_connector = dev;

        if (current_device == default_device &&
            devices[be]->current_output_connector ==
            devices[be]->default_output_connector)
        {
           ui->SetDefault->setChecked(true);
        } else {
           ui->SetDefault->setChecked(false);
        }

        ui->Timer->setChecked(devices[be]->output[dev]->timed);
        ui->Shared->setChecked(devices[be]->output[dev]->shared);

        val = (devices[be]->output[dev]->no_speakers/2)-1;
        ui->OutputSpeakers->setCurrentIndex(val);

        val = devices[be]->output[dev]->no_periods - 1;
        ui->OutputPeriods->setCurrentIndex(val);

        val = aaxRenderMode(devices[be]->output[dev]->setup)-1;
        ui->SpeakerSetup->setCurrentIndex(val);

        val = devices[be]->output[dev]->refresh_rate;
        ui->RefreshRate->setValue(val);

        val = FreqToIndex(devices[be]->output[dev]->sample_freq);
        ui->OutputSampleFreq->setCurrentIndex(val);

        val = devices[be]->output[dev]->bitrate/64;
        ui->OutputBitrate->setCurrentIndex(val);
    }
}

void
AeonWaveConfig::changeMixer(int val)
{
    unsigned be = current_device;
    int max_output = (int)devices[be]->output.size() - 1;
    if (max_output >= 0)
    {
        int dev = _MINMAX(val, 0, max_output);
        std::string name;
        QString desc;

        name = devices[be]->name.c_str();
        if (be != (unsigned)file_be_pos)
        {
            if (devices[be]->output.size() > 0)
            {
                const char *ifname = devices[be]->output[dev]->name.c_str();
                name += std::string(": ") + ifname;
            }
        }
        else {
            name += ": " + std::string(tmpDir()) + "/AeonWave.wav";
        }

        aax::AeonWave cfg(name.c_str(), AAX_MODE_WRITE_STEREO);
        if (cfg)
        {
            unsigned int x, y, min, max;
            const char *s;

            if (aax::is_valid(cfg, AAX_CONFIG)) {
               ui->label_acquire->setVisible(false);
            }

            desc = tr("<tabel>");
            cfg.set(AAX_INITIALIZED);

            s = cfg.info(AAX_NAME_STRING);
            desc += tr("<tr><td>Driver:</td>");
            desc += tr("<td colspan=\"3\">%1</td></tr>").arg(s);

            s = cfg.info(AAX_RENDERER_STRING);
            desc += tr("<tr><td>Renderer:</td>");
            desc += tr("<td colspan=\"3\">%1</td></tr>").arg(s);

            x = aax::major_version();
            y = aax::minor_version();
            s = cfg.version();
            desc += tr("<tr><td width=\"25%\">Version:</td>");
            desc += tr("<td width=\"75%\" colspan=\"3\">%1 (%2.%3)</td></tr>").arg(s).arg(x).arg(y);

            s = cfg.info(AAX_VENDOR_STRING);
            desc += tr("<tr><td>Vendor:</td>");
            desc += tr("<td colspan=\"3\">%1</td></tr>").arg(s);
            desc += tr("</table>");

            min = cfg.get(AAX_TRACKS_MIN);
            max = cfg.get(AAX_TRACKS_MAX);
            desc += tr("<tabel>");
            desc += tr("<tr><td>Supported tacks:</td>");
            desc += tr("<td>%1 - %2 tracks</td>").arg(min).arg(max);
            desc += tr("<td width=\"25%\">&nbsp;</td>");
            x = cfg.get(AAX_TIMER_MODE);
            desc += tr("<td width=\"25%\">");
            if (x) {
               desc += tr("<img src=\":/checked.png\">");
            } else {
               desc += tr("<img src=\":/unchecked.png\">");
            }
            desc += tr(" timed mode</td></tr>");


            min = cfg.get(AAX_FREQUENCY_MIN);
            max = cfg.get(AAX_FREQUENCY_MAX);
            desc += tr("<tr><td>Frequency range:</td>");
            desc += tr("<td>%1 - %2 kHz</td>").arg(min/1000.0f).arg(max/1000.0f);
            desc += tr("<td></td>");
            x = cfg.get(AAX_SHARED_MODE);
            if (x) {
               desc += tr("<td><img src=\":/checked.png\">");
            } else {
               desc += tr("<td><img src=\":/unchecked.png\">");
            }
            desc += tr(" shared mode</td></tr>");

            x = cfg.get(AAX_FREQUENCY);
            desc += tr("<tr><td>Mixer frequency:</td>");
            desc += tr("<td>%1 Hz</td>").arg(x);
            desc += tr("<td></td>");
            x = cfg.get(AAX_BATCHED_MODE);
            if (x) {
               desc += tr("<td><img src=\":/checked.png\">");
            } else {
               desc += tr("<td><img src=\":/unchecked.png\">");
            }
            desc += tr(" batched mode</td></tr>");

            x = cfg.get(AAX_REFRESH_RATE);
            if (x)
            {
               desc += tr("<tr><td>Mixer refresh rate:</td>");
               desc += tr("<td>%1 Hz</td>").arg(x);
            }

            x = cfg.get(AAX_LATENCY);
            if (x)
            {
               desc += tr("<tr><td>Mixer latency:</td>");
               desc += tr("<td>%1 ms</td>").arg(x/1000.0f, 5, 'f', 2);
            }
            desc += tr("<td></td>");
            desc += tr("<td></td></tr>");

            x = cfg.get(AAX_MONO_EMITTERS);
            desc += tr("<tr><td>Max. mono emitters:</td>");
            if (x == UINT_MAX) {
               desc += tr("<td>infinite</td>");
            } else {
               desc += tr("<td>%1</td>").arg(x);
            }
            desc += tr("<td></td>");
            desc += tr("<td></td></tr>");

            y = cfg.get(AAX_STEREO_EMITTERS);
            desc += tr("<tr><td>Max. stereo emitters:</td>");
            if (y == UINT_MAX/2) {
               desc += tr("<td>infinite</td>");
            } else {
               desc += tr("<td>%1</td>").arg(y);
            }
            desc += tr("<td></td>");
            desc += tr("<td></td></tr>");

            desc += tr("<tr><td>Max. audio-frames:</td>");
            x = cfg.get(AAX_AUDIO_FRAMES);
            if (x == UINT_MAX) {
               desc += tr("<td>infinite</td>");
            } else {
               desc += tr("<td>%1</td>").arg(x);
            }
            desc += tr("<td></td>");
            desc += tr("<td></td></tr>");

            desc += tr("</table>");

            ui->MixerInfo->setText(desc);
//          ui->MixerInfo->adjustSize();
//          ui->MixerInfo->show();

            if (aax::major_version() > 2 || (aax::major_version() == 2 && aax::minor_version() >= 5))
            {
                desc = tr("<table width=\"100%\">");
                desc += tr("<tr><td width=\"50%\">");
                desc += tr("<font><b>Supported Filters:</b></font>");
                desc += tr("</td><td width=\"50%\">");
                desc += tr("<font><b>Supported Effects:</b></font>");
                desc += tr("</td></tr>");

                int max_flt = aaxGetByType(AAX_MAX_FILTER);
                int max_eff = aaxGetByType(AAX_MAX_EFFECT);
                for (int i=1; i<_MAX(max_flt, max_eff); i++)
                {
                    desc += tr("<tr>");
                    if (i < max_flt)
                    {
                        s = cfg.info(aaxFilterType(i));
                        if (cfg.supports(s)) {
                            desc += tr("<td><img src=\":/checked.png\">");
                            desc += tr("<font>&nbsp;%1</font>").arg(s);
                            desc += tr("</td>");
                        } else {
                            desc += tr("<td><img src=\":/unchecked.png\">");
                            desc += tr("<font color=\"LightSlateGray\">&nbsp;%1</font>").arg(s);
                            desc += tr("</td>");
                        }
                    }
                    else {
                       desc += tr("<td></td>");
                    }

                    if (i < max_eff)
                    {
                        s = cfg.info(aaxEffectType(i));
                        if (cfg.supports(s)) {
                            desc += tr("<td><img src=\":/checked.png\">");
                            desc += tr("<font>&nbsp;%1</font>").arg(s);
                            desc += tr("</td>");
                        } else {
                            desc += tr("<td><img src=\":/unchecked.png\">");
                            desc += tr("<font color=\"LightSlateGray\">&nbsp;%1</font>").arg(s);
                            desc += tr("</td>");
                        }
                    }
                    desc += tr("</tr>");
                }
                desc += tr("</table>");
            }
            else {
               desc = tr("Unsupported by this version of AeonWave");
            }
            ui->FiltersEffects->setText(desc);
        }
    }
}

void
AeonWaveConfig::changeDevice(int val)
{
    unsigned int max_device = _MAX((unsigned int)devices.size()-1, 0);
    current_device = _MINMAX((unsigned int)val, 0, max_device);
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
    aax::AeonWave aax;
    int major = aax::major_version();
    int minor = aax::minor_version();

    if (major < 2 || (major == 2 && minor < 5))
    {
        alert(tr("WARNING:\n"
                 "This software only works with AeonWave 2.5.0 or later.\n"
                 "Current version is %1.%2").arg(major).arg(minor));
        exit(-1);
    }

    device_t *device;
    while (const char* d = aax.drivers(general_setup))
    {
        if (std::string(d) == "None")
            continue;

        int no_devices = 0;
        while (const char* r = aax.devices())
        {
            device = new device_t;
            device->default_output_connector = 0;
            device->current_output_connector = 0;
            device->current_input_connector = 0;
            device->name = d;

            if (!r) r = "default";
            device->name += std::string(" on ") + r;
            devices.push_back(device);
            no_devices++;

            while (const char* i = aax.interfaces())
            {
                if (*i)
                {
                    connector_t *connector = new connector_t(i);
                    device->output.push_back(connector);
                }
            }

            if (device->output.size() == 0) {
                connector_t *connector = new connector_t("default");
                device->output.push_back(connector);
            }
        }

        if (!no_devices)
        {
            device = new device_t;
            device->default_output_connector = 0;
            device->current_output_connector = 0;
            device->current_input_connector = 0;
            device->name = d;
            devices.push_back(device);
        }
    }

    while (const char* d = aax.drivers(AAX_MODE_READ))
    {
        if (std::string(d) == "None")
            continue;

        while (const char* r = aax.devices())
        {
            std::string devname = d;

            if (!r) r = "default";
            devname += std::string(" on ") + r;

            unsigned int q;
            for (q=0; q<devices.size(); q++)
            {
                if (devices[q]->name == devname) {
                    break;
                }
            }

            if (q == devices.size())
            {
                device = new device_t;
                device->default_output_connector = 0;
                device->current_output_connector = 0;
                device->current_input_connector = 0;
                device->name = devname;
                devices.push_back(device);
            }
            else
            {
                device = devices[q];

                if (devname == "AeonWave on Audio Files" ||
                    devname == "AeonWave on Audio Stream") {
                    file_be_pos = q;
                }
            }

            while (const char* i = aax.interfaces())
            {
                if (*i)
                {
                    connector_t *connector = new connector_t(i, false);
                    device->input.push_back(connector);
                }
            }

            if (device->input.size() == 0) {
                connector_t *connector = new connector_t("default", false);
                device->input.push_back(connector);
            }
        }
    }
}

void
AeonWaveConfig::readConfigFiles()
{
    /* read the system wide configuration file */
    std::string path = systemConfigFile(CONFIG_FILE);
    if (!path.empty())
    {
        xmlId *xid = xmlOpen(path.c_str());
        if (xid)
        {
            readConfigSettings(xid);
            xmlClose(xid);
        }
    }

     /* read the user configurstion file */
    path = userConfigFile(CONFIG_FILE);
    if (!path.empty())
    {
        xmlId *xid = xmlOpen(path.c_str());
        if (xid)
        {
            readConfigSettings(xid);
            xmlClose(xid);
        }
    }
}

void
AeonWaveConfig::setDefaultDevice(char *name, char *input)
{
    if (input)
    {
        char *connector = strstr(input, ": ");

        if (connector)
        {
            *connector = 0;
            connector += strlen(": ");
        }

        for (unsigned be=0; be<devices.size(); be++)
        {
            if (!strcasecmp(devices[be]->name.c_str(), input))
            {
                default_input_device = be;
                if (connector)
                {
                    for (unsigned dev=0; dev<devices[be]->input.size(); dev++)
                    {
                        const char *in = devices[be]->input[dev]->name.c_str();
                        if (!strcasecmp(in, connector))
                        {
                           devices[be]->default_input_connector = dev;
                           break;
                        }
                    }
                }
                break;
            }
        }
    }

    if (name)
    {
       char *connector = strstr(name, ": ");
       if (connector)
       {
           *connector = 0;
           connector += strlen(": ");
       }

       for (unsigned be=0; be<devices.size(); be++)
       {
           if (!strcasecmp(devices[be]->name.c_str(), name))
           {
               default_device = be;
               if (connector)
               {
                   for (unsigned dev=0; dev<devices[be]->output.size(); dev++)
                   {
                       const char *out = devices[be]->output[dev]->name.c_str();
                       if (!strcasecmp(out, connector))
                       {
                          devices[be]->default_output_connector = dev;
                          break;
                       }
                   }
               }
               break;
            }
        }
    }
}

void
AeonWaveConfig::readOldConfigSettings(xmlId* xcid)
{
    if (xcid)
    {
        xmlId *xbid = xmlMarkId(xcid);
        size_t num = xmlNodeGetNum(xbid, "backend");
        for (size_t xbe=0; xbe<num; xbe++)
        {
            xmlNodeGetPos(xcid, xbid, "backend", xbe);

            xmlId *output = xmlNodeGet(xbid, "output");
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
                            devices[be]->default_output_connector = 0;
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

            xmlId *input = xmlNodeGet(xbid, "input");
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
AeonWaveConfig::readConfigSettings(xmlId* xid)
{
    xmlId *xcid = xmlNodeGet(xid, "/configuration");
    if (xcid)
    {
        char *default_input_device = NULL;
        char *default_device = NULL;

        xmlId *xoid = xmlNodeGet(xcid, "output");
        if (xoid)
        {
            default_device = xmlNodeGetString(xoid, "device");

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
                } else if (!strcasecmp(setup, "spatial-surround")) {
                    general_setup = AAX_MODE_WRITE_SPATIAL_SURROUND;
                } else if (!strcasecmp(setup, "hrtf")) {
                    general_setup = AAX_MODE_WRITE_HRTF;
                } else if (!strcasecmp(setup, "surround")) {
                    general_setup = AAX_MODE_WRITE_SURROUND;
                }
                xmlFree(setup);
            }
            xmlFree(xoid);
        }

        xmlId *xiid = xmlNodeGet(xcid, "input");
        if (xiid)
        {
            default_input_device = xmlNodeGetString(xiid, "device");
            xmlFree(xiid);
        }

        float v = (float)xmlNodeGetDouble(xcid, "version");
        if (v < 0.0f || v > 2.9f) {
         alert(tr("WARNING:\n"
                  "Incompattible configuration file version, skipping."));
        }
        else if (v < 2.0f) {
           readOldConfigSettings(xcid);
        }
        else {
            readNewConfigSettings(xcid);
        }

        setDefaultDevice(default_device, default_input_device);
        xmlFree(default_device);
        xmlFree(xcid);
    }
}

void
AeonWaveConfig::readNewConfigSettings(xmlId* xcid)
{
    if (xcid)
    {
        xmlId *xdid = xmlMarkId(xcid);
        size_t dev_num = xmlNodeGetNum(xdid, "device");
        for (size_t xdev=0; xdev<dev_num; xdev++)
        {
            xmlNodeGetPos(xcid, xdid, "device", xdev);

            char *backend = xmlAttributeGetString(xdid, "backend");
            char *driver = xmlAttributeGetString(xdid, "name");

            std::string device = backend + std::string(" on ") + driver;
            xmlFree(driver);
            xmlFree(backend);

            xmlId *xiid = xmlMarkId(xdid);
            size_t con_num = xmlNodeGetNum(xiid, "connector");
            for (size_t con=0; con<con_num; con++)
            {
                xmlNodeGetPos(xdid, xiid, "connector", con);

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
                                devices[be]->default_output_connector = 0;
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
AeonWaveConfig::readConnectorOutSettings(xmlId *xiid, unsigned be, unsigned dev)
{
    devices[be]->output[dev]->timed = xmlNodeGetBool(xiid, "timed");
    devices[be]->output[dev]->shared = xmlNodeGetBool(xiid, "shared");

    int val = xmlNodeGetInt(xiid, "frequency-hz");
    if (val >= 4000 && val <= 192000) {
        devices[be]->output[dev]->sample_freq = val;
    }

    val = xmlNodeGetInt(xiid, "interval-hz");
    if (!val) val = refresh_rate;
    devices[be]->output[dev]->refresh_rate = val;

    val = xmlNodeGetInt(xiid, "bitrate");
    if (val >= 0 && val <= 320) {
        devices[be]->output[dev]->bitrate = val;
    }

    val = xmlNodeGetInt(xiid, "channels");
    if (val > 0 && val <= 8) {
        devices[be]->output[dev]->no_speakers = val;
    }

    val = xmlNodeGetInt(xiid, "periods");
    if (val > 0 && val <= 16) {
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
        else if (!strcasecmp(setup, "spatial-surround")) {
            devices[be]->output[dev]->setup = AAX_MODE_WRITE_SPATIAL_SURROUND;
        }
        else if (!strcasecmp(setup, "hrtf")) {
            devices[be]->output[dev]->setup = AAX_MODE_WRITE_HRTF;
        }
        else if (!strcasecmp(setup, "surround")) {
            devices[be]->output[dev]->setup = AAX_MODE_WRITE_SURROUND;
        }
        xmlFree(setup);
    }
}

void
AeonWaveConfig::readConnectorInSettings(xmlId *xiid, unsigned be, unsigned dev)
{
    int val = xmlNodeGetInt(xiid, "frequency-hz");
    if (val) {
        devices[be]->input[dev]->sample_freq = val;
    }

    val = xmlNodeGetInt(xiid, "interval-hz");
    if (!val) val = refresh_rate;
    devices[be]->input[dev]->refresh_rate = val;

    val = xmlNodeGetInt(xiid, "periods");
    if (val > 0 && val <= 16) {
        devices[be]->input[dev]->no_periods = val;
    }

    char *setup = xmlNodeGetString(xiid, "setup");
    if (setup)
    {
        if (!strcasecmp(setup, "stereo")) {
            devices[be]->input[dev]->setup = AAX_TRACK_ALL;
        }
        else if (!strcasecmp(setup, "left")) {
            devices[be]->input[dev]->setup = AAX_TRACK_LEFT;
        }
        else if (!strcasecmp(setup, "right")) {
            devices[be]->input[dev]->setup = AAX_TRACK_RIGHT;
        }
        else if (!strcasecmp(setup, "mix")) {
            devices[be]->input[dev]->setup = AAX_TRACK_MIX;
        }
        xmlFree(setup);
    }
}

void
AeonWaveConfig::displayUiDevicesConfig()
{
    unsigned be = current_device;
    const char *devname;
    std::string name;
    bool found;
    int idx;

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

    idx = devices[be]->current_input_connector;
    devname = devices[be]->name.c_str();
    name = devname;

    if (devices[be]->input.size() > 0)
    {
        const char *ifname = devices[be]->input[idx]->name.c_str();
        name += std::string(": ") + ifname;
    }

    aax::AeonWave cfg(name.c_str(), AAX_MODE_READ);
    if (cfg)
    {
       int min, max;

       cfg.set(AAX_INITIALIZED);

       min = cfg.get(AAX_FREQUENCY_MIN);
       max = cfg.get(AAX_FREQUENCY_MAX);
       itemsGrayOut(ui->InputSampleFreq, min, max);

       min = cfg.get(AAX_PERIODS_MIN);
       max = cfg.get(AAX_PERIODS_MAX);
       itemsGrayOut(ui->InputPeriods, min, max);

       found = true;
    }
    else
    {
        found = false;
        alert(tr("Input device unavailable or busy"));
    }

    /* Output connectors */
    idx = devices[be]->current_output_connector;
    ui->Mixer->clear();
    ui->OutputConnector->clear();
    for (unsigned i=0; i<devices[be]->output.size(); i++)
    {
        QString name = QString::fromStdString(devices[be]->output[i]->name);
        ui->OutputConnector->addItem(name);
        ui->Mixer->addItem(name);
    }
    ui->OutputConnector->setCurrentIndex(idx);
    ui->Mixer->setCurrentIndex(idx);
    changeOutputConnector(idx);

    if (current_device == (unsigned)file_be_pos) {
        ui->OutputBitrate->setEnabled(true);
    } else {
        ui->OutputBitrate->setEnabled(false);
    }

    idx = devices[be]->current_output_connector;
    devname = devices[be]->name.c_str();
    name = devname;

    if (be != (unsigned)file_be_pos)
    {
        if (devices[be]->output.size() > 0)
        {
            const char *ifname = devices[be]->output[idx]->name.c_str();
            name += std::string(": ") + ifname;
        }
    }
    else {
       name += ": " + std::string(tmpDir()) + "/AeonWave.wav";
    }
    cfg.close();

    aax::AeonWave cfgi(name.c_str(), AAX_MODE_WRITE_STEREO);
    if (cfgi)
    {
       int min, max;
       bool x;

       cfgi.set(AAX_INITIALIZED);

       x = cfgi.get(AAX_TIMER_MODE);
       ui->Timer->setEnabled(x);

       x = cfgi.get(AAX_SHARED_MODE);
       ui->Shared->setEnabled(x);

       min = cfgi.get(AAX_TRACKS_MIN);
       max = cfgi.get(AAX_TRACKS_MAX);
       itemsGrayOut(ui->OutputSpeakers, min, max);

       min = cfgi.get(AAX_FREQUENCY_MIN);
       max = cfgi.get(AAX_FREQUENCY_MAX);
       itemsGrayOut(ui->OutputSampleFreq, min, max);

       min = cfgi.get(AAX_PERIODS_MIN);
       max = cfgi.get(AAX_PERIODS_MAX);
       itemsGrayOut(ui->OutputPeriods, min, max);
    }
    else if (found) {
        alert(tr("Output device unavailable or busy"));
    }
}

void
AeonWaveConfig::displayUiConfig()
{
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
AeonWaveConfig::itemsGrayOut(QComboBox *combobox, unsigned int min, unsigned int max)
{
   int valmin = -1, valmax = 0;
   bool disabled;

   for (int i=0; i<combobox->count(); i++)
   {
       const QStandardItemModel* model;
       model = qobject_cast<const QStandardItemModel*>(combobox->model());

       unsigned int val = combobox->itemText(i).toInt();
       if (val == 0 && combobox->itemText(i).contains(" ")) {
          val = combobox->itemText(i).split(" ")[0].toInt();
       }
       else if (val == 0 && combobox->itemText(i).contains(".")) {
           val = combobox->itemText(i).split(".")[0].toInt() +
                 combobox->itemText(i).split(".")[1].toInt();
       }
       if ((val < min) || (val > max)) disabled = true;
       else disabled = false;

       QStandardItem* item = model->item(i);
       Qt::ItemFlags flags = item->flags();
       if (disabled)
       {
           flags &= ~(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
       }
       else
       {
           flags |= (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
           if (valmin < 0) valmin = i;
           if (valmin >= 0) valmax = i;
       }
       item->setFlags(flags);
   }

   if (valmin < 0) valmin = 0;
   combobox->setCurrentIndex(_MINMAX(combobox->currentIndex(), valmin, valmax));
}

void
AeonWaveConfig::writeConfigFile()
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
        file << "<configuration>" << std::endl;
        file << " <version>2.0</version>" << std::endl;

        if (default_input_device != UINT_MAX)
        {
            file << " <input>" << std::endl;
            unsigned dev = devices[default_input_device]->default_input_connector;
            file << "  <device>";
            file << devices[default_input_device]->name;
            file << ": ";
            file << devices[default_input_device]->input[dev]->name;
            file << "</device>" << std::endl;

            file << "  <frequency-hz>";
            file << devices[default_input_device]->input[dev]->sample_freq;
            file << "</frequency-hz>" << std::endl;

            file << " </input>" << std::endl;
        }

        file << " <output>" << std::endl;
        if (default_device != UINT_MAX)
        {
           unsigned dev = devices[default_device]->default_output_connector;
           file << "  <device>";
           file << devices[default_device]->name;
           file << ": ";
           file << devices[default_device]->output[dev]->name;
           file << "</device>" << std::endl;

           file << "  <frequency-hz>";
            file << devices[default_device]->output[dev]->sample_freq;
            file << "</frequency-hz>" << std::endl;
        }
        else
        {
            file << "  <frequency-hz>"<< general_sample_freq;
            file << "</frequency-hz>" << std::endl;
        }

        file << "  <setup>";
        if (general_setup == AAX_MODE_WRITE_STEREO) file << "stereo";
        else if (general_setup == AAX_MODE_WRITE_SPATIAL) file << "spatial";
        else if (general_setup == AAX_MODE_WRITE_SPATIAL_SURROUND) file << "spatial-surround";
        else if (general_setup == AAX_MODE_WRITE_HRTF) file << "hrtf";
        else if (general_setup == AAX_MODE_WRITE_SURROUND) file << "surround";
        file << "</setup>" << std::endl;
        file << " </output>" << std::endl << std::endl;

        for (unsigned be=0; be<devices.size(); be++)
        {
            char *device = strdup(devices[be]->name.c_str());
            char *ptr;

            ptr = strstr(device, " on ");
            if (!ptr) continue;

            *ptr = 0;
            ptr += strlen(" on ");

            file << " <device backend=\"" << device << "\"";
            file << " name=\"" << ptr << "\">" << std::endl;
            free(device);

            for (unsigned dev=0; dev<devices[be]->output.size(); dev++)
            {
                file << "  <connector name=\"" << devices[be]->output[dev]->name;
                file << "\" type=\"out\">" << std::endl;

                file << "   <timed>" << (devices[be]->output[dev]->timed ? "true" : "false");
                file << "</timed>" << std::endl;
                file << "   <shared>" << (devices[be]->output[dev]->shared ? "true" : "false");
                file << "</shared>" << std::endl;

                file << "   <setup>";
                switch(devices[be]->output[dev]->setup)
                {
                case AAX_MODE_WRITE_SPATIAL:
                    file << "spatial";
                    break;
                case AAX_MODE_WRITE_SPATIAL_SURROUND:
                    file << "spatial-surround";
                    break;
                case AAX_MODE_WRITE_HRTF:
                    file << "hrtf";
                    break;
                case AAX_MODE_WRITE_SURROUND:
                    file << "surround";
                    break;
                case AAX_MODE_WRITE_STEREO:
                default:
                    file << "stereo";
                    break;
                }
                file << "</setup>" << std::endl;

                file << "   <periods>";
                file << devices[be]->output[dev]->no_periods;
                file <<"</periods>" << std::endl;

                if (be == (unsigned)file_be_pos)
                {
                    file << "   <bitrate>";
                    file << devices[be]->output[dev]->bitrate;
                    file << "</bitrate>" << std::endl;
                }

                file << "   <channels>";
                file << devices[be]->output[dev]->no_speakers;
                file <<"</channels>" << std::endl;

                file << "   <frequency-hz>";
                file << devices[be]->output[dev]->sample_freq;
                file << "</frequency-hz>" << std::endl;

                file << "   <interval-hz>";
                file << devices[be]->output[dev]->refresh_rate;
                file << "</interval-hz>" << std::endl;

                if (devices[be]->output[dev]->setup == AAX_MODE_WRITE_HRTF)
                {
                    file << "   <hrtf>" << std::endl;
                    file << "    <side-delay-sec>640e-6</side-delay-sec>" << std::endl;
//                  file << "    <side-offset-sec>0.0</side-offset-sec>" << std::endl;
                    file << "    <forward-delay-sec>65e-6</forward-delay-sec>" << std::endl;
                    file << "    <forward-offset-sec>15e-6</forward-offset-sec>" << std::endl;
                    file << "    <up-delay-sec>-110e-6</up-delay-sec>" << std::endl;
                    file << "    <up-offset-sec>200e-6</up-offset-sec>" << std::endl;
                    file << "   </hrtf>" << std::endl;
                }
                else
                {
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
                }
                file << "  </connector>" << std::endl;
            }

            for(unsigned dev=0; dev<devices[be]->input.size(); dev++)
            {
                file << "  <connector name=\"" << devices[be]->input[dev]->name;
                file << "\" type=\"in\">" << std::endl;

                file << "   <timed>false</timed>" << std::endl;
                file << "   <shared>false</shared>" << std::endl;

                file << "   <setup>";
                switch(devices[be]->input[dev]->setup)
                {
                case AAX_TRACK_ALL:
                    file << "stereo";
                    break;
                case AAX_TRACK_LEFT:
                    file << "left";
                    break;
                case AAX_TRACK_RIGHT:
                    file << "right";
                    break;
                case AAX_TRACK_MIX:
                default:
                    file << "mix";
                    break;
                }
                file << "</setup>" << std::endl;

                file << "   <periods>";
                file << devices[be]->input[dev]->no_periods;
                file <<"</periods>" << std::endl;

                file << "   <frequency-hz>";
                file << devices[be]->input[dev]->sample_freq;
                file << "</frequency-hz>" << std::endl;

                file << "   <interval-hz>";
                file << devices[be]->input[dev]->refresh_rate;
                file << "</interval-hz>" << std::endl;

                file << "  </connector>" << std::endl;
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
            QString msg = tr("Error writing to file: ");
            msg += from_path.c_str();
            msg += "\n\r";
            msg += tr(strerror(errno));
            alert(msg);
            file.close();
            return;
        }

        file << "<?xml version=\"1.0\"?>" << std::endl << std::endl;
        file << "<configuration>" << std::endl;
//      file << " <version>1.1</version>" << std::endl;

        file << " <output>" << std::endl;
        file << "  <frequency-hz>"<< general_sample_freq;
        file << "</frequency-hz>" << std::endl;

        file << "  <interval-hz>" << refresh_rate;
        file << "</interval-hz>" << std::endl;

        file << "  <setup>";
        if (general_setup == AAX_MODE_WRITE_STEREO) file << "stereo";
        else if (general_setup == AAX_MODE_WRITE_SPATIAL) file << "spatial";
        else if (general_setup == AAX_MODE_WRITE_SPATIAL_SURROUND) file << "spatial-surround";
        else if (general_setup == AAX_MODE_WRITE_HRTF) file << "hrtf";
        else if (general_setup == AAX_MODE_WRITE_SURROUND) file << "surround";
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
                case AAX_MODE_WRITE_SPATIAL_SURROUND:
                    file << "spatial-surround";
                    break;
                case AAX_MODE_WRITE_HRTF:
                    file << "hrtf";
                    break;
                case AAX_MODE_WRITE_SURROUND:
                    file << "surround";
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

