/********************************************************************************
** Form generated from reading UI file 'aaxconfig.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef AAXCONFIG_UI_H
#define AAXCONFIG_UI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "widgets.h"

QT_BEGIN_NAMESPACE

class Ui_Configuration
{
public:
    aaxConfigLineEdit *ProductKey;
    QLabel *label_product_key;
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
            Configuration->setObjectName(QString::fromUtf8("Configuration"));
        Configuration->resize(530, 400);
        Configuration->setMinimumSize(QSize(530, 400));
        Configuration->setMaximumSize(QSize(530, 400));
        ProductKey = new aaxConfigLineEdit(Configuration);
        ProductKey->setObjectName(QString::fromUtf8("ProductKey"));
        ProductKey->setGeometry(QRect(300, 33, 221, 21));
        label_product_key = new QLabel(Configuration);
        label_product_key->setObjectName(QString::fromUtf8("label_product_key"));
        label_product_key->setGeometry(QRect(300, 10, 91, 17));
        Device = new QComboBox(Configuration);
        Device->setObjectName(QString::fromUtf8("Device"));
        Device->setGeometry(QRect(10, 30, 271, 27));
        label_backend = new QLabel(Configuration);
        label_backend->setObjectName(QString::fromUtf8("label_backend"));
        label_backend->setGeometry(QRect(10, 10, 71, 17));
        OK = new QDialogButtonBox(Configuration);
        OK->setObjectName(QString::fromUtf8("OK"));
        OK->setGeometry(QRect(300, 360, 221, 32));
        OK->setOrientation(Qt::Horizontal);
        OK->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tabWidget = new QTabWidget(Configuration);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 60, 511, 291));
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        label_input_sample_bitrate = new QLabel(tab_2);
        label_input_sample_bitrate->setObjectName(QString::fromUtf8("label_input_sample_bitrate"));
        label_input_sample_bitrate->setGeometry(QRect(160, 170, 81, 20));
        label_output_sample_freq = new QLabel(tab_2);
        label_output_sample_freq->setObjectName(QString::fromUtf8("label_output_sample_freq"));
        label_output_sample_freq->setGeometry(QRect(20, 70, 121, 20));
        OutputSampleFreq = new QComboBox(tab_2);
        OutputSampleFreq->setObjectName(QString::fromUtf8("OutputSampleFreq"));
        OutputSampleFreq->setGeometry(QRect(20, 90, 121, 27));
        OutputConnector = new QComboBox(tab_2);
        OutputConnector->setObjectName(QString::fromUtf8("OutputConnector"));
        OutputConnector->setGeometry(QRect(10, 10, 311, 27));
        OutputSpeakers = new QComboBox(tab_2);
        OutputSpeakers->setObjectName(QString::fromUtf8("OutputSpeakers"));
        OutputSpeakers->setGeometry(QRect(160, 140, 121, 27));
        label_periods = new QLabel(tab_2);
        label_periods->setObjectName(QString::fromUtf8("label_periods"));
        label_periods->setGeometry(QRect(20, 170, 121, 20));
        SpeakerSetup = new QComboBox(tab_2);
        SpeakerSetup->setObjectName(QString::fromUtf8("SpeakerSetup"));
        SpeakerSetup->setGeometry(QRect(20, 140, 121, 27));
        Timer = new QCheckBox(tab_2);
        Timer->setObjectName(QString::fromUtf8("Timer"));
        Timer->setGeometry(QRect(160, 225, 221, 22));
        OutputPeriods = new QComboBox(tab_2);
        OutputPeriods->setObjectName(QString::fromUtf8("OutputPeriods"));
        OutputPeriods->setGeometry(QRect(20, 190, 121, 27));
        label_speakers = new QLabel(tab_2);
        label_speakers->setObjectName(QString::fromUtf8("label_speakers"));
        label_speakers->setGeometry(QRect(160, 120, 121, 20));
        label_setup = new QLabel(tab_2);
        label_setup->setObjectName(QString::fromUtf8("label_setup"));
        label_setup->setGeometry(QRect(20, 120, 121, 17));
        OutputBitrate = new QComboBox(tab_2);
        OutputBitrate->setObjectName(QString::fromUtf8("OutputBitrate"));
        OutputBitrate->setEnabled(false);
        OutputBitrate->setGeometry(QRect(160, 190, 121, 27));
        Shared = new QCheckBox(tab_2);
        Shared->setObjectName(QString::fromUtf8("Shared"));
        Shared->setGeometry(QRect(20, 225, 97, 22));
        label_refresh_rate = new QLabel(tab_2);
        label_refresh_rate->setObjectName(QString::fromUtf8("label_refresh_rate"));
        label_refresh_rate->setGeometry(QRect(160, 70, 121, 17));
        RefreshRate = new QSpinBox(tab_2);
        RefreshRate->setObjectName(QString::fromUtf8("RefreshRate"));
        RefreshRate->setGeometry(QRect(160, 90, 121, 27));
        graphicsView = new QLabel(tab_2);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(310, 50, 180, 180));
        graphicsView->setFrameShape(QFrame::NoFrame);
        graphicsView->setFrameShadow(QFrame::Sunken);
        graphicsView->setLineWidth(1);
        SetDefault = new QCheckBox(tab_2);
        SetDefault->setObjectName(QString::fromUtf8("SetDefault"));
        SetDefault->setGeometry(QRect(20, 40, 171, 22));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        InputConnector = new QComboBox(tab_3);
        InputConnector->setObjectName(QString::fromUtf8("InputConnector"));
        InputConnector->setGeometry(QRect(10, 10, 311, 27));
        InputSampleFreq = new QComboBox(tab_3);
        InputSampleFreq->setObjectName(QString::fromUtf8("InputSampleFreq"));
        InputSampleFreq->setGeometry(QRect(20, 90, 121, 27));
        label_input_sample_freq = new QLabel(tab_3);
        label_input_sample_freq->setObjectName(QString::fromUtf8("label_input_sample_freq"));
        label_input_sample_freq->setGeometry(QRect(20, 70, 81, 20));
        InputRefreshRate = new QSpinBox(tab_3);
        InputRefreshRate->setObjectName(QString::fromUtf8("InputRefreshRate"));
        InputRefreshRate->setGeometry(QRect(160, 90, 121, 27));
        label_refresh_rate_2 = new QLabel(tab_3);
        label_refresh_rate_2->setObjectName(QString::fromUtf8("label_refresh_rate_2"));
        label_refresh_rate_2->setGeometry(QRect(160, 70, 121, 17));
        label_setup_2 = new QLabel(tab_3);
        label_setup_2->setObjectName(QString::fromUtf8("label_setup_2"));
        label_setup_2->setGeometry(QRect(20, 120, 121, 17));
        label_periods_2 = new QLabel(tab_3);
        label_periods_2->setObjectName(QString::fromUtf8("label_periods_2"));
        label_periods_2->setGeometry(QRect(20, 170, 121, 20));
        LineInSetup = new QComboBox(tab_3);
        LineInSetup->setObjectName(QString::fromUtf8("LineInSetup"));
        LineInSetup->setGeometry(QRect(20, 140, 121, 27));
        InputPeriods = new QComboBox(tab_3);
        InputPeriods->setObjectName(QString::fromUtf8("InputPeriods"));
        InputPeriods->setGeometry(QRect(20, 190, 121, 27));
        graphicsViewInput = new QLabel(tab_3);
        graphicsViewInput->setObjectName(QString::fromUtf8("graphicsViewInput"));
        graphicsViewInput->setGeometry(QRect(310, 60, 180, 180));
        graphicsViewInput->setFrameShape(QFrame::NoFrame);
        graphicsViewInput->setFrameShadow(QFrame::Sunken);
        graphicsViewInput->setLineWidth(1);
        InputSetDefault = new QCheckBox(tab_3);
        InputSetDefault->setObjectName(QString::fromUtf8("InputSetDefault"));
        InputSetDefault->setGeometry(QRect(20, 40, 171, 22));
        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        Mixer = new QComboBox(tab);
        Mixer->setObjectName(QString::fromUtf8("Mixer"));
        Mixer->setGeometry(QRect(10, 10, 311, 27));
        graphicsView_logo = new QLabel(tab);
        graphicsView_logo->setObjectName(QString::fromUtf8("graphicsView_logo"));
        graphicsView_logo->setGeometry(QRect(360, 0, 130, 48));
        graphicsView_logo->setFrameShape(QFrame::NoFrame);
        graphicsView_logo->setFrameShadow(QFrame::Sunken);
        graphicsView_logo->setLineWidth(1);
        graphicsView_logo->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        MixerInfo = new QLabel(tab);
        MixerInfo->setObjectName(QString::fromUtf8("MixerInfo"));
        MixerInfo->setGeometry(QRect(10, 40, 491, 211));
        tabWidget->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        FiltersEffects = new QLabel(tab_4);
        FiltersEffects->setObjectName(QString::fromUtf8("FiltersEffects"));
        FiltersEffects->setGeometry(QRect(10, 10, 491, 241));
        tabWidget->addTab(tab_4, QString());
        label_acquire = new QLabel(Configuration);
        label_acquire->setObjectName(QString::fromUtf8("label_acquire"));
        label_acquire->setGeometry(QRect(460, 10, 56, 20));
        label_acquire->setLayoutDirection(Qt::RightToLeft);
        label_acquire->setTextFormat(Qt::RichText);
        label_acquire->setOpenExternalLinks(true);
        QWidget::setTabOrder(Device, ProductKey);
        QWidget::setTabOrder(ProductKey, tabWidget);
        QWidget::setTabOrder(tabWidget, OutputConnector);
        QWidget::setTabOrder(OutputConnector, Shared);
        QWidget::setTabOrder(Shared, Timer);
        QWidget::setTabOrder(Timer, OutputSampleFreq);
        QWidget::setTabOrder(OutputSampleFreq, RefreshRate);
        QWidget::setTabOrder(RefreshRate, SpeakerSetup);
        QWidget::setTabOrder(SpeakerSetup, OutputSpeakers);
        QWidget::setTabOrder(OutputSpeakers, OutputPeriods);
        QWidget::setTabOrder(OutputPeriods, OutputBitrate);
        QWidget::setTabOrder(OutputBitrate, InputConnector);
        QWidget::setTabOrder(InputConnector, InputSampleFreq);
        QWidget::setTabOrder(InputSampleFreq, InputRefreshRate);
        QWidget::setTabOrder(InputRefreshRate, LineInSetup);
        QWidget::setTabOrder(LineInSetup, InputPeriods);
        QWidget::setTabOrder(InputPeriods, Mixer);
        QWidget::setTabOrder(Mixer, OK);

        retranslateUi(Configuration);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Configuration);
    } // setupUi

    void retranslateUi(QDialog *Configuration)
    {
        Configuration->setWindowTitle(QApplication::translate("Configuration", "AeonWave Configuration", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ProductKey->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>A valid prouduct key unlocks the advanced options of AeonWave and turns the software from the Lite mode to the HD mode.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_product_key->setText(QApplication::translate("Configuration", "Product-Key:", 0, QApplication::UnicodeUTF8));
        label_backend->setText(QApplication::translate("Configuration", "Device:", 0, QApplication::UnicodeUTF8));
        label_input_sample_bitrate->setText(QApplication::translate("Configuration", "Bitrate", 0, QApplication::UnicodeUTF8));
        label_output_sample_freq->setText(QApplication::translate("Configuration", "Frequency:", 0, QApplication::UnicodeUTF8));
        OutputSampleFreq->clear();
        OutputSampleFreq->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "8000 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "11025 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "16000 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "22050 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "32000 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "44056 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "44100 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "48000 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "88200 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "96000 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "176400 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "192000 Hz", 0, QApplication::UnicodeUTF8)
        );
        OutputSpeakers->clear();
        OutputSpeakers->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "5.1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "7.1", 0, QApplication::UnicodeUTF8)
        );
        label_periods->setText(QApplication::translate("Configuration", "Buffer Periods:", 0, QApplication::UnicodeUTF8));
        SpeakerSetup->clear();
        SpeakerSetup->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "Stereo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "Spatial", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "Surround", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "HRTF", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        Timer->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Enable the system timer for pushing data to the audio device.</p><p>This will save power consumption but is less reliable for lower latencies. Therefore this option is only used for refresh rates below 75Hz.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Timer->setText(QApplication::translate("Configuration", "Timer Driven (experimental)", 0, QApplication::UnicodeUTF8));
        OutputPeriods->clear();
        OutputPeriods->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "4", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        OutputPeriods->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>For lowest latencies set this option to 1 or 2 (default).</p><p>Some devices require more playback-periods for smooth playback but this will increase the latency.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_speakers->setText(QApplication::translate("Configuration", "No. Speakers:", 0, QApplication::UnicodeUTF8));
        label_setup->setText(QApplication::translate("Configuration", "Speaker Setup:", 0, QApplication::UnicodeUTF8));
        OutputBitrate->clear();
        OutputBitrate->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "64kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "128kpbs", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "192kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "256kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "320kbps", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        OutputBitrate->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>The bitrate defines the size and accuracy of compressed audio formats like mp3.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        Shared->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Allow this device to be shared with other applications.</p><p>This will increase latency.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Shared->setText(QApplication::translate("Configuration", "Shared", 0, QApplication::UnicodeUTF8));
        label_refresh_rate->setText(QApplication::translate("Configuration", "Refresh Rate (Hz):", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        RefreshRate->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>The refresh rate defines how often new data is retrieved from the device.</p><p>Higher values result in lower latency.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        SetDefault->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Allow this device to be shared with other applications.</p><p>This will increase latency.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        SetDefault->setText(QApplication::translate("Configuration", "Set as default output", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Configuration", "Playback", 0, QApplication::UnicodeUTF8));
        InputSampleFreq->clear();
        InputSampleFreq->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "8000 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "11025 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "16000 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "22050 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "32000 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "44056 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "44100 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "48000 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "88200 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "96000 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "176400 Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "192000 Hz", 0, QApplication::UnicodeUTF8)
        );
        label_input_sample_freq->setText(QApplication::translate("Configuration", "Frequency:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        InputRefreshRate->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>The refresh rate defines how often new data is retrieved from the device.</p><p>Higher values result in lower latency.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_refresh_rate_2->setText(QApplication::translate("Configuration", "Refresh Rate (Hz):", 0, QApplication::UnicodeUTF8));
        label_setup_2->setText(QApplication::translate("Configuration", "Line-in Setup:", 0, QApplication::UnicodeUTF8));
        label_periods_2->setText(QApplication::translate("Configuration", "Buffer Periods:", 0, QApplication::UnicodeUTF8));
        LineInSetup->clear();
        LineInSetup->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "Stereo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "Mono Left", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "Mono Right", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "Mono Mix", 0, QApplication::UnicodeUTF8)
        );
        InputPeriods->clear();
        InputPeriods->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "4", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        InputPeriods->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>For lowest latencies set this option to 1 or 2 (default).</p><p>Some devices require more capture-periods for smooth recording but this will increase the latency.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        InputSetDefault->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Allow this device to be shared with other applications.</p><p>This will increase latency.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        InputSetDefault->setText(QApplication::translate("Configuration", "Set as default intput", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("Configuration", "Capture", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Configuration", "Information", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("Configuration", "Filters and Effects", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_acquire->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Click here to get a valid Product-Key (an internet connection is required)</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_acquire->setText(QApplication::translate("Configuration", "<html><head/><body><p><a href=\"http://www.adalin.com/buy_aeonwaveHD.html\"><span style=\" text-decoration: underline; color:#0000ff;\">Acquire</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Configuration: public Ui_Configuration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // AAXCONFIG_UI_H
