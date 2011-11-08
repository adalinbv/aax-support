/********************************************************************************
** Form generated from reading UI file 'aaxconfig.ui'
**
** Created: Wed Oct 26 13:05:22 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class UiConfig
{
public:
    QLabel *label_product_key;
    QLineEdit *ProductKey;

    QLabel *label_general_sample_freq;
    QComboBox *GeneralSampleFreq;

    QLabel *label_refresh_rate;
    QSpinBox *RefreshRate;

    QLabel *label_setup;
    QComboBox *SpeakerSetup;

    QLabel *label_backend;
    QComboBox *Backend;

    QLabel *label_output_device;
    QComboBox *OutputDevice;

    QLabel *label_output_sample_freq;
    QComboBox *OutputSampleFreq;

    QLabel *label_speakers;
    QComboBox *OutputSpeakers;

    QLabel *label_input_device;
    QComboBox *InputDevice;

    QLabel *label_input_sample_freq;
    QComboBox *InputSampleFreq;

    QPushButton *OK;

    QFrame *line;
    QFrame *line_2;

    void setupUi(QDialog *Configuration)
    {
        if (Configuration->objectName().isEmpty())
            Configuration->setObjectName(QString::fromUtf8("Configuration"));
        Configuration->resize(579, 366);
        ProductKey = new QLineEdit(Configuration);
        ProductKey->setObjectName(QString::fromUtf8("ProductKey"));
        ProductKey->setGeometry(QRect(20, 30, 251, 21));
        label_product_key = new QLabel(Configuration);
        label_product_key->setObjectName(QString::fromUtf8("label_product_key"));
        label_product_key->setGeometry(QRect(20, 10, 91, 17));
        GeneralSampleFreq = new QComboBox(Configuration);
        GeneralSampleFreq->setObjectName(QString::fromUtf8("GeneralSampleFreq"));
        GeneralSampleFreq->setGeometry(QRect(20, 100, 121, 27));
        label_general_sample_freq = new QLabel(Configuration);
        label_general_sample_freq->setObjectName(QString::fromUtf8("label_general_sample_freq"));
        label_general_sample_freq->setGeometry(QRect(20, 80, 131, 20));
        RefreshRate = new QSpinBox(Configuration);
        RefreshRate->setObjectName(QString::fromUtf8("RefreshRate"));
        RefreshRate->setGeometry(QRect(170, 100, 91, 27));
        label_refresh_rate = new QLabel(Configuration);
        label_refresh_rate->setObjectName(QString::fromUtf8("label_refresh_rate"));
        label_refresh_rate->setGeometry(QRect(170, 80, 91, 17));
        SpeakerSetup = new QComboBox(Configuration);
        SpeakerSetup->setObjectName(QString::fromUtf8("SpeakerSetup"));
        SpeakerSetup->setGeometry(QRect(20, 150, 121, 27));
        label_setup = new QLabel(Configuration);
        label_setup->setObjectName(QString::fromUtf8("label_setup"));
        label_setup->setGeometry(QRect(20, 130, 62, 17));
        Backend = new QComboBox(Configuration);
        Backend->setObjectName(QString::fromUtf8("Backend"));
        Backend->setGeometry(QRect(290, 30, 271, 27));
        label_backend = new QLabel(Configuration);
        label_backend->setObjectName(QString::fromUtf8("label_backend"));
        label_backend->setGeometry(QRect(290, 10, 71, 17));
        label_output_sample_freq = new QLabel(Configuration);
        label_output_sample_freq->setObjectName(QString::fromUtf8("label_output_sample_freq"));
        label_output_sample_freq->setGeometry(QRect(290, 130, 131, 20));
        OutputSampleFreq = new QComboBox(Configuration);
        OutputSampleFreq->setObjectName(QString::fromUtf8("OutputSampleFreq"));
        OutputSampleFreq->setGeometry(QRect(290, 150, 121, 27));
        OutputSpeakers = new QComboBox(Configuration);
        OutputSpeakers->setObjectName(QString::fromUtf8("OutputSpeakers"));
        OutputSpeakers->setGeometry(QRect(460, 150, 101, 27));
        label_speakers = new QLabel(Configuration);
        label_speakers->setObjectName(QString::fromUtf8("label_speakers"));
        label_speakers->setGeometry(QRect(460, 130, 101, 17));
        OutputDevice = new QComboBox(Configuration);
        OutputDevice->setObjectName(QString::fromUtf8("OutputDevice"));
        OutputDevice->setGeometry(QRect(290, 100, 271, 27));
        label_output_device = new QLabel(Configuration);
        label_output_device->setObjectName(QString::fromUtf8("label_output_device"));
        label_output_device->setGeometry(QRect(290, 80, 151, 20));
        label_input_sample_freq = new QLabel(Configuration);
        label_input_sample_freq->setObjectName(QString::fromUtf8("label_input_sample_freq"));
        label_input_sample_freq->setGeometry(QRect(290, 250, 131, 20));
        InputSampleFreq = new QComboBox(Configuration);
        InputSampleFreq->setObjectName(QString::fromUtf8("InputSampleFreq"));
        InputSampleFreq->setGeometry(QRect(290, 270, 121, 27));
        label_input_device = new QLabel(Configuration);
        label_input_device->setObjectName(QString::fromUtf8("label_input_device"));
        label_input_device->setGeometry(QRect(290, 200, 141, 20));
        InputDevice = new QComboBox(Configuration);
        InputDevice->setObjectName(QString::fromUtf8("InputDevice"));
        InputDevice->setGeometry(QRect(290, 220, 271, 27));
        line = new QFrame(Configuration);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(20, 60, 251, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        OK = new QPushButton(Configuration);
        OK->setObjectName(QString::fromUtf8("OK"));
        OK->setGeometry(QRect(470, 320, 93, 27));
        line_2 = new QFrame(Configuration);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(270, 10, 20, 301));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        QWidget::setTabOrder(ProductKey, GeneralSampleFreq);
        QWidget::setTabOrder(GeneralSampleFreq, RefreshRate);
        QWidget::setTabOrder(RefreshRate, SpeakerSetup);
        QWidget::setTabOrder(SpeakerSetup, Backend);
        QWidget::setTabOrder(Backend, OutputDevice);
        QWidget::setTabOrder(OutputDevice, OutputSampleFreq);
        QWidget::setTabOrder(OutputSampleFreq, OutputSpeakers);
        QWidget::setTabOrder(OutputSpeakers, InputDevice);
        QWidget::setTabOrder(InputDevice, InputSampleFreq);
        QWidget::setTabOrder(InputSampleFreq, OK);

        retranslateUi(Configuration);
        QObject::connect(OK, SIGNAL(clicked()), Configuration, SLOT(writeConfig()));
        QObject::connect(ProductKey, SIGNAL(textEdited(QString)), Configuration, SLOT(changeProductKey(QString)));
        QObject::connect(RefreshRate, SIGNAL(valueChanged(int)), Configuration, SLOT(changeRefreshRate(int)));
        QObject::connect(SpeakerSetup, SIGNAL(currentIndexChanged(int)), Configuration, SLOT(changeSpeakerSetup(int)));
        QObject::connect(GeneralSampleFreq, SIGNAL(currentIndexChanged(int)), Configuration, SLOT(changeGeneralSampleFreq(int)));
        QObject::connect(InputSampleFreq, SIGNAL(currentIndexChanged(int)), Configuration, SLOT(changeInputSampleFreq(int)));
        QObject::connect(OutputSampleFreq, SIGNAL(currentIndexChanged(int)), Configuration, SLOT(changeOutputSampleFreq(int)));
        QObject::connect(OutputSpeakers, SIGNAL(currentIndexChanged(int)), Configuration, SLOT(changeNoSpeakers(int)));
        QObject::connect(InputDevice, SIGNAL(currentIndexChanged(int)), Configuration, SLOT(changeInputDevice(int)));
        QObject::connect(OutputDevice, SIGNAL(currentIndexChanged(int)), Configuration, SLOT(changeOutputDevice(int)));
        QObject::connect(Backend, SIGNAL(currentIndexChanged(int)), Configuration, SLOT(changeBackend(int)));

        QMetaObject::connectSlotsByName(Configuration);
    } // setupUi

    void retranslateUi(QDialog *Configuration)
    {
        Configuration->setWindowTitle(QApplication::translate("Configuration", "AeonWave Configure", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ProductKey->setToolTip(QApplication::translate("Configuration", "product-key", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_product_key->setText(QApplication::translate("Configuration", "Product-Key:", 0, QApplication::UnicodeUTF8));
        GeneralSampleFreq->clear();
        GeneralSampleFreq->insertItems(0, QStringList()
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
        label_general_sample_freq->setText(QApplication::translate("Configuration", "Sample Frequency:", 0, QApplication::UnicodeUTF8));
        label_refresh_rate->setText(QApplication::translate("Configuration", "Refresh rate:", 0, QApplication::UnicodeUTF8));
        SpeakerSetup->clear();
        SpeakerSetup->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "Stereo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "Spatial", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "Surround", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "HRTF", 0, QApplication::UnicodeUTF8)
        );
        label_setup->setText(QApplication::translate("Configuration", "Setup:", 0, QApplication::UnicodeUTF8));
        label_backend->setText(QApplication::translate("Configuration", "Backend:", 0, QApplication::UnicodeUTF8));
        label_output_sample_freq->setText(QApplication::translate("Configuration", "Sample Frequency:", 0, QApplication::UnicodeUTF8));
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
        label_speakers->setText(QApplication::translate("Configuration", "Speaker Setup:", 0, QApplication::UnicodeUTF8));
        label_output_device->setText(QApplication::translate("Configuration", "Default Output Device:", 0, QApplication::UnicodeUTF8));
        label_input_sample_freq->setText(QApplication::translate("Configuration", "Sample Frequency:", 0, QApplication::UnicodeUTF8));
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
        label_input_device->setText(QApplication::translate("Configuration", "Default Input Device:", 0, QApplication::UnicodeUTF8));
        OK->setText(QApplication::translate("Configuration", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Configuration: public UiConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_H
