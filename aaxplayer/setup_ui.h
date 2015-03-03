/********************************************************************************
** Form generated from reading UI file 'setup.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SETUP_UI_H
#define SETUP_UI_H

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

QT_BEGIN_NAMESPACE

class Ui_Configuration
{
public:
    QLabel *label_input_device;
    QComboBox *OutputDevice;
    QDialogButtonBox *cfgOk;
    QComboBox *OutputInterface;
    QCheckBox *enableAGC;
    QCheckBox *enableAutoPlay;
    QCheckBox *enableVUmeters;

    void setupUi(QDialog *Configuration)
    {
        if (Configuration->objectName().isEmpty())
            Configuration->setObjectName(QString::fromUtf8("Configuration"));
        Configuration->resize(290, 200);
        Configuration->setMinimumSize(QSize(290, 200));
        Configuration->setMaximumSize(QSize(290, 200));
        label_input_device = new QLabel(Configuration);
        label_input_device->setObjectName(QString::fromUtf8("label_input_device"));
        label_input_device->setGeometry(QRect(20, 10, 111, 20));
        OutputDevice = new QComboBox(Configuration);
        OutputDevice->setObjectName(QString::fromUtf8("OutputDevice"));
        OutputDevice->setGeometry(QRect(20, 30, 250, 27));
        OutputDevice->setMinimumSize(QSize(250, 27));
        OutputDevice->setMaximumSize(QSize(250, 27));
        OutputDevice->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        cfgOk = new QDialogButtonBox(Configuration);
        cfgOk->setObjectName(QString::fromUtf8("cfgOk"));
        cfgOk->setGeometry(QRect(90, 160, 181, 27));
        cfgOk->setMinimumSize(QSize(80, 22));
        cfgOk->setFocusPolicy(Qt::NoFocus);
        cfgOk->setStyleSheet(QString::fromUtf8("min-width: 80px;\n"
"min-height: 22px;"));
        cfgOk->setOrientation(Qt::Horizontal);
        cfgOk->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        OutputInterface = new QComboBox(Configuration);
        OutputInterface->setObjectName(QString::fromUtf8("OutputInterface"));
        OutputInterface->setGeometry(QRect(20, 60, 250, 27));
        OutputInterface->setMinimumSize(QSize(250, 27));
        OutputInterface->setMaximumSize(QSize(250, 27));
        OutputInterface->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        enableAGC = new QCheckBox(Configuration);
        enableAGC->setObjectName(QString::fromUtf8("enableAGC"));
        enableAGC->setEnabled(true);
        enableAGC->setGeometry(QRect(20, 130, 251, 22));
        enableAGC->setChecked(true);
        enableAutoPlay = new QCheckBox(Configuration);
        enableAutoPlay->setObjectName(QString::fromUtf8("enableAutoPlay"));
        enableAutoPlay->setGeometry(QRect(20, 110, 251, 22));
        enableVUmeters = new QCheckBox(Configuration);
        enableVUmeters->setObjectName(QString::fromUtf8("enableVUmeters"));
        enableVUmeters->setGeometry(QRect(20, 90, 251, 22));
        QWidget::setTabOrder(OutputDevice, OutputInterface);
        QWidget::setTabOrder(OutputInterface, enableAutoPlay);
        QWidget::setTabOrder(enableAutoPlay, enableAGC);

        retranslateUi(Configuration);
        QObject::connect(cfgOk, SIGNAL(accepted()), Configuration, SLOT(accept()));
        QObject::connect(cfgOk, SIGNAL(rejected()), Configuration, SLOT(reject()));

        QMetaObject::connectSlotsByName(Configuration);
    } // setupUi

    void retranslateUi(QDialog *Configuration)
    {
        Configuration->setWindowTitle(QApplication::translate("Configuration", "Hardware Configuration", 0, QApplication::UnicodeUTF8));
        label_input_device->setText(QApplication::translate("Configuration", "Output Device:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        enableAGC->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>AGC (Auto Gain Control) automatically adjusts the input gain to 0dB.</p><p>As a result all files will sound about equally loud.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        enableAGC->setText(QApplication::translate("Configuration", "Enable AGC", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        enableAutoPlay->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Auto Play will start the newly selected file from the file selection dialog as soon as the input device is ready (finished playing the previous track).</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        enableAutoPlay->setText(QApplication::translate("Configuration", "Enable Auto Play", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        enableVUmeters->setToolTip(QApplication::translate("Configuration", "<html><head/><body><p>Auto Play will start the newly selected file from the file selection dialog as soon as the input device is ready (finished playing the previous track).</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        enableVUmeters->setText(QApplication::translate("Configuration", "Show VU meters", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Configuration: public Ui_Configuration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SETUP_UI_H
