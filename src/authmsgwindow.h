/***************************************************************************
 *  Copyright (c) 2012 by Aten Zhang <atenzd@gmail.com>                    *
 *                                                                         *
 *  This file is part of gMentoHust.                                       *
 *                                                                         *
 *  gMentoHust is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  gMentoHust is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with gMentoHust.  If not, see <http://www.gnu.org/licenses/>.    *
 ***************************************************************************/
#ifndef AUTHMSGWINDOW_H
#define AUTHMSGWINDOW_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QTextEdit>
#include <QPushButton>
#include <QProcess>
#include <QString>
#include <QIcon>
#include <QMenu>

class AuthMsgWindow : public QDialog
{
    Q_OBJECT

public:
    AuthMsgWindow(QWidget *parent = 0);
    ~AuthMsgWindow();


    friend class MainWindow;

//signals:
//    void authMWhidden();

private slots:
    void readresult();
    void exitClicked();
    void miniClicked();
    void trayIconAct(QSystemTrayIcon::ActivationReason reason);
    void showMessage();

private:
    void setArgs(const QString &id, const QString &pd);
    void trayActions();
    void createTrayMenu();

    QTextEdit *authMsg;
    QPushButton *exitButton;
    QPushButton *miniButton;
    QProcess *backend;
    QStringList *args;
    QSystemTrayIcon *sysTrayIcon;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *exitAction;
    QAction *restoreAction;
    QAction *quitAction;
    //QIcon *icon;
};

#endif // AUTHMSGWINDOW_H
