/********************************************************************************
** Form generated from reading UI file 'record.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef RECORD_UI_H
#define RECORD_UI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTimeEdit>

QT_BEGIN_NAMESPACE

class Ui_Record
{
public:
    QDialogButtonBox *buttonBox;
    QTimeEdit *timeEdit;
    QLabel *label;

    void setupUi(QDialog *Recording)
    {
        if (Recording->objectName().isEmpty())
            Recording->setObjectName(QString::fromUtf8("Recording"));
        Recording->resize(374, 188);
        buttonBox = new QDialogButtonBox(Recording);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 140, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        timeEdit = new QTimeEdit(Recording);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setGeometry(QRect(210, 90, 101, 27));
        label = new QLabel(Recording);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 30, 321, 51));
        label->setWordWrap(true);

        retranslateUi(Recording);

        QMetaObject::connectSlotsByName(Recording);
    } // setupUi

    void retranslateUi(QDialog *Recording)
    {
        Recording->setWindowTitle(QApplication::translate("Record", "Recording Time", 0, QApplication::UnicodeUTF8));
        timeEdit->setDisplayFormat(QApplication::translate("Record", "HH:mm:ss", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Record", "If you want to define a time length restriction, set it below and the progress indicator will show how much time is left.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Record: public Ui_Record {};
} // namespace Ui

QT_END_NAMESPACE

#endif // RECORD_UI_H
