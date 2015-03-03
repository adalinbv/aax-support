/********************************************************************************
** Form generated from reading UI file 'aaxplayer.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef AAXPLAYER_UI_H
#define AAXPLAYER_UI_H

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

class Ui_AudioPlayer
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionHardware;
    QAction *actionAbout;
    QAction *actionLicense;
    QAction *actionOpenDir;
    QAction *actionInfo;
    QAction *actionConnect;
    QAction *actionAddFavorite;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuView;
    QPushButton *stopPlay;
    QLabel *timeCurrent;
    QLabel *timeRemaining;
    QProgressBar *pctPlaying;
    QPushButton *startPlay;
    QLabel *timeTotal;
    QPushButton *pausePlay;
    QDial *volume;
    QLabel *labelVolumeMin;
    QLabel *labelVolumeMax;
    QLabel *labelVolume;

    void setupUi(QDialog *AudioPlayer)
    {
        if (AudioPlayer->objectName().isEmpty())
            AudioPlayer->setObjectName(QString::fromUtf8("AudioPlayer"));
        AudioPlayer->resize(350, 125);
        AudioPlayer->setMinimumSize(QSize(350, 125));
        AudioPlayer->setMaximumSize(QSize(350, 125));
        actionOpen = new QAction(AudioPlayer);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionExit = new QAction(AudioPlayer);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionHardware = new QAction(AudioPlayer);
        actionHardware->setObjectName(QString::fromUtf8("actionHardware"));
        actionAbout = new QAction(AudioPlayer);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionLicense = new QAction(AudioPlayer);
        actionLicense->setObjectName(QString::fromUtf8("actionLicense"));
        actionOpenDir = new QAction(AudioPlayer);
        actionOpenDir->setObjectName(QString::fromUtf8("actionOpenDir"));
        actionInfo = new QAction(AudioPlayer);
        actionInfo->setObjectName(QString::fromUtf8("actionInfo"));
        actionConnect = new QAction(AudioPlayer);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionAddFavorite = new QAction(AudioPlayer);
        actionAddFavorite->setObjectName(QString::fromUtf8("actionAddFavorite"));
        menubar = new QMenuBar(AudioPlayer);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 351, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        stopPlay = new QPushButton(AudioPlayer);
        stopPlay->setObjectName(QString::fromUtf8("stopPlay"));
        stopPlay->setGeometry(QRect(125, 80, 51, 25));
        stopPlay->setFocusPolicy(Qt::NoFocus);
        timeCurrent = new QLabel(AudioPlayer);
        timeCurrent->setObjectName(QString::fromUtf8("timeCurrent"));
        timeCurrent->setGeometry(QRect(15, 40, 71, 12));
        QFont font;
        font.setFamily(QString::fromUtf8("Liberation Sans Narrow"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        timeCurrent->setFont(font);
        timeCurrent->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        timeRemaining = new QLabel(AudioPlayer);
        timeRemaining->setObjectName(QString::fromUtf8("timeRemaining"));
        timeRemaining->setGeometry(QRect(185, 40, 71, 12));
        timeRemaining->setFont(font);
        timeRemaining->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pctPlaying = new QProgressBar(AudioPlayer);
        pctPlaying->setObjectName(QString::fromUtf8("pctPlaying"));
        pctPlaying->setGeometry(QRect(15, 57, 242, 12));
        pctPlaying->setFont(font);
        pctPlaying->setValue(24);
        startPlay = new QPushButton(AudioPlayer);
        startPlay->setObjectName(QString::fromUtf8("startPlay"));
        startPlay->setGeometry(QRect(15, 80, 51, 25));
        startPlay->setFocusPolicy(Qt::NoFocus);
        timeTotal = new QLabel(AudioPlayer);
        timeTotal->setObjectName(QString::fromUtf8("timeTotal"));
        timeTotal->setGeometry(QRect(185, 73, 71, 12));
        timeTotal->setFont(font);
        timeTotal->setLayoutDirection(Qt::LeftToRight);
        timeTotal->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pausePlay = new QPushButton(AudioPlayer);
        pausePlay->setObjectName(QString::fromUtf8("pausePlay"));
        pausePlay->setGeometry(QRect(70, 80, 51, 25));
        pausePlay->setFocusPolicy(Qt::NoFocus);
        volume = new QDial(AudioPlayer);
        volume->setObjectName(QString::fromUtf8("volume"));
        volume->setGeometry(QRect(270, 35, 70, 80));
        volume->setFocusPolicy(Qt::NoFocus);
        volume->setValue(80);
        volume->setNotchesVisible(true);
        labelVolumeMin = new QLabel(AudioPlayer);
        labelVolumeMin->setObjectName(QString::fromUtf8("labelVolumeMin"));
        labelVolumeMin->setGeometry(QRect(255, 100, 31, 16));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Liberation Sans Narrow"));
        font1.setPointSize(9);
        font1.setBold(false);
        font1.setWeight(50);
        labelVolumeMin->setFont(font1);
        labelVolumeMin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelVolumeMax = new QLabel(AudioPlayer);
        labelVolumeMax->setObjectName(QString::fromUtf8("labelVolumeMax"));
        labelVolumeMax->setGeometry(QRect(325, 100, 31, 16));
        labelVolumeMax->setFont(font1);
        labelVolumeMax->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelVolume = new QLabel(AudioPlayer);
        labelVolume->setObjectName(QString::fromUtf8("labelVolume"));
        labelVolume->setGeometry(QRect(270, 22, 70, 20));
        labelVolume->setFont(font);
        labelVolume->setAlignment(Qt::AlignCenter);
        QWidget::setTabOrder(startPlay, pausePlay);
        QWidget::setTabOrder(pausePlay, stopPlay);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionConnect);
        menuFile->addAction(actionOpenDir);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionHardware);
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionLicense);
        menuHelp->addAction(actionAbout);
        menuView->addAction(actionInfo);
        menuView->addAction(actionAddFavorite);

        retranslateUi(AudioPlayer);

        QMetaObject::connectSlotsByName(AudioPlayer);
    } // setupUi

    void retranslateUi(QDialog *AudioPlayer)
    {
        AudioPlayer->setWindowTitle(QApplication::translate("AudioPlayer", "AeonWave Audio Player", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        AudioPlayer->setToolTip(QApplication::translate("AudioPlayer", "<html><head/><body><p>Audio Player</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionOpen->setText(QApplication::translate("AudioPlayer", "Select File", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("AudioPlayer", "Select an Input File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionOpen->setShortcut(QApplication::translate("AudioPlayer", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("AudioPlayer", "Exit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExit->setToolTip(QApplication::translate("AudioPlayer", "Close the application", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExit->setShortcut(QApplication::translate("AudioPlayer", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionHardware->setText(QApplication::translate("AudioPlayer", "Configure", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionHardware->setToolTip(QApplication::translate("AudioPlayer", "Hardware Configuration", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionHardware->setShortcut(QApplication::translate("AudioPlayer", "Ctrl+H", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("AudioPlayer", "About", 0, QApplication::UnicodeUTF8));
        actionAbout->setShortcut(QApplication::translate("AudioPlayer", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        actionLicense->setText(QApplication::translate("AudioPlayer", "Copyright License", 0, QApplication::UnicodeUTF8));
        actionLicense->setShortcut(QApplication::translate("AudioPlayer", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionOpenDir->setText(QApplication::translate("AudioPlayer", "Open Directory", 0, QApplication::UnicodeUTF8));
        actionOpenDir->setShortcut(QApplication::translate("AudioPlayer", "Ctrl+D", 0, QApplication::UnicodeUTF8));
        actionInfo->setText(QApplication::translate("AudioPlayer", "Song Info", 0, QApplication::UnicodeUTF8));
        actionInfo->setShortcut(QApplication::translate("AudioPlayer", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("AudioPlayer", "Open Stream", 0, QApplication::UnicodeUTF8));
        actionConnect->setShortcut(QApplication::translate("AudioPlayer", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionAddFavorite->setText(QApplication::translate("AudioPlayer", "Add Favorite", 0, QApplication::UnicodeUTF8));
        actionAddFavorite->setShortcut(QApplication::translate("AudioPlayer", "Alt+F", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("AudioPlayer", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("AudioPlayer", "Help", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("AudioPlayer", "View", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        stopPlay->setToolTip(QApplication::translate("AudioPlayer", "<html><head/><body><p>Stop</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        timeCurrent->setToolTip(QApplication::translate("AudioPlayer", "<html><head/><body><p>Playing Time</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        timeCurrent->setText(QApplication::translate("AudioPlayer", "00:00:00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        timeRemaining->setToolTip(QApplication::translate("AudioPlayer", "<html><head/><body><p>Time Remaining</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        timeRemaining->setText(QApplication::translate("AudioPlayer", "00:00:00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        startPlay->setToolTip(QApplication::translate("AudioPlayer", "<html><head/><body><p>Play</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        timeTotal->setToolTip(QApplication::translate("AudioPlayer", "<html><head/><body><p>Total Time</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        timeTotal->setText(QApplication::translate("AudioPlayer", "00:00:00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pausePlay->setToolTip(QApplication::translate("AudioPlayer", "<html><head/><body><p>Pause</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelVolumeMin->setText(QApplication::translate("AudioPlayer", "min", 0, QApplication::UnicodeUTF8));
        labelVolumeMax->setText(QApplication::translate("AudioPlayer", "max", 0, QApplication::UnicodeUTF8));
        labelVolume->setText(QApplication::translate("AudioPlayer", "volume", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AudioPlayer: public Ui_AudioPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // AAXPLAYER_UI_H
