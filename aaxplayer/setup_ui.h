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
    QComboBox *RecordBitRate;

    void setupUi(QDialog *Configuration)
    {
        if (Configuration->objectName().isEmpty())
            Configuration->setObjectName(QString::fromUtf8("Configuration"));
        Configuration->resize(290, 175);
        Configuration->setMinimumSize(QSize(290, 175));
        Configuration->setMaximumSize(QSize(290, 175));
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
        cfgOk->setGeometry(QRect(90, 130, 181, 27));
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
        RecordBitRate = new QComboBox(Configuration);
        RecordBitRate->setObjectName(QString::fromUtf8("RecordBitRate"));
        RecordBitRate->setGeometry(QRect(20, 90, 100, 27));
        RecordBitRate->setMinimumSize(QSize(100, 27));
        RecordBitRate->setMaximumSize(QSize(100, 27));
        RecordBitRate->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        QWidget::setTabOrder(OutputDevice, cfgOk);

        retranslateUi(Configuration);
        QObject::connect(cfgOk, SIGNAL(accepted()), Configuration, SLOT(accept()));
        QObject::connect(cfgOk, SIGNAL(rejected()), Configuration, SLOT(reject()));

        QMetaObject::connectSlotsByName(Configuration);
    } // setupUi

    void retranslateUi(QDialog *Configuration)
    {
        Configuration->setWindowTitle(QApplication::translate("Configuration", "Hardware Configuration", 0, QApplication::UnicodeUTF8));
        label_input_device->setText(QApplication::translate("Configuration", "Output Device:", 0, QApplication::UnicodeUTF8));
        RecordBitRate->clear();
        RecordBitRate->insertItems(0, QStringList()
         << QApplication::translate("Configuration", "64 kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "128 kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "192 kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "256 kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Configuration", "320 kbps", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class Configuration: public Ui_Configuration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SETUP_UI_H
