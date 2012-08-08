/********************************************************************************
** Form generated from reading UI file 'aaxconfig.ui'
**
** Created: Wed Aug 8 10:05:26 2012
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
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_Configuration
{
public:
    QLineEdit *ProductKey;
    QLabel *label_product_key;
    QComboBox *SpeakerSetup;
    QLabel *label_setup;
    QComboBox *Backend;
    QLabel *label_backend;
    QLabel *label_output_sample_freq;
    QComboBox *OutputSampleFreq;
    QComboBox *OutputSpeakers;
    QLabel *label_speakers;
    QComboBox *OutputDevice;
    QLabel *label_output_device;
    QLabel *label_input_sample_freq;
    QComboBox *InputSampleFreq;
    QLabel *label_input_device;
    QComboBox *InputDevice;
    QPushButton *OK;
    QLabel *graphicsView;
    QLabel *label_general_sample_freq;
    QComboBox *GeneralSampleFreq;
    QSpinBox *RefreshRate;
    QLabel *label_refresh_rate;

    void setupUi(QDialog *Configuration)
    {
        if (Configuration->objectName().isEmpty())
            Configuration->setObjectName(QString::fromUtf8("Configuration"));
        Configuration->resize(579, 390);
        Configuration->setMinimumSize(QSize(579, 390));
        Configuration->setMaximumSize(QSize(579, 390));
        ProductKey = new QLineEdit(Configuration);
        ProductKey->setObjectName(QString::fromUtf8("ProductKey"));
        ProductKey->setGeometry(QRect(10, 30, 311, 21));
        label_product_key = new QLabel(Configuration);
        label_product_key->setObjectName(QString::fromUtf8("label_product_key"));
        label_product_key->setGeometry(QRect(10, 10, 91, 17));
        SpeakerSetup = new QComboBox(Configuration);
        SpeakerSetup->setObjectName(QString::fromUtf8("SpeakerSetup"));
        SpeakerSetup->setGeometry(QRect(200, 260, 121, 27));
        label_setup = new QLabel(Configuration);
        label_setup->setObjectName(QString::fromUtf8("label_setup"));
        label_setup->setGeometry(QRect(200, 240, 101, 17));
        Backend = new QComboBox(Configuration);
        Backend->setObjectName(QString::fromUtf8("Backend"));
        Backend->setGeometry(QRect(10, 80, 311, 27));
        label_backend = new QLabel(Configuration);
        label_backend->setObjectName(QString::fromUtf8("label_backend"));
        label_backend->setGeometry(QRect(10, 60, 71, 17));
        label_output_sample_freq = new QLabel(Configuration);
        label_output_sample_freq->setObjectName(QString::fromUtf8("label_output_sample_freq"));
        label_output_sample_freq->setGeometry(QRect(200, 180, 81, 20));
        OutputSampleFreq = new QComboBox(Configuration);
        OutputSampleFreq->setObjectName(QString::fromUtf8("OutputSampleFreq"));
        OutputSampleFreq->setGeometry(QRect(200, 200, 121, 27));
        OutputSpeakers = new QComboBox(Configuration);
        OutputSpeakers->setObjectName(QString::fromUtf8("OutputSpeakers"));
        OutputSpeakers->setGeometry(QRect(200, 320, 121, 27));
        label_speakers = new QLabel(Configuration);
        label_speakers->setObjectName(QString::fromUtf8("label_speakers"));
        label_speakers->setGeometry(QRect(200, 300, 121, 20));
        OutputDevice = new QComboBox(Configuration);
        OutputDevice->setObjectName(QString::fromUtf8("OutputDevice"));
        OutputDevice->setGeometry(QRect(10, 140, 311, 27));
        label_output_device = new QLabel(Configuration);
        label_output_device->setObjectName(QString::fromUtf8("label_output_device"));
        label_output_device->setGeometry(QRect(10, 120, 151, 20));
        label_input_sample_freq = new QLabel(Configuration);
        label_input_sample_freq->setObjectName(QString::fromUtf8("label_input_sample_freq"));
        label_input_sample_freq->setGeometry(QRect(450, 180, 81, 20));
        InputSampleFreq = new QComboBox(Configuration);
        InputSampleFreq->setObjectName(QString::fromUtf8("InputSampleFreq"));
        InputSampleFreq->setGeometry(QRect(450, 200, 121, 27));
        label_input_device = new QLabel(Configuration);
        label_input_device->setObjectName(QString::fromUtf8("label_input_device"));
        label_input_device->setGeometry(QRect(340, 120, 141, 20));
        InputDevice = new QComboBox(Configuration);
        InputDevice->setObjectName(QString::fromUtf8("InputDevice"));
        InputDevice->setGeometry(QRect(340, 140, 231, 27));
        OK = new QPushButton(Configuration);
        OK->setObjectName(QString::fromUtf8("OK"));
        OK->setGeometry(QRect(470, 350, 93, 27));
        graphicsView = new QLabel(Configuration);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(0, 190, 180, 180));
        graphicsView->setFrameShape(QFrame::NoFrame);
        graphicsView->setFrameShadow(QFrame::Sunken);
        graphicsView->setLineWidth(1);
        label_general_sample_freq = new QLabel(Configuration);
        label_general_sample_freq->setObjectName(QString::fromUtf8("label_general_sample_freq"));
        label_general_sample_freq->setGeometry(QRect(340, 10, 131, 20));
        GeneralSampleFreq = new QComboBox(Configuration);
        GeneralSampleFreq->setObjectName(QString::fromUtf8("GeneralSampleFreq"));
        GeneralSampleFreq->setGeometry(QRect(340, 30, 121, 27));
        RefreshRate = new QSpinBox(Configuration);
        RefreshRate->setObjectName(QString::fromUtf8("RefreshRate"));
        RefreshRate->setGeometry(QRect(480, 30, 91, 27));
        label_refresh_rate = new QLabel(Configuration);
        label_refresh_rate->setObjectName(QString::fromUtf8("label_refresh_rate"));
        label_refresh_rate->setGeometry(QRect(480, 10, 91, 17));
        QWidget::setTabOrder(ProductKey, GeneralSampleFreq);
        QWidget::setTabOrder(GeneralSampleFreq, RefreshRate);
        QWidget::setTabOrder(RefreshRate, Backend);
        QWidget::setTabOrder(Backend, OutputDevice);
        QWidget::setTabOrder(OutputDevice, OutputSampleFreq);
        QWidget::setTabOrder(OutputSampleFreq, SpeakerSetup);
        QWidget::setTabOrder(SpeakerSetup, OutputSpeakers);
        QWidget::setTabOrder(OutputSpeakers, InputDevice);
        QWidget::setTabOrder(InputDevice, InputSampleFreq);
        QWidget::setTabOrder(InputSampleFreq, OK);
        QWidget::setTabOrder(OK, graphicsView);

        retranslateUi(Configuration);
        QObject::connect(OK, SIGNAL(clicked()), Configuration, SLOT(writeConfig()));
        QObject::connect(ProductKey, SIGNAL(textEdited(QString)), Configuration, SLOT(changeProductKey(QString)));
        QObject::connect(SpeakerSetup, SIGNAL(currentIndexChanged(int)), Configuration, SLOT(changeSpeakerSetup(int)));
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
        Configuration->setWindowTitle(QApplication::translate("Configuration", "Dialog", 0, QApplication::UnicodeUTF8));
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
        label_backend->setText(QApplication::translate("Configuration", "Backend:", 0, QApplication::UnicodeUTF8));
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
        label_output_device->setText(QApplication::translate("Configuration", "Output Device:", 0, QApplication::UnicodeUTF8));
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
        label_input_device->setText(QApplication::translate("Configuration", "Input Device:", 0, QApplication::UnicodeUTF8));
        OK->setText(QApplication::translate("Configuration", "OK", 0, QApplication::UnicodeUTF8));
        label_general_sample_freq->setText(QApplication::translate("Configuration", "Sample Frequency:", 0, QApplication::UnicodeUTF8));
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
        label_refresh_rate->setText(QApplication::translate("Configuration", "Refresh Rate:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Configuration: public Ui_Configuration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // AAXCONFIG_UI_H
