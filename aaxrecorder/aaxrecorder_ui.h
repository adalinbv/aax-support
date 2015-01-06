/********************************************************************************
** Form generated from reading UI file 'aaxrecorder.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef AAXRECORDER_UI_H
#define AAXRECORDER_UI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDial>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_AudioRecorder
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionHardware;
    QAction *actionAbout;
    QAction *actionLicense;
    QAction *actionOpenDir;
    QAction *actionInfo;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QPushButton *stopRecord;
    QLabel *timeCurrent;
    QLabel *timeRemaining;
    QProgressBar *pctRecording;
    QPushButton *startRecord;
    QLabel *timeTotal;
    QPushButton *pauseRecord;
    QProgressBar *VUleft;
    QProgressBar *VUright;
    QDial *volume;
    QLabel *labelVolumeMin;
    QLabel *labelVolumeMax;
    QLabel *labelVolume;

    void setupUi(QDialog *AudioRecorder)
    {
        if (AudioRecorder->objectName().isEmpty())
            AudioRecorder->setObjectName(QString::fromUtf8("AudioRecorder"));
        AudioRecorder->resize(400, 125);
        AudioRecorder->setMinimumSize(QSize(400, 125));
        AudioRecorder->setMaximumSize(QSize(400, 125));
        actionOpen = new QAction(AudioRecorder);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionExit = new QAction(AudioRecorder);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionHardware = new QAction(AudioRecorder);
        actionHardware->setObjectName(QString::fromUtf8("actionHardware"));
        actionAbout = new QAction(AudioRecorder);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionLicense = new QAction(AudioRecorder);
        actionLicense->setObjectName(QString::fromUtf8("actionLicense"));
        actionOpenDir = new QAction(AudioRecorder);
        actionOpenDir->setObjectName(QString::fromUtf8("actionOpenDir"));
        actionInfo = new QAction(AudioRecorder);
        actionInfo->setObjectName(QString::fromUtf8("actionInfo"));
        menubar = new QMenuBar(AudioRecorder);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 181, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        stopRecord = new QPushButton(AudioRecorder);
        stopRecord->setObjectName(QString::fromUtf8("stopRecord"));
        stopRecord->setGeometry(QRect(255, 80, 51, 25));
        stopRecord->setFocusPolicy(Qt::NoFocus);
        timeCurrent = new QLabel(AudioRecorder);
        timeCurrent->setObjectName(QString::fromUtf8("timeCurrent"));
        timeCurrent->setGeometry(QRect(145, 40, 71, 12));
        QFont font;
        font.setFamily(QString::fromUtf8("Liberation Sans Narrow"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        timeCurrent->setFont(font);
        timeCurrent->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        timeRemaining = new QLabel(AudioRecorder);
        timeRemaining->setObjectName(QString::fromUtf8("timeRemaining"));
        timeRemaining->setGeometry(QRect(315, 40, 71, 12));
        timeRemaining->setFont(font);
        timeRemaining->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pctRecording = new QProgressBar(AudioRecorder);
        pctRecording->setObjectName(QString::fromUtf8("pctRecording"));
        pctRecording->setGeometry(QRect(145, 57, 242, 12));
        pctRecording->setFont(font);
        pctRecording->setValue(24);
        startRecord = new QPushButton(AudioRecorder);
        startRecord->setObjectName(QString::fromUtf8("startRecord"));
        startRecord->setGeometry(QRect(145, 80, 51, 25));
        startRecord->setFocusPolicy(Qt::NoFocus);
        timeTotal = new QLabel(AudioRecorder);
        timeTotal->setObjectName(QString::fromUtf8("timeTotal"));
        timeTotal->setGeometry(QRect(315, 73, 71, 12));
        timeTotal->setFont(font);
        timeTotal->setLayoutDirection(Qt::LeftToRight);
        timeTotal->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pauseRecord = new QPushButton(AudioRecorder);
        pauseRecord->setObjectName(QString::fromUtf8("pauseRecord"));
        pauseRecord->setGeometry(QRect(200, 80, 51, 25));
        pauseRecord->setFocusPolicy(Qt::NoFocus);
        VUleft = new QProgressBar(AudioRecorder);
        VUleft->setObjectName(QString::fromUtf8("VUleft"));
        VUleft->setGeometry(QRect(10, 28, 14, 85));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        VUleft->setFont(font1);
        VUleft->setValue(0);
        VUleft->setOrientation(Qt::Vertical);
        VUright = new QProgressBar(AudioRecorder);
        VUright->setObjectName(QString::fromUtf8("VUright"));
        VUright->setGeometry(QRect(30, 28, 14, 85));
        VUright->setFont(font1);
        VUright->setValue(0);
        VUright->setOrientation(Qt::Vertical);
        volume = new QDial(AudioRecorder);
        volume->setObjectName(QString::fromUtf8("volume"));
        volume->setGeometry(QRect(60, 33, 70, 80));
        volume->setFocusPolicy(Qt::NoFocus);
        volume->setValue(80);
        volume->setNotchesVisible(true);
        labelVolumeMin = new QLabel(AudioRecorder);
        labelVolumeMin->setObjectName(QString::fromUtf8("labelVolumeMin"));
        labelVolumeMin->setGeometry(QRect(45, 98, 31, 16));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Liberation Sans Narrow"));
        font2.setPointSize(9);
        font2.setBold(false);
        font2.setWeight(50);
        labelVolumeMin->setFont(font2);
        labelVolumeMin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelVolumeMax = new QLabel(AudioRecorder);
        labelVolumeMax->setObjectName(QString::fromUtf8("labelVolumeMax"));
        labelVolumeMax->setGeometry(QRect(115, 98, 31, 16));
        labelVolumeMax->setFont(font2);
        labelVolumeMax->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelVolume = new QLabel(AudioRecorder);
        labelVolume->setObjectName(QString::fromUtf8("labelVolume"));
        labelVolume->setGeometry(QRect(60, 20, 70, 20));
        labelVolume->setFont(font);
        labelVolume->setAlignment(Qt::AlignCenter);
        QWidget::setTabOrder(startRecord, pauseRecord);
        QWidget::setTabOrder(pauseRecord, stopRecord);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpenDir);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionHardware);
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionLicense);
        menuHelp->addAction(actionAbout);

        retranslateUi(AudioRecorder);

        QMetaObject::connectSlotsByName(AudioRecorder);
    } // setupUi

    void retranslateUi(QDialog *AudioRecorder)
    {
        AudioRecorder->setWindowTitle(QApplication::translate("AudioRecorder", "AeonWave Audio Recorder", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        AudioRecorder->setToolTip(QApplication::translate("AudioRecorder", "<html><head/><body><p>Audio Recorder</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionOpen->setText(QApplication::translate("AudioRecorder", "Select File", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("AudioRecorder", "Select an Input File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionOpen->setShortcut(QApplication::translate("AudioRecorder", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("AudioRecorder", "Exit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExit->setToolTip(QApplication::translate("AudioRecorder", "Close the application", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExit->setShortcut(QApplication::translate("AudioRecorder", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionHardware->setText(QApplication::translate("AudioRecorder", "Configure", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionHardware->setToolTip(QApplication::translate("AudioRecorder", "Hardware Configuration", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionHardware->setShortcut(QApplication::translate("AudioRecorder", "Ctrl+H", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("AudioRecorder", "About", 0, QApplication::UnicodeUTF8));
        actionAbout->setShortcut(QApplication::translate("AudioRecorder", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        actionLicense->setText(QApplication::translate("AudioRecorder", "Copyright License", 0, QApplication::UnicodeUTF8));
        actionLicense->setShortcut(QApplication::translate("AudioRecorder", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionOpenDir->setText(QApplication::translate("AudioRecorder", "Open Directory", 0, QApplication::UnicodeUTF8));
        actionOpenDir->setShortcut(QApplication::translate("AudioRecorder", "Ctrl+D", 0, QApplication::UnicodeUTF8));
        actionInfo->setText(QApplication::translate("AudioRecorder", "Song Info", 0, QApplication::UnicodeUTF8));
        actionInfo->setShortcut(QApplication::translate("AudioRecorder", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("AudioRecorder", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("AudioRecorder", "Help", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        stopRecord->setToolTip(QApplication::translate("AudioRecorder", "<html><head/><body><p>Stop</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        timeCurrent->setToolTip(QApplication::translate("AudioRecorder", "<html><head/><body><p>Recording Time</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        timeCurrent->setText(QApplication::translate("AudioRecorder", "00:00:00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        timeRemaining->setToolTip(QApplication::translate("AudioRecorder", "<html><head/><body><p>Time Remaining</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        timeRemaining->setText(QApplication::translate("AudioRecorder", "00:00:00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        startRecord->setToolTip(QApplication::translate("AudioRecorder", "<html><head/><body><p>Record</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        timeTotal->setToolTip(QApplication::translate("AudioRecorder", "<html><head/><body><p>Total Time</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        timeTotal->setText(QApplication::translate("AudioRecorder", "00:00:00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pauseRecord->setToolTip(QApplication::translate("AudioRecorder", "<html><head/><body><p>Pause</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        VUleft->setToolTip(QApplication::translate("AudioRecorder", "<html><head/><body><br>Left channel</br><br>VU meter</br></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        VUleft->setFormat(QString());
#ifndef QT_NO_TOOLTIP
        VUright->setToolTip(QApplication::translate("AudioRecorder", "<html><head/><body><br>Right channel</br><br>VU meter</br></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        VUright->setFormat(QString());
        labelVolumeMin->setText(QApplication::translate("AudioRecorder", "min", 0, QApplication::UnicodeUTF8));
        labelVolumeMax->setText(QApplication::translate("AudioRecorder", "max", 0, QApplication::UnicodeUTF8));
        labelVolume->setText(QApplication::translate("AudioRecorder", "volume", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AudioRecorder: public Ui_AudioRecorder {};
} // namespace Ui

QT_END_NAMESPACE

#endif // AAXRECORDER_UI_H
