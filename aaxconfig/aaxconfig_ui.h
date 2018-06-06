/********************************************************************************
** Form generated from reading UI file 'aaxconfig.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef AAXCONFIG_UI_H
#define AAXCONFIG_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Configuration
{
public:
    QComboBox *Device;
    QLabel *label_backend;
    QDialogButtonBox *OK;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QLabel *label_input_sample_bitrate;
    QLabel *label_output_sample_freq;
    QComboBox *OutputSampleFreq;
    QComboBox *OutputConnector;
    QComboBox *OutputSpeakers;
    QLabel *label_periods;
    QComboBox *SpeakerSetup;
    QCheckBox *Timer;
    QComboBox *OutputPeriods;
    QLabel *label_speakers;
    QLabel *label_setup;
    QComboBox *OutputBitrate;
    QCheckBox *Shared;
    QLabel *label_refresh_rate;
    QSpinBox *RefreshRate;
    QLabel *graphicsView;
    QCheckBox *SetDefault;
    QPushButton *testPlay;
    QPushButton *applySettings;
    QWidget *tab_3;
    QComboBox *InputConnector;
    QComboBox *InputSampleFreq;
    QLabel *label_input_sample_freq;
    QSpinBox *InputRefreshRate;
    QLabel *label_refresh_rate_2;
    QLabel *label_setup_2;
    QLabel *label_periods_2;
    QComboBox *LineInSetup;
    QComboBox *InputPeriods;
    QLabel *graphicsViewInput;
    QCheckBox *InputSetDefault;
    QWidget *tab;
    QComboBox *Mixer;
    QLabel *graphicsView_logo;
    QLabel *MixerInfo;
    QWidget *tab_4;
    QLabel *FiltersEffects;
    QLabel *label_acquire;

    void setupUi(QDialog *Configuration)
    {
        if (Configuration->objectName().isEmpty())
            Configuration->setObjectName(QStringLiteral("Configuration"));
        Configuration->resize(530, 400);
        Configuration->setMinimumSize(QSize(530, 400));
        Configuration->setMaximumSize(QSize(530, 400));
        Device = new QComboBox(Configuration);
        Device->setObjectName(QStringLiteral("Device"));
        Device->setGeometry(QRect(10, 30, 271, 27));
        label_backend = new QLabel(Configuration);
        label_backend->setObjectName(QStringLiteral("label_backend"));
        label_backend->setGeometry(QRect(10, 10, 71, 17));
        OK = new QDialogButtonBox(Configuration);
        OK->setObjectName(QStringLiteral("OK"));
        OK->setGeometry(QRect(300, 360, 221, 32));
        OK->setOrientation(Qt::Horizontal);
        OK->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tabWidget = new QTabWidget(Configuration);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 60, 511, 291));
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        label_input_sample_bitrate = new QLabel(tab_2);
        label_input_sample_bitrate->setObjectName(QStringLiteral("label_input_sample_bitrate"));
        label_input_sample_bitrate->setGeometry(QRect(160, 170, 81, 20));
        label_output_sample_freq = new QLabel(tab_2);
        label_output_sample_freq->setObjectName(QStringLiteral("label_output_sample_freq"));
        label_output_sample_freq->setGeometry(QRect(20, 70, 121, 20));
        OutputSampleFreq = new QComboBox(tab_2);
        OutputSampleFreq->setObjectName(QStringLiteral("OutputSampleFreq"));
        OutputSampleFreq->setGeometry(QRect(20, 90, 121, 27));
        OutputConnector = new QComboBox(tab_2);
        OutputConnector->setObjectName(QStringLiteral("OutputConnector"));
        OutputConnector->setGeometry(QRect(10, 10, 311, 27));
        OutputSpeakers = new QComboBox(tab_2);
        OutputSpeakers->setObjectName(QStringLiteral("OutputSpeakers"));
        OutputSpeakers->setGeometry(QRect(160, 140, 121, 27));
        label_periods = new QLabel(tab_2);
        label_periods->setObjectName(QStringLiteral("label_periods"));
        label_periods->setGeometry(QRect(20, 170, 121, 20));
        SpeakerSetup = new QComboBox(tab_2);
        SpeakerSetup->setObjectName(QStringLiteral("SpeakerSetup"));
        SpeakerSetup->setGeometry(QRect(20, 140, 121, 27));
        Timer = new QCheckBox(tab_2);
        Timer->setObjectName(QStringLiteral("Timer"));
        Timer->setGeometry(QRect(160, 225, 221, 22));
        OutputPeriods = new QComboBox(tab_2);
        OutputPeriods->setObjectName(QStringLiteral("OutputPeriods"));
        OutputPeriods->setGeometry(QRect(20, 190, 121, 27));
        label_speakers = new QLabel(tab_2);
        label_speakers->setObjectName(QStringLiteral("label_speakers"));
        label_speakers->setGeometry(QRect(160, 120, 121, 20));
        label_setup = new QLabel(tab_2);
        label_setup->setObjectName(QStringLiteral("label_setup"));
        label_setup->setGeometry(QRect(20, 120, 121, 17));
        OutputBitrate = new QComboBox(tab_2);
        OutputBitrate->setObjectName(QStringLiteral("OutputBitrate"));
        OutputBitrate->setEnabled(false);
        OutputBitrate->setGeometry(QRect(160, 190, 121, 27));
        Shared = new QCheckBox(tab_2);
        Shared->setObjectName(QStringLiteral("Shared"));
        Shared->setGeometry(QRect(20, 225, 97, 22));
        label_refresh_rate = new QLabel(tab_2);
        label_refresh_rate->setObjectName(QStringLiteral("label_refresh_rate"));
        label_refresh_rate->setGeometry(QRect(160, 70, 121, 17));
        RefreshRate = new QSpinBox(tab_2);
        RefreshRate->setObjectName(QStringLiteral("RefreshRate"));
        RefreshRate->setGeometry(QRect(160, 90, 121, 27));
        graphicsView = new QLabel(tab_2);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(310, 50, 180, 180));
        graphicsView->setFrameShape(QFrame::NoFrame);
        graphicsView->setFrameShadow(QFrame::Sunken);
        graphicsView->setLineWidth(1);
        SetDefault = new QCheckBox(tab_2);
        SetDefault->setObjectName(QStringLiteral("SetDefault"));
        SetDefault->setGeometry(QRect(20, 40, 171, 22));
        testPlay = new QPushButton(tab_2);
        testPlay->setObjectName(QStringLiteral("testPlay"));
        testPlay->setGeometry(QRect(410, 10, 71, 25));
        applySettings = new QPushButton(tab_2);
        applySettings->setObjectName(QStringLiteral("applySettings"));
        applySettings->setGeometry(QRect(330, 10, 71, 25));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        InputConnector = new QComboBox(tab_3);
        InputConnector->setObjectName(QStringLiteral("InputConnector"));
        InputConnector->setGeometry(QRect(10, 10, 311, 27));
        InputSampleFreq = new QComboBox(tab_3);
        InputSampleFreq->setObjectName(QStringLiteral("InputSampleFreq"));
        InputSampleFreq->setGeometry(QRect(20, 90, 121, 27));
        label_input_sample_freq = new QLabel(tab_3);
        label_input_sample_freq->setObjectName(QStringLiteral("label_input_sample_freq"));
        label_input_sample_freq->setGeometry(QRect(20, 70, 81, 20));
        InputRefreshRate = new QSpinBox(tab_3);
        InputRefreshRate->setObjectName(QStringLiteral("InputRefreshRate"));
        InputRefreshRate->setGeometry(QRect(160, 90, 121, 27));
        label_refresh_rate_2 = new QLabel(tab_3);
        label_refresh_rate_2->setObjectName(QStringLiteral("label_refresh_rate_2"));
        label_refresh_rate_2->setGeometry(QRect(160, 70, 121, 17));
        label_setup_2 = new QLabel(tab_3);
        label_setup_2->setObjectName(QStringLiteral("label_setup_2"));
        label_setup_2->setGeometry(QRect(20, 120, 121, 17));
        label_periods_2 = new QLabel(tab_3);
        label_periods_2->setObjectName(QStringLiteral("label_periods_2"));
        label_periods_2->setGeometry(QRect(20, 170, 121, 20));
        LineInSetup = new QComboBox(tab_3);
        LineInSetup->setObjectName(QStringLiteral("LineInSetup"));
        LineInSetup->setGeometry(QRect(20, 140, 121, 27));
        InputPeriods = new QComboBox(tab_3);
        InputPeriods->setObjectName(QStringLiteral("InputPeriods"));
        InputPeriods->setGeometry(QRect(20, 190, 121, 27));
        graphicsViewInput = new QLabel(tab_3);
        graphicsViewInput->setObjectName(QStringLiteral("graphicsViewInput"));
        graphicsViewInput->setGeometry(QRect(310, 60, 180, 180));
        graphicsViewInput->setFrameShape(QFrame::NoFrame);
        graphicsViewInput->setFrameShadow(QFrame::Sunken);
        graphicsViewInput->setLineWidth(1);
        InputSetDefault = new QCheckBox(tab_3);
        InputSetDefault->setObjectName(QStringLiteral("InputSetDefault"));
        InputSetDefault->setGeometry(QRect(20, 40, 171, 22));
        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        Mixer = new QComboBox(tab);
        Mixer->setObjectName(QStringLiteral("Mixer"));
        Mixer->setGeometry(QRect(10, 10, 311, 27));
        graphicsView_logo = new QLabel(tab);
        graphicsView_logo->setObjectName(QStringLiteral("graphicsView_logo"));
        graphicsView_logo->setGeometry(QRect(360, 0, 130, 48));
        graphicsView_logo->setFrameShape(QFrame::NoFrame);
        graphicsView_logo->setFrameShadow(QFrame::Sunken);
        graphicsView_logo->setLineWidth(1);
        graphicsView_logo->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        MixerInfo = new QLabel(tab);
        MixerInfo->setObjectName(QStringLiteral("MixerInfo"));
        MixerInfo->setGeometry(QRect(10, 40, 491, 211));
        tabWidget->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        FiltersEffects = new QLabel(tab_4);
        FiltersEffects->setObjectName(QStringLiteral("FiltersEffects"));
        FiltersEffects->setGeometry(QRect(10, 10, 491, 241));
        tabWidget->addTab(tab_4, QString());
        label_acquire = new QLabel(Configuration);
        label_acquire->setObjectName(QStringLiteral("label_acquire"));
        label_acquire->setGeometry(QRect(460, 10, 56, 20));
        label_acquire->setLayoutDirection(Qt::RightToLeft);
        label_acquire->setTextFormat(Qt::RichText);
        label_acquire->setOpenExternalLinks(true);
        QWidget::setTabOrder(Device, tabWidget);
        QWidget::setTabOrder(tabWidget, OutputConnector);
        QWidget::setTabOrder(OutputConnector, SetDefault);
        QWidget::setTabOrder(SetDefault, OutputSampleFreq);
        QWidget::setTabOrder(OutputSampleFreq, RefreshRate);
        QWidget::setTabOrder(RefreshRate, SpeakerSetup);
        QWidget::setTabOrder(SpeakerSetup, OutputSpeakers);
        QWidget::setTabOrder(OutputSpeakers, OutputPeriods);
        QWidget::setTabOrder(OutputPeriods, OutputBitrate);
        QWidget::setTabOrder(OutputBitrate, Shared);
        QWidget::setTabOrder(Shared, Timer);
        QWidget::setTabOrder(Timer, applySettings);
        QWidget::setTabOrder(applySettings, testPlay);
        QWidget::setTabOrder(testPlay, InputConnector);
        QWidget::setTabOrder(InputConnector, InputSetDefault);
        QWidget::setTabOrder(InputSetDefault, InputSampleFreq);
        QWidget::setTabOrder(InputSampleFreq, InputRefreshRate);
        QWidget::setTabOrder(InputRefreshRate, LineInSetup);
        QWidget::setTabOrder(LineInSetup, InputPeriods);
        QWidget::setTabOrder(InputPeriods, Mixer);

        retranslateUi(Configuration);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Configuration);
    } // setupUi

    void retranslateUi(QDialog *Configuration)
    {
        Configuration->setWindowTitle(QApplication::translate("Configuration", "AeonWave Configuration", Q_NULLPTR));
        label_backend->setText(QApplication::translate("Configuration", "Device:", Q_NULLPTR));
        label_input_sample_bitrate->setText(QApplication::translate("Configuration", "Bitrate", Q_NULLPTR));
        label_output_sample_freq->setText(QApplication::translate("Configuration", "Frequency:", Q_NULLPTR));
        OutputSampleFreq->clear();
        OutputSampleFreq->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "8000 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "11025 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "16000 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "22050 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "32000 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "44056 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "44100 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "48000 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "88200 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "96000 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "176400 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "192000 Hz", Q_NULLPTR)
        );
        OutputSpeakers->clear();
        OutputSpeakers->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "2", Q_NULLPTR)
         << QApplication::translate("Configuration", "4", Q_NULLPTR)
         << QApplication::translate("Configuration", "5.1", Q_NULLPTR)
         << QApplication::translate("Configuration", "7.1", Q_NULLPTR)
        );
        label_periods->setText(QApplication::translate("Configuration", "Buffer Periods:", Q_NULLPTR));
        SpeakerSetup->clear();
        SpeakerSetup->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "Stereo", Q_NULLPTR)
         << QApplication::translate("Configuration", "Spatial", Q_NULLPTR)
         << QApplication::translate("Configuration", "Surround", Q_NULLPTR)
         << QApplication::translate("Configuration", "HRTF", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        Timer->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Enable the system timer for pushing data to the audio device.</p><p>This will save power consumption but is less reliable for lower latencies. Therefore this option is only used for refresh rates below 75Hz.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        Timer->setText(QApplication::translate("Configuration", "Timer Driven (experimental)", Q_NULLPTR));
        OutputPeriods->clear();
        OutputPeriods->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "1", Q_NULLPTR)
         << QApplication::translate("Configuration", "2", Q_NULLPTR)
         << QApplication::translate("Configuration", "3", Q_NULLPTR)
         << QApplication::translate("Configuration", "4", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        OutputPeriods->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>For lowest latencies set this option to 1 or 2 (default).</p><p>Some devices require more playback-periods for smooth playback but this will increase the latency.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        label_speakers->setText(QApplication::translate("Configuration", "No. Speakers:", Q_NULLPTR));
        label_setup->setText(QApplication::translate("Configuration", "Speaker Setup:", Q_NULLPTR));
        OutputBitrate->clear();
        OutputBitrate->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "64kbps", Q_NULLPTR)
         << QApplication::translate("Configuration", "128kpbs", Q_NULLPTR)
         << QApplication::translate("Configuration", "192kbps", Q_NULLPTR)
         << QApplication::translate("Configuration", "256kbps", Q_NULLPTR)
         << QApplication::translate("Configuration", "320kbps", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        OutputBitrate->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>The bitrate defines the size and accuracy of compressed audio formats like mp3.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        Shared->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Allow this device to be shared with other applications.</p><p>This will increase latency.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        Shared->setText(QApplication::translate("Configuration", "Shared", Q_NULLPTR));
        label_refresh_rate->setText(QApplication::translate("Configuration", "Refresh Rate (Hz):", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        RefreshRate->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>The refresh rate defines how often new data is retrieved from the device.</p><p>Higher values result in lower latency.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        SetDefault->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Allow this device to be shared with other applications.</p><p>This will increase latency.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        SetDefault->setText(QApplication::translate("Configuration", "Set as default output", Q_NULLPTR));
        testPlay->setText(QApplication::translate("Configuration", "test", Q_NULLPTR));
        applySettings->setText(QApplication::translate("Configuration", "apply", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Configuration", "Playback", Q_NULLPTR));
        InputSampleFreq->clear();
        InputSampleFreq->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "8000 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "11025 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "16000 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "22050 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "32000 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "44056 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "44100 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "48000 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "88200 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "96000 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "176400 Hz", Q_NULLPTR)
         << QApplication::translate("Configuration", "192000 Hz", Q_NULLPTR)
        );
        label_input_sample_freq->setText(QApplication::translate("Configuration", "Frequency:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        InputRefreshRate->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>The refresh rate defines how often new data is retrieved from the device.</p><p>Higher values result in lower latency.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        label_refresh_rate_2->setText(QApplication::translate("Configuration", "Refresh Rate (Hz):", Q_NULLPTR));
        label_setup_2->setText(QApplication::translate("Configuration", "Line-in Setup:", Q_NULLPTR));
        label_periods_2->setText(QApplication::translate("Configuration", "Buffer Periods:", Q_NULLPTR));
        LineInSetup->clear();
        LineInSetup->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "Stereo", Q_NULLPTR)
         << QApplication::translate("Configuration", "Mono Left", Q_NULLPTR)
         << QApplication::translate("Configuration", "Mono Right", Q_NULLPTR)
         << QApplication::translate("Configuration", "Mono Mix", Q_NULLPTR)
        );
        InputPeriods->clear();
        InputPeriods->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "1", Q_NULLPTR)
         << QApplication::translate("Configuration", "2", Q_NULLPTR)
         << QApplication::translate("Configuration", "3", Q_NULLPTR)
         << QApplication::translate("Configuration", "4", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        InputPeriods->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>For lowest latencies set this option to 1 or 2 (default).</p><p>Some devices require more capture-periods for smooth recording but this will increase the latency.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        InputSetDefault->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Allow this device to be shared with other applications.</p><p>This will increase latency.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        InputSetDefault->setText(QApplication::translate("Configuration", "Set as default intput", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("Configuration", "Capture", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Configuration", "Information", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("Configuration", "Filters and Effects", Q_NULLPTR));
        label_acquire->setText(QApplication::translate("Configuration", "<html><head/><body><p><a href=\"http://www.adalin.com/buy_aeonwaveHD.html\"><span style=\" text-decoration: underline; color:#0000ff;\">Acquire</span></a></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Configuration: public Ui_Configuration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // AAXCONFIG_UI_H
