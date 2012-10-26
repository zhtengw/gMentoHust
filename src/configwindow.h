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
#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QDialog>

class QPushButton;
class QTextEdit;
class QProcess;
class QComboBox;
class QLabel;
class QButtonGroup;
class QCheckBox;
class QString;
class QLineEdit;

class ConfigWindow : public QDialog
{
    Q_OBJECT
public:
    ConfigWindow(QWidget *parent = 0);
    ~ConfigWindow();
    friend class MainWindow;

private slots:
    void confirmClicked();
    void showNetCards();
    void saveNetCard(const QString name);
    void saveMulAdr(const int index);
    void saveDispNotif(const QString time);

private:
    //void createNetCardSelect();
    //void createMulcastSelect();
    void setArgs();
    QStringList *args;

    QPushButton *confirmButton;
    QPushButton *cancelButton;
    QTextEdit *temp;

    QLabel *netCardName;
    QProcess *getNetCard;
    QComboBox *netCardSelect;
    QStringList *netCardArg;

    QLabel *mulCastAdrName;
    QComboBox *mulCastAdr;
    QStringList *mulCastAdrArg;

    QLabel *dispNotif;
    QLineEdit *dispNotifTime;
    QStringList *dispNotifArg;
};

#endif // CONFIGWINDOW_H
