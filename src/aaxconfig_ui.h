/********************************************************************************
** Form generated from reading UI file 'aaxconfig.ui'
**
** Created: Mon Mar 31 10:19:30 2014
**      by: Qt User Interface Compiler version 4.8.1
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
#include "widgets.h"

QT_BEGIN_NAMESPACE

class Ui_Configuration
{
public:
    aaxConfigLineEdit *ProductKey;
    QLabel *label_product_key;
    QComboBox *SpeakerSetup;
    QLabel *label_setup;
    QComboBox *Device;
    QLabel *label_backend;
    QLabel *label_output_sample_freq;
    QComboBox *OutputSampleFreq;
    QComboBox *OutputSpeakers;
    QLabel *label_speakers;
    QComboBox *OutputConnector;
    QLabel *label_output_device;
    QLabel *label_input_sample_freq;
    QComboBox *InputSampleFreq;
    QLabel *label_input_device;
    QComboBox *InputConnector;
    QLabel *graphicsView;
    QSpinBox *RefreshRate;
    QLabel *label_refresh_rate;
    QLabel *label_periods;
    QComboBox *OutputPeriods;
    QCheckBox *Timer;
    QCheckBox *Shared;
    QDialogButtonBox *OK;
    QLabel *graphicsView_logo;
    QComboBox *OutputBitrate;
    QLabel *label_input_sample_bitrate;

    void setupUi(QDialog *Configuration)
    {
        if (Configuration->objectName().isEmpty())
            Configuration->setObjectName(QString::fromUtf8("Configuration"));
        Configuration->resize(579, 400);
        Configuration->setMinimumSize(QSize(579, 400));
        Configuration->setMaximumSize(QSize(579, 400));
        ProductKey = new aaxConfigLineEdit(Configuration);
        ProductKey->setObjectName(QString::fromUtf8("ProductKey"));
        ProductKey->setGeometry(QRect(340, 305, 231, 21));
        label_product_key = new QLabel(Configuration);
        label_product_key->setObjectName(QString::fromUtf8("label_product_key"));
        label_product_key->setGeometry(QRect(340, 280, 91, 17));
        SpeakerSetup = new QComboBox(Configuration);
        SpeakerSetup->setObjectName(QString::fromUtf8("SpeakerSetup"));
        SpeakerSetup->setGeometry(QRect(200, 200, 121, 27));
        label_setup = new QLabel(Configuration);
        label_setup->setObjectName(QString::fromUtf8("label_setup"));
        label_setup->setGeometry(QRect(200, 180, 121, 17));
        Device = new QComboBox(Configuration);
        Device->setObjectName(QString::fromUtf8("Device"));
        Device->setGeometry(QRect(10, 30, 311, 27));
        label_backend = new QLabel(Configuration);
        label_backend->setObjectName(QString::fromUtf8("label_backend"));
        label_backend->setGeometry(QRect(10, 10, 71, 17));
        label_output_sample_freq = new QLabel(Configuration);
        label_output_sample_freq->setObjectName(QString::fromUtf8("label_output_sample_freq"));
        label_output_sample_freq->setGeometry(QRect(200, 130, 121, 20));
        OutputSampleFreq = new QComboBox(Configuration);
        OutputSampleFreq->setObjectName(QString::fromUtf8("OutputSampleFreq"));
        OutputSampleFreq->setGeometry(QRect(200, 150, 121, 27));
        OutputSpeakers = new QComboBox(Configuration);
        OutputSpeakers->setObjectName(QString::fromUtf8("OutputSpeakers"));
        OutputSpeakers->setGeometry(QRect(200, 250, 121, 27));
        label_speakers = new QLabel(Configuration);
        label_speakers->setObjectName(QString::fromUtf8("label_speakers"));
        label_speakers->setGeometry(QRect(200, 230, 121, 20));
        OutputConnector = new QComboBox(Configuration);
        OutputConnector->setObjectName(QString::fromUtf8("OutputConnector"));
        OutputConnector->setGeometry(QRect(10, 80, 311, 27));
        label_output_device = new QLabel(Configuration);
        label_output_device->setObjectName(QString::fromUtf8("label_output_device"));
        label_output_device->setGeometry(QRect(10, 60, 151, 20));
        label_input_sample_freq = new QLabel(Configuration);
        label_input_sample_freq->setObjectName(QString::fromUtf8("label_input_sample_freq"));
        label_input_sample_freq->setGeometry(QRect(450, 130, 81, 20));
        InputSampleFreq = new QComboBox(Configuration);
        InputSampleFreq->setObjectName(QString::fromUtf8("InputSampleFreq"));
        InputSampleFreq->setGeometry(QRect(450, 150, 121, 27));
        label_input_device = new QLabel(Configuration);
        label_input_device->setObjectName(QString::fromUtf8("label_input_device"));
        label_input_device->setGeometry(QRect(340, 60, 141, 20));
        InputConnector = new QComboBox(Configuration);
        InputConnector->setObjectName(QString::fromUtf8("InputConnector"));
        InputConnector->setGeometry(QRect(340, 80, 231, 27));
        graphicsView = new QLabel(Configuration);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(0, 200, 180, 180));
        graphicsView->setFrameShape(QFrame::NoFrame);
        graphicsView->setFrameShadow(QFrame::Sunken);
        graphicsView->setLineWidth(1);
        RefreshRate = new QSpinBox(Configuration);
        RefreshRate->setObjectName(QString::fromUtf8("RefreshRate"));
        RefreshRate->setGeometry(QRect(340, 30, 91, 27));
        label_refresh_rate = new QLabel(Configuration);
        label_refresh_rate->setObjectName(QString::fromUtf8("label_refresh_rate"));
        label_refresh_rate->setGeometry(QRect(340, 10, 91, 17));
        label_periods = new QLabel(Configuration);
        label_periods->setObjectName(QString::fromUtf8("label_periods"));
        label_periods->setGeometry(QRect(200, 280, 121, 20));
        OutputPeriods = new QComboBox(Configuration);
        OutputPeriods->setObjectName(QString::fromUtf8("OutputPeriods"));
        OutputPeriods->setGeometry(QRect(200, 300, 121, 27));
        Timer = new QCheckBox(Configuration);
        Timer->setObjectName(QString::fromUtf8("Timer"));
        Timer->setGeometry(QRect(110, 110, 221, 22));
        Shared = new QCheckBox(Configuration);
        Shared->setObjectName(QString::fromUtf8("Shared"));
        Shared->setGeometry(QRect(10, 110, 97, 22));
        OK = new QDialogButtonBox(Configuration);
        OK->setObjectName(QString::fromUtf8("OK"));
        OK->setGeometry(QRect(351, 359, 221, 32));
        OK->setOrientation(Qt::Horizontal);
        OK->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        graphicsView_logo = new QLabel(Configuration);
        graphicsView_logo->setObjectName(QString::fromUtf8("graphicsView_logo"));
        graphicsView_logo->setGeometry(QRect(440, 15, 130, 48));
        graphicsView_logo->setFrameShape(QFrame::NoFrame);
        graphicsView_logo->setFrameShadow(QFrame::Sunken);
        graphicsView_logo->setLineWidth(1);
        graphicsView_logo->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        OutputBitrate = new QComboBox(Configuration);
        OutputBitrate->setObjectName(QString::fromUtf8("OutputBitrate"));
        OutputBitrate->setEnabled(false);
        OutputBitrate->setGeometry(QRect(200, 350, 121, 27));
        label_input_sample_bitrate = new QLabel(Configuration);
        label_input_sample_bitrate->setObjectName(QString::fromUtf8("label_input_sample_bitrate"));
        label_input_sample_bitrate->setGeometry(QRect(200, 330, 81, 20));
        QWidget::setTabOrder(Device, ProductKey);
        QWidget::setTabOrder(ProductKey, RefreshRate);
        QWidget::setTabOrder(RefreshRate, OutputConnector);
        QWidget::setTabOrder(OutputConnector, Shared);
        QWidget::setTabOrder(Shared, Timer);
        QWidget::setTabOrder(Timer, OutputSampleFreq);
        QWidget::setTabOrder(OutputSampleFreq, SpeakerSetup);
        QWidget::setTabOrder(SpeakerSetup, OutputSpeakers);
        QWidget::setTabOrder(OutputSpeakers, OutputPeriods);
        QWidget::setTabOrder(OutputPeriods, InputConnector);
        QWidget::setTabOrder(InputConnector, InputSampleFreq);
        QWidget::setTabOrder(InputSampleFreq, OK);

        retranslateUi(Configuration);

        QMetaObject::connectSlotsByName(Configuration);
    } // setupUi

    void retranslateUi(QDialog *Configuration)
    {
        Configuration->setWindowTitle(QApplication::translate("Configuration", "AeonWave Configuration", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ProductKey->setToolTip(QApplication::translate("Configuration", "product-key", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_product_key->setText(QApplication::translate("Configuration", "Product-Key:", 0, QApplication::UnicodeUTF8));
        SpeakerSetup->clear();
        SpeakerSetup->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "Stereo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "Spatial", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "Surround", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "HRTF", 0, QApplication::UnicodeUTF8)
        );
        label_setup->setText(QApplication::translate("Configuration", "Speaker Setup:", 0, QApplication::UnicodeUTF8));
        label_backend->setText(QApplication::translate("Configuration", "Device:", 0, QApplication::UnicodeUTF8));
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
        label_speakers->setText(QApplication::translate("Configuration", "No. Speakers:", 0, QApplication::UnicodeUTF8));
        label_output_device->setText(QApplication::translate("Configuration", "Output Port:", 0, QApplication::UnicodeUTF8));
        label_input_sample_freq->setText(QApplication::translate("Configuration", "Frequency:", 0, QApplication::UnicodeUTF8));
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
        label_input_device->setText(QApplication::translate("Configuration", "Input Port:", 0, QApplication::UnicodeUTF8));
        label_refresh_rate->setText(QApplication::translate("Configuration", "Refresh Rate:", 0, QApplication::UnicodeUTF8));
        label_periods->setText(QApplication::translate("Configuration", "Buffer Periods:", 0, QApplication::UnicodeUTF8));
        OutputPeriods->clear();
        OutputPeriods->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "4", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        OutputPeriods->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>For lowest latencies set this option to 2 (default).</p><p>Some devices require a higer number of playback periods for higher refresh rates which decreases latency.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        Timer->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Enable the system timer for pushing data to the audio device.</p><p>This will save power consumption but is less reliable for lower latencies. Therefore this option is only used for refresh rates below 75Hz.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Timer->setText(QApplication::translate("Configuration", "Timer Driven (Experimental)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        Shared->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Allow this device to be shared with other applications.</p><p>This will increase latency.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Shared->setText(QApplication::translate("Configuration", "Shared", 0, QApplication::UnicodeUTF8));
        OutputBitrate->clear();
        OutputBitrate->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "64kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "128kpbs", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "192kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "256kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "320kbps", 0, QApplication::UnicodeUTF8)
        );
        label_input_sample_bitrate->setText(QApplication::translate("Configuration", "Bitrate", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Configuration: public Ui_Configuration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // AAXCONFIG_UI_H
