/********************************************************************************
** Form generated from reading UI file 'remote.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef REMOTE_UI_H
#define REMOTE_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_Remote
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *remoteURL;

    void setupUi(QDialog *Connect)
    {
        if (Connect->objectName().isEmpty())
            Connect->setObjectName(QString::fromUtf8("Connect"));
        Connect->resize(374, 188);
        buttonBox = new QDialogButtonBox(Connect);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 140, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(Connect);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 30, 321, 51));
        label->setWordWrap(true);
        remoteURL = new QLineEdit(Connect);
        remoteURL->setObjectName(QString::fromUtf8("remoteURL"));
        remoteURL->setGeometry(QRect(30, 90, 321, 27));

        retranslateUi(Connect);

        QMetaObject::connectSlotsByName(Connect);
    } // setupUi

    void retranslateUi(QDialog *Connect)
    {
        Connect->setWindowTitle(QApplication::translate("Remote", "Remote Connect", 0));
        label->setText(QApplication::translate("Remote", "Type the URL of the remote streaming server to connect to. Only mp3 streams over http are supported.", 0));
    } // retranslateUi

};

namespace Ui {
    class Remote: public Ui_Remote {};
} // namespace Ui

QT_END_NAMESPACE

#endif // REMOTE_UI_H
