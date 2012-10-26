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
#include <QtGui>

#include "mainwindow.h"
#include "configwindow.h"
#include "authmsgwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{

     QSettings setting("Aten","Gmentohust");


    //authMW = new AuthMsgWindow;
    //confW = new ConfigWindow;
    netid = new QLabel(tr("NetID:"));
    idEdit = new QLineEdit;
    netid->setBuddy(idEdit);
    idEdit->setText(setting.value("netid","").toString());

    passwd = new QLabel(tr("Password:"));
    pdEdit = new QLineEdit;
    pdEdit->setEchoMode(QLineEdit::Password);
    passwd->setBuddy(pdEdit);
    pdEdit->setText(setting.value("password","").toString());

    rembCheckBox = new QCheckBox(tr("Remember for next authification"));

    authButton = new QPushButton(tr("Authenticate"));
    authButton->setDefault(true);
    authButton->setEnabled(false);

    closeButton = new QPushButton(tr("Close"));
    configButton = new QPushButton(tr("Configure"));

    createAuthMW();
    createCfgWd();

    enableAuthButton("");
    connect(pdEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableAuthButton(const QString&)));
    connect(idEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableAuthButton(const QString&)));


    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    //connect(configButton,SIGNAL(clicked()),this,SLOT(openCfgWd()));

    connect(rembCheckBox, SIGNAL(stateChanged(int)), this, SLOT(saveID(int)));

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(netid);
    topLeftLayout->addWidget(idEdit);

    QHBoxLayout *midLeftLayout = new QHBoxLayout;
    midLeftLayout->addWidget(passwd);
    midLeftLayout->addWidget(pdEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addLayout(midLeftLayout);
    leftLayout->addWidget(rembCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(authButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addWidget(configButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("MentoHust"));
    setFixedHeight(sizeHint().height());

}

MainWindow::~MainWindow()
{
    
}


void MainWindow::createAuthMW()
{

    authMW = new AuthMsgWindow;
    connect(authButton, SIGNAL(clicked()), this, SLOT(authClicked()));
    connect(authButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(authMW, SIGNAL(destroyed()), this, SLOT(show()));
    connect(authMW, SIGNAL(destroyed()), this, SLOT(createAuthMW()));

}
void MainWindow::authClicked()
{
    QString tempid = idEdit->text();
    QString temppd = pdEdit->text();

    authMW->setArgs(tempid, temppd);
    confW->setArgs();
    authMW->backend->start("mentohust", *authMW->args<<*confW->args);
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        authMW->sysTrayIcon->show();
    }
    authMW->show();

}

void MainWindow::enableAuthButton(const QString &text)
{
    authButton->setEnabled(!(pdEdit->text().isEmpty()||idEdit->text().isEmpty()));
}

void MainWindow::createCfgWd()
{
    confW= new ConfigWindow;
    connect(confW,SIGNAL(destroyed()),this,SLOT(createCfgWd()));
    connect(configButton, SIGNAL(clicked()), confW, SLOT(show()));
}

void MainWindow::showAndHideIcon()
{
    this->show();
    //authMW->sysTrayIcon->hide();
}

void MainWindow::saveID(int state)
{
    if(state == 2 )
    {
        //QMessageBox::information(this,tr("ss"),tr("I'm checked!"));
        QSettings setting("Aten","Gmentohust");
        setting.setValue("netid",idEdit->text());
        setting.setValue("password",pdEdit->text());
    }
}
